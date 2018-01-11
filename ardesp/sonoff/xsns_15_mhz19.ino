/*
  xsns_15_mhz19.ino - MH-Z19(B) CO2 sensor support for Sonoff-Tasmota

  Copyright (C) 2018  Theo Arends

  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifdef USE_MHZ19
/*********************************************************************************************\
 * MH-Z19 - CO2 sensor
 *
 * Adapted from  EspEasy plugin P049 by Dmitry (rel22 ___ inbox.ru)
 **********************************************************************************************
 * Filter usage
 *
 * Select filter usage on low stability readings
\*********************************************************************************************/

enum MhzFilterOptions {MHZ19_FILTER_OFF, MHZ19_FILTER_OFF_ALLSAMPLES, MHZ19_FILTER_FAST, MHZ19_FILTER_MEDIUM, MHZ19_FILTER_SLOW};

#define MHZ19_FILTER_OPTION          MHZ19_FILTER_FAST

/*********************************************************************************************\
 * Source: http://www.winsen-sensor.com/d/files/infrared-gas-sensor/mh-z19b-co2-ver1_0.pdf
 *
 * Automatic Baseline Correction (ABC logic function)
 *
 * ABC logic function refers to that sensor itself do zero point judgment and automatic calibration procedure
 * intelligently after a continuous operation period. The automatic calibration cycle is every 24 hours after powered on.
 *
 * The zero point of automatic calibration is 400ppm.
 *
 * This function is usually suitable for indoor air quality monitor such as offices, schools and homes,
 * not suitable for greenhouse, farm and refrigeratory where this function should be off.
 *
 * Please do zero calibration timely, such as manual or commend calibration.
\*********************************************************************************************/

#define MHZ19_ABC_ENABLE             1       // Automatic Baseline Correction (0 = off, 1 = on (default))

/*********************************************************************************************/

#include <TasmotaSerial.h>

#ifndef CO2_LOW
#define CO2_LOW                      800     // Below this CO2 value show green light
#endif
#ifndef CO2_HIGH
#define CO2_HIGH                     1200    // Above this CO2 value show red light
#endif

#define MHZ19_READ_TIMEOUT           500     // Must be way less than 1000
#define MHZ19_RETRY_COUNT            8

TasmotaSerial *MhzSerial;

const char kMhzTypes[] PROGMEM = "MHZ19|MHZ19B";

const uint8_t mhz_cmnd_read_ppm[9] =    {0xFF, 0x01, 0x86, 0x00, 0x00, 0x00, 0x00, 0x00, 0x79};
const uint8_t mhz_cmnd_abc_enable[9] =  {0xFF, 0x01, 0x79, 0xA0, 0x00, 0x00, 0x00, 0x00, 0xE6};
const uint8_t mhz_cmnd_abc_disable[9] = {0xFF, 0x01, 0x79, 0x00, 0x00, 0x00, 0x00, 0x00, 0x86};

uint8_t mhz_type = 1;
uint16_t mhz_last_ppm = 0;
uint8_t mhz_filter = MHZ19_FILTER_OPTION;
bool mhz_abc_enable = MHZ19_ABC_ENABLE;
bool mhz_abc_must_apply = false;
char mhz_types[7];

float mhz_temperature = 0;
uint8_t mhz_timer = 0;
uint8_t mhz_retry = MHZ19_RETRY_COUNT;

uint8_t mhz_state = 0;

/*********************************************************************************************/

bool MhzCheckAndApplyFilter(uint16_t ppm, uint8_t s)
{
  if (1 == s) {
    return false;            // S==1 => "A" version sensor bootup, do not use values.
  }
  if (mhz_last_ppm < 400 || mhz_last_ppm > 5000) {
    // Prevent unrealistic values during start-up with filtering enabled.
    // Just assume the entered value is correct.
    mhz_last_ppm = ppm;
    return true;
  }
  int32_t difference = ppm - mhz_last_ppm;
  if (s > 0 && s < 64 && mhz_filter != MHZ19_FILTER_OFF) {
    // Not the "B" version of the sensor, S value is used.
    // S==0 => "B" version, else "A" version
    // The S value is an indication of the stability of the reading.
    // S == 64 represents a stable reading and any lower value indicates (unusual) fast change.
    // Now we increase the delay filter for low values of S and increase response time when the
    // value is more stable.
    // This will make the reading useful in more turbulent environments,
    // where the sensor would report more rapid change of measured values.
    difference *= s;
    difference /= 64;
  }
  if (MHZ19_FILTER_OFF == mhz_filter) {
    if (s != 0 && s != 64) {
      return false;
    }
  } else {
    difference >>= (mhz_filter -1);
  }
  mhz_last_ppm = static_cast<uint16_t>(mhz_last_ppm + difference);
  return true;
}

