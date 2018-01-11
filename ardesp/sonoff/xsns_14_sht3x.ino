/*
  xsns_14_sht3x.ino - SHT3X temperature and humidity sensor support for Sonoff-Tasmota

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

#ifdef USE_I2C
#ifdef USE_SHT3X
/*********************************************************************************************\
 * SHT3X - Temperature and Humidy
 *
 * I2C Address: 0x44 or 0x45
\*********************************************************************************************/

#define SHT3X_ADDR_GND      0x44       // address pin low (GND)
#define SHT3X_ADDR_VDD      0x45       // address pin high (VDD)

uint8_t sht3x_type = 0;
uint8_t sht3x_address;
uint8_t sht3x_addresses[] = { SHT3X_ADDR_GND, SHT3X_ADDR_VDD };

bool Sht3xRead(float &t, float &h)
{
  unsigned int data[6];

  t = NAN;
  h = NAN;

  Wire.beginTransmission(sht3x_address);
  Wire.write(0x2C);                    // Enable clock stretching
  Wire.write(0x06);                    // High repeatability
  if (Wire.endTransmission() != 0) {   // Stop I2C transmission
    return false;
  }
  delay(30);                           // Timing verified with logic analyzer (10 is to short)
  Wire.requestFrom(sht3x_address, (uint8_t)6);   // Request 6 bytes of data
  for (int i = 0; i < 6; i++) {
    data[i] = Wire.read();             // cTemp msb, cTemp lsb, cTemp crc, humidity msb, humidity lsb, humidity crc
  };
  t = ConvertTemp((float)((((data[0] << 8) | data[1]) * 175) / 65535.0) - 45);
  h = (float)((((data[3] << 8) | data[4]) * 100) / 65535.0);
  return (!isnan(t) && !isnan(h));
}

/********************************************************************************************/

void Sht3xDetect()
{
  if (sht3x_type) {
    return;
  }

  float t;
  float h;
  sht3x_type = 1;
  for (byte i = 0; i < sizeof(sht3x_addresses); i++) {
    sht3x_address = sht3x_addresses[i];
    if (Sht3xRead(t, h)) {
      snprintf_P(log_data, sizeof(log_data), S_LOG_I2C_FOUND_AT, "SHT3X", sht3x_address);
      AddLog(LOG_LEVEL_DEBUG);
      return;
    }
  }
  sht3x_type = 0;
}

void Sht3xShow(boolean json)
{
  if (sht3x_type) {
    float t;
    float h;
    if (Sht3xRead(t, h)) {
      char temperature[10];
      char humidity[10];
      dtostrfd(t, Settings.flag2.temperature_resolution, temperature);
      dtostrfd(h, Settings.flag2.humidity_resolution, humidity);

      if (json) {
        snprintf_P(mqtt_data, sizeof(mqtt_data), JSON_SNS_TEMPHUM, mqtt_data, "SHT3X", temperature, humidity);
#ifdef USE_DOMOTICZ
        DomoticzTempHumSensor(temperature, humidity);
#endif  // USE_DOMOTICZ
#ifdef USE_WEBSERVER
      } else {
        snprintf_P(mqtt_data, sizeof(mqtt_data), HTTP_SNS_TEMP, mqtt_data, "SHT3X", temperature, TempUnit());
        snprintf_P(mqtt_data, sizeof(mqtt_data), HTTP_SNS_HUM, mqtt_data, "SHT3X", humidity);
#endif  // USE_WEBSERVER
      }
    }
  }
}

/*********************************************************************************************\
 * Interface
\*********************************************************************************************/

#define XSNS_14

boolean Xsns14(byte function)
{
  boolean result = false;

  if (i2c_flg) {
    switch (function) {
      case FUNC_INIT:
        Sht3xDetect();
        break;
      case FUNC_JSON_APPEND:
        Sht3xShow(1);
        break;
#ifdef USE_WEBSERVER
      case FUNC_WEB_APPEND:
        Sht3xShow(0);
        break;
#endif  // USE_WEBSERVER
    }
  }
  return result;
}

#endif  // USE_SHT3X
#endif  // USE_I2C