void Mhz50ms()
{
  mhz_state++;
  if (4 == mhz_state) {  // Every 200 mSec
    mhz_state = 0;

    uint8_t mhz_response[9];

    mhz_timer++;
    if (6 == mhz_timer) {  // MH-Z19 measuring cycle takes 1005 +5% ms
      mhz_timer = 0;

      MhzSerial->write(mhz_cmnd_read_ppm, 9);
    }

    if (1 == mhz_timer) {
      if (mhz_retry) {
        mhz_retry--;
        if (!mhz_retry) {
          mhz_last_ppm = 0;
          mhz_temperature = 0;
        }
      }

      unsigned long  start = millis();
      uint8_t counter = 0;
      while (((millis() - start) < MHZ19_READ_TIMEOUT) && (counter < 9)) {
        if (MhzSerial->available() > 0) {
          mhz_response[counter++] = MhzSerial->read();
        }
      }
      if (counter < 9) {
//        AddLog_P(LOG_LEVEL_DEBUG, PSTR(D_LOG_DEBUG "MH-Z19 comms timeout"));
        return;
      }

      byte crc = 0;
      for (uint8_t i = 1; i < 8; i++) {
        crc += mhz_response[i];
      }
      crc = 255 - crc;
      crc++;
      if (mhz_response[8] != crc) {
//        AddLog_P(LOG_LEVEL_DEBUG, PSTR(D_LOG_DEBUG "MH-Z19 crc error"));
        return;
      }
      if (0xFF != mhz_response[0] || 0x86 != mhz_response[1]) {
//        AddLog_P(LOG_LEVEL_DEBUG, PSTR(D_LOG_DEBUG "MH-Z19 bad response"));
        return;
      }

      uint16_t u = (mhz_response[6] << 8) | mhz_response[7];
      if (15000 == u) {      // During (and only ever at) sensor boot, 'u' is reported as 15000
        if (!mhz_abc_enable) {
          // After bootup of the sensor the ABC will be enabled.
          // Thus only actively disable after bootup.
          mhz_abc_must_apply = true;
        }
      } else {
        uint16_t ppm = (mhz_response[2] << 8) | mhz_response[3];
        mhz_temperature = ConvertTemp((float)mhz_response[4] - 40);
        uint8_t s = mhz_response[5];
        mhz_type = (s) ? 1 : 2;
        if (MhzCheckAndApplyFilter(ppm, s)) {
          mhz_retry = MHZ19_RETRY_COUNT;
          LightSetSignal(CO2_LOW, CO2_HIGH, mhz_last_ppm);

          if (0 == s || 64 == s) {  // Reading is stable.
            if (mhz_abc_must_apply) {
              mhz_abc_must_apply = false;
              if (mhz_abc_enable) {
                MhzSerial->write(mhz_cmnd_abc_enable, 9);   // Sent sensor ABC Enable
              } else {
                MhzSerial->write(mhz_cmnd_abc_disable, 9);  // Sent sensor ABC Disable
              }
            }
          }

        }
      }
    }

  }
}

/*********************************************************************************************/

void MhzInit()
{
  mhz_type = 0;
  if ((pin[GPIO_MHZ_RXD] < 99) && (pin[GPIO_MHZ_TXD] < 99)) {
    MhzSerial = new TasmotaSerial(pin[GPIO_MHZ_RXD], pin[GPIO_MHZ_TXD]);
    if (MhzSerial->begin()) {
      mhz_type = 1;
    }
  }
}

void MhzShow(boolean json)
{
  char temperature[10];
  dtostrfd(mhz_temperature, Settings.flag2.temperature_resolution, temperature);
  GetTextIndexed(mhz_types, sizeof(mhz_types), mhz_type -1, kMhzTypes);

  if (json) {
    snprintf_P(mqtt_data, sizeof(mqtt_data), PSTR("%s,\"%s\":{\"" D_JSON_CO2 "\":%d,\"" D_JSON_TEMPERATURE "\":%s}"), mqtt_data, mhz_types, mhz_last_ppm, temperature);
#ifdef USE_DOMOTICZ
    DomoticzSensor(DZ_AIRQUALITY, mhz_last_ppm);
#endif  // USE_DOMOTICZ
#ifdef USE_WEBSERVER
  } else {
    snprintf_P(mqtt_data, sizeof(mqtt_data), HTTP_SNS_CO2, mqtt_data, mhz_types, mhz_last_ppm);
    snprintf_P(mqtt_data, sizeof(mqtt_data), HTTP_SNS_TEMP, mqtt_data, mhz_types, temperature, TempUnit());
#endif  // USE_WEBSERVER
  }
}

/*********************************************************************************************\
 * Interface
\*********************************************************************************************/

#define XSNS_15

boolean Xsns15(byte function)
{
  boolean result = false;

  if (mhz_type) {
    switch (function) {
      case FUNC_INIT:
        MhzInit();
        break;
      case FUNC_EVERY_50_MSECOND:
        Mhz50ms();
        break;
      case FUNC_JSON_APPEND:
        MhzShow(1);
        break;
#ifdef USE_WEBSERVER
      case FUNC_WEB_APPEND:
        MhzShow(0);
        break;
#endif  // USE_WEBSERVER
    }
  }
  return result;
}

#endif  // USE_MHZ19
