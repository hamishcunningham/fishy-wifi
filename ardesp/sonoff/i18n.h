/*
  i18n.h - internationalization for Sonoff-Tasmota

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

#ifndef _I18N_H_
#define _I18N_H_

/*********************************************************************************************\
 * All text used in Commands, MQTT, JSON messages only in English (#1473)
 *
 * (At this location they can/may be overruled by language files using undefs)
\*********************************************************************************************/

#define D_JSON_ABORTED "Aborted"
#define D_JSON_ACTIVE "Active"
#define D_JSON_ADDRESS "Address"
#define D_JSON_AIRQUALITY "AirQuality"
#define D_JSON_ANALOG_INPUT "Analog"
#define D_JSON_AP "AP"                   // Access Point
#define D_JSON_APMAC_ADDRESS "APMac"
#define D_JSON_APPENDED "Appended"
#define D_JSON_BAUDRATE "Baudrate"
#define D_JSON_BLINK "Blink"
#define D_JSON_BLOCKED_LOOP "Blocked Loop"
#define D_JSON_BOOTVERSION "Boot"
#define D_JSON_BOOTCOUNT "BootCount"
#define D_JSON_BUILDDATETIME "BuildDateTime"
#define D_JSON_CO2 "CarbonDioxide"
#define D_JSON_COMMAND "Command"
#define D_JSON_COREVERSION "Core"
#define D_JSON_COUNTER "Counter"
#define D_JSON_CURRENT "Current"         // As in Voltage and Current
#define D_JSON_DATA "Data"
#define D_JSON_DNSSERVER "DNSServer"
#define D_JSON_DONE "Done"
#define D_JSON_EMPTY "Empty"
#define D_JSON_ENDDST "EndDST"           // End Daylight Savings Time
#define D_JSON_ERASE "Erase"
#define D_JSON_ERROR "Error"
#define D_JSON_EVERY "Every"
#define D_JSON_FAILED "Failed"
#define D_JSON_FALLBACKTOPIC "FallbackTopic"
#define D_JSON_FLASHMODE "FlashMode"
#define D_JSON_FLASHSIZE "FlashSize"
#define D_JSON_FREEMEMORY "Free"
#define D_JSON_FROM "from"
#define D_JSON_GAS "Gas"
#define D_JSON_GATEWAY "Gateway"
#define D_JSON_HEAPSIZE "Heap"
#define D_JSON_HIGH "High"
#define D_JSON_HUMIDITY "Humidity"
#define D_JSON_I2CSCAN_DEVICES_FOUND_AT "Device(s) found at"
#define D_JSON_I2CSCAN_UNKNOWN_ERROR_AT "Unknown error at"
#define D_JSON_I2CSCAN_NO_DEVICES_FOUND "No devices found"
#define D_JSON_ID "Id"
#define D_JSON_ILLUMINANCE "Illuminance"
#define D_JSON_UNKNOWN "Unknown"
#define D_JSON_LIGHT "Light"
#define D_JSON_LOCAL_TIME "Local"
#define D_JSON_LOW "Low"
#define D_JSON_MAC "Mac"
#define D_JSON_MASK "Mask"
#define D_JSON_NO "No"
#define D_JSON_NOISE "Noise"
#define D_JSON_NONE "None"
#define D_JSON_OR "or"
#define D_JSON_PERIOD "Period"
#define D_JSON_POWERFACTOR "Factor"
#define D_JSON_POWERUSAGE "Power"
#define D_JSON_PRESSURE "Pressure"
#define D_JSON_PRESSUREATSEALEVEL "SeaPressure"
#define D_JSON_PROGRAMFLASHSIZE "ProgramFlashSize"
#define D_JSON_PROGRAMSIZE "ProgramSize"
#define D_JSON_RESTARTING "Restarting"
#define D_JSON_RESTARTREASON "RestartReason"
#define D_JSON_RSSI "RSSI"
#define D_JSON_SAVEADDRESS "SaveAddress"
#define D_JSON_SAVECOUNT "SaveCount"
#define D_JSON_SAVED "Saved"
#define D_JSON_SAVESTATE "SaveState"
#define D_JSON_SDKVERSION "SDK"
#define D_JSON_SELECTED "selected"
#define D_JSON_SSID "SSId"
#define D_JSON_STARTDST "StartDST"       // Start Daylight Savings Time
#define D_JSON_STARTED "Started"
#define D_JSON_SUBNETMASK "Subnetmask"
#define D_JSON_SUCCESSFUL "Successful"
#define D_JSON_SWITCH "Switch"
#define D_JSON_SYNC "Sync"
#define D_JSON_TEMPERATURE "Temperature"
#define D_JSON_TEMPERATURE_UNIT "TempUnit"
#define D_JSON_TIME "Time"
#define D_JSON_TODAY "Today"
#define D_JSON_TOTAL "Total"
#define D_JSON_TYPE "Type"
#define D_JSON_UPTIME "Uptime"
#define D_JSON_UTC_TIME "UTC"
#define D_JSON_UV_LEVEL "UvLevel"
#define D_JSON_VCC "Vcc"
#define D_JSON_VERSION "Version"
#define D_JSON_VOLTAGE "Voltage"
#define D_JSON_WIFI "Wifi"
#define D_JSON_WRONG "Wrong"
#define D_JSON_YESTERDAY "Yesterday"

#define D_RSLT_ENERGY "ENERGY"
#define D_RSLT_INFO "INFO"
#define D_RSLT_MARGINS "MARGINS"
#define D_RSLT_POWER "POWER"
#define D_RSLT_RESULT "RESULT"
#define D_RSLT_SENSOR "SENSOR"
#define D_RSLT_STATE "STATE"
#define D_RSLT_UPTIME "UPTIME"
#define D_RSLT_WARNING "WARNING"

// Commands sonoff.ino
#define D_CMND_MQTTHOST "MqttHost"
#define D_CMND_MQTTPORT "MqttPort"
#define D_CMND_MQTTRETRY "MqttRetry"
#define D_CMND_STATETEXT "StateText"
#define D_CMND_MQTTFINGERPRINT "MqttFingerprint"
#define D_CMND_MQTTCLIENT "MqttClient"
#define D_CMND_MQTTUSER "MqttUser"
#define D_CMND_MQTTPASSWORD "MqttPassword"
#define D_CMND_FULLTOPIC "FullTopic"
#define D_CMND_PREFIX "Prefix"
  #define PRFX_MAX_STRING_LENGTH 5
  #define D_CMND "cmnd"
  #define D_STAT "stat"
  #define D_TELE "tele"
#define D_CMND_GROUPTOPIC "GroupTopic"
#define D_CMND_TOPIC "Topic"
#define D_CMND_BUTTONTOPIC "ButtonTopic"
#define D_CMND_SWITCHTOPIC "SwitchTopic"
#define D_CMND_BUTTONRETAIN "ButtonRetain"
#define D_CMND_SWITCHRETAIN "SwitchRetain"
#define D_CMND_POWERRETAIN "PowerRetain"
#define D_CMND_SENSORRETAIN "SensorRetain"
#define D_CMND_BACKLOG "Backlog"
#define D_CMND_DELAY "Delay"
#define D_CMND_STATUS "Status"
  #define D_STATUS1_PARAMETER "PRM"
  #define D_STATUS2_FIRMWARE "FWR"
  #define D_STATUS3_LOGGING "LOG"
  #define D_STATUS4_MEMORY "MEM"
  #define D_STATUS5_NETWORK "NET"
  #define D_STATUS6_MQTT "MQT"
  #define D_STATUS7_TIME "TIM"
  #define D_STATUS8_POWER "PWR"
  #define D_STATUS9_MARGIN "PTH"
  #define D_STATUS10_SENSOR "SNS"
  #define D_STATUS11_STATUS "STS"
#define D_CMND_POWER "Power"
#define D_CMND_POWERONSTATE "PowerOnState"
#define D_CMND_PULSETIME "PulseTime"
#define D_CMND_BLINKTIME "BlinkTime"
#define D_CMND_BLINKCOUNT "BlinkCount"
#define D_CMND_SAVEDATA "SaveData"
#define D_CMND_SETOPTION "SetOption"
#define D_CMND_TEMPERATURE_RESOLUTION "TempRes"
#define D_CMND_HUMIDITY_RESOLUTION "HumRes"
#define D_CMND_PRESSURE_RESOLUTION "PressRes"
#define D_CMND_POWER_RESOLUTION "WattRes"
#define D_CMND_VOLTAGE_RESOLUTION "VoltRes"
#define D_CMND_CURRENT_RESOLUTION "AmpRes"
#define D_CMND_ENERGY_RESOLUTION "EnergyRes"
#define D_CMND_MODULE "Module"
#define D_CMND_MODULES "Modules"
#define D_CMND_GPIO "GPIO"
  #define D_JSON_NOT_SUPPORTED "Not supported"
#define D_CMND_GPIOS "GPIOs"
#define D_CMND_PWM "PWM"
#define D_CMND_PWMFREQUENCY "PWMFrequency"
#define D_CMND_PWMRANGE "PWMRange"
#define D_CMND_COUNTER "Counter"
#define D_CMND_COUNTERTYPE "CounterType"
#define D_CMND_COUNTERDEBOUNCE "CounterDebounce"
#define D_CMND_SLEEP "Sleep"
#define D_CMND_UPLOAD "Upload"
#define D_CMND_UPGRADE "Upgrade"
  #define D_JSON_ONE_OR_GT "1 or >%s to upgrade"
#define D_CMND_OTAURL "OtaUrl"
#define D_CMND_SERIALLOG "SerialLog"
#define D_CMND_SYSLOG "SysLog"
#define D_CMND_LOGHOST "LogHost"
#define D_CMND_LOGPORT "LogPort"
#define D_CMND_IPADDRESS "IPAddress"
#define D_CMND_NTPSERVER "NtpServer"
#define D_CMND_AP "Ap"
#define D_CMND_SSID "SSId"
#define D_CMND_PASSWORD "Password"
#define D_CMND_HOSTNAME "Hostname"
#define D_CMND_WIFICONFIG "WifiConfig"
  #define WCFG_MAX_STRING_LENGTH 12
  #define D_WCFG_0_RESTART "Restart"
  #define D_WCFG_1_SMARTCONFIG "SmartConfig"
  #define D_WCFG_2_WIFIMANAGER "WifiManager"
  #define D_WCFG_3_WPSCONFIG "WPSConfig"
  #define D_WCFG_4_RETRY "Retry"
  #define D_WCFG_5_WAIT "Wait"
#define D_CMND_FRIENDLYNAME "FriendlyName"
#define D_CMND_SWITCHMODE "SwitchMode"
#define D_CMND_WEBSERVER "Webserver"
  #define D_JSON_WEBSERVER_MODE "WebServerMode"
  #define D_JSON_ACTIVE_FOR "Active for"
  #define D_JSON_ON_DEVICE "on"
  #define D_JSON_WITH_IP_ADDRESS "with IP address"
#define D_CMND_WEBPASSWORD "WebPassword"
#define D_CMND_WEBLOG "WebLog"
#define D_CMND_EMULATION "Emulation"
#define D_CMND_TELEPERIOD "TelePeriod"
#define D_CMND_RESTART "Restart"
  #define D_JSON_ONE_TO_RESTART "1 to restart"
#define D_CMND_RESET "Reset"
  #define D_JSON_RESET_AND_RESTARTING "Reset and Restarting"
  #define D_JSON_ONE_TO_RESET "1 to reset"
#define D_CMND_TIMEZONE "Timezone"
#define D_CMND_ALTITUDE "Altitude"
#define D_CMND_LEDPOWER "LedPower"
#define D_CMND_LEDSTATE "LedState"
#define D_CMND_CFGDUMP "CfgDump"
#define D_CMND_I2CSCAN "I2CScan"
#define D_CMND_INA219MODE "Ina219Mode"
#define D_CMND_EXCEPTION "Exception"

// Commands xdrv_01_light.ino
#define D_CMND_COLOR "Color"
#define D_CMND_COLORTEMPERATURE "CT"
#define D_CMND_DIMMER "Dimmer"
#define D_CMND_LED "Led"
#define D_CMND_LEDTABLE "LedTable"
#define D_CMND_FADE "Fade"
#define D_CMND_PIXELS "Pixels"
#define D_CMND_SCHEME "Scheme"
#define D_CMND_SPEED "Speed"
#define D_CMND_WAKEUP "Wakeup"
#define D_CMND_WAKEUPDURATION "WakeUpDuration"
#define D_CMND_WIDTH "Width"

// Commands xdrv_02_irremote.ino
#define D_CMND_IRSEND "IRSend"
#define D_CMND_IRHVAC "IRHVAC"

// Commands xsns_03_energy.ino
#define D_CMND_POWERLOW "PowerLow"
#define D_CMND_POWERHIGH "PowerHigh"
#define D_CMND_VOLTAGELOW "VoltageLow"
#define D_CMND_VOLTAGEHIGH "VoltageHigh"
#define D_CMND_CURRENTLOW "CurrentLow"
#define D_CMND_CURRENTHIGH "CurrentHigh"
#define D_CMND_ENERGYRESET "EnergyReset"
#define D_CMND_HLWPCAL "HlwPcal"
#define D_CMND_HLWPSET "HlwPset"
#define D_CMND_HLWUCAL "HlwUcal"
#define D_CMND_HLWUSET "HlwUset"
#define D_CMND_HLWICAL "HlwIcal"
#define D_CMND_HLWISET "HlwIset"
#define D_CMND_MAXPOWER "MaxPower"
#define D_CMND_MAXPOWERHOLD "MaxPowerHold"
#define D_CMND_MAXPOWERWINDOW "MaxPowerWindow"
  #define D_JSON_MAXPOWERREACHED "MaxPowerReached"
  #define D_JSON_MAXPOWERREACHEDRETRY "MaxPowerReachedRetry"
#define D_CMND_SAFEPOWER "SafePower"
#define D_CMND_SAFEPOWERHOLD "SafePowerHold"
#define D_CMND_SAFEPOWERWINDOW "SafePowerWindow"
  #define D_JSON_POWERMONITOR "PowerMonitor"
#define D_CMND_MAXENERGY "MaxEnergy"
#define D_CMND_MAXENERGYSTART "MaxEnergyStart"
  #define D_JSON_ENERGYMONITOR "EnergyMonitor"
  #define D_JSON_MAXENERGYREACHED "MaxEnergyReached"

// Commands xdrv_04_snfbridge.ino
#define D_CMND_RFCODE "RfCode"
#define D_CMND_RFHIGH "RfHigh"
#define D_CMND_RFHOST "RfHost"
#define D_CMND_RFKEY "RfKey"
  #define D_JSON_START_LEARNING "Start learning"
  #define D_JSON_SET_TO_DEFAULT "Set to default"
  #define D_JSON_DEFAULT_SENT "Default sent"
  #define D_JSON_LEARNED_SENT "Learned sent"
  #define D_JSON_LEARNING_ACTIVE "Learning active"
  #define D_JSON_LEARN_FAILED "Learn failed"
  #define D_JSON_LEARNED "Learned"
#define D_CMND_RFLOW "RfLow"
#define D_CMND_RFSYNC "RfSync"
  #define D_JSON_RFRECEIVED "RfReceived"

// Commands xdrv_05_domoticz.ino
#define D_CMND_DOMOTICZ "Domoticz"
#define D_CMND_IDX "Idx"
#define D_CMND_KEYIDX "KeyIdx"
#define D_CMND_SWITCHIDX "SwitchIdx"
#define D_CMND_SENSORIDX "SensorIdx"
#define D_CMND_UPDATETIMER "UpdateTimer"

// Commands xdrv_06_display.ino
#define D_CMND_DISPLAY "Display"
#define D_CMND_DISP_ADDRESS "Address"
#define D_CMND_DISP_COLS "Cols"
#define D_CMND_DISP_DIMMER "Dimmer"
#define D_CMND_DISP_MODE "Mode"
#define D_CMND_DISP_MODEL "Model"
#define D_CMND_DISP_REFRESH "Refresh"
#define D_CMND_DISP_ROWS "Rows"
#define D_CMND_DISP_SIZE "Size"
#define D_CMND_DISP_TEXT "Text"

/********************************************************************************************/

#ifndef MY_LANGUAGE
  #include "language/en-GB.h"
#else
  #define QUOTEME(x) QUOTEME_1(x)
  #define QUOTEME_1(x) #x
  #define INCLUDE_FILE(x) QUOTEME(language/x.h)
  #include INCLUDE_FILE(MY_LANGUAGE)
#endif

// Common
enum UnitNames {
  UNIT_AMPERE,
  UNIT_HOUR,
  UNIT_KILOOHM,
  UNIT_KILOWATTHOUR,
  UNIT_LUX,
  UNIT_MICROSECOND,
  UNIT_MILLIAMPERE,
  UNIT_MILLISECOND,
  UNIT_MINUTE,
  UNIT_PPM,
  UNIT_PRESSURE,
  UNIT_SECOND,
  UNIT_SECTORS,
  UNIT_VOLT,
  UNIT_WATT,
  UNIT_WATTHOUR };
const char kUnitNames[] PROGMEM =
  D_UNIT_AMPERE "|"
  D_UNIT_HOUR "|"
  D_UNIT_KILOOHM "|"
  D_UNIT_KILOWATTHOUR "|"
  D_UNIT_LUX "|"
  D_UNIT_MICROSECOND "|"
  D_UNIT_MILLIAMPERE "|"
  D_UNIT_MILLISECOND "|"
  D_UNIT_MINUTE "|"
  D_UNIT_PPM "|"
  D_UNIT_PRESSURE "|"
  D_UNIT_SECOND "|"
  D_UNIT_SECTORS "|"
  D_UNIT_VOLT "|"
  D_UNIT_WATT "|"
  D_UNIT_WATTHOUR ;

const char S_JSON_COMMAND_NVALUE_SPACE_UNIT[] PROGMEM =       "{\"%s\":\"%d %s\"}";
const char S_JSON_COMMAND_SVALUE_SPACE_UNIT[] PROGMEM =       "{\"%s\":\"%s %s\"}";
const char S_JSON_COMMAND_NVALUE_UNIT[] PROGMEM =             "{\"%s\":\"%d%s\"}";
const char S_JSON_COMMAND_NVALUE_UNIT_NVALUE_UNIT[] PROGMEM = "{\"%s\":\"%d%s (%d%s)\"}";

const char S_JSON_COMMAND_NVALUE_SVALUE[] PROGMEM =           "{\"%s\":\"%d (%s)\"}";
const char S_JSON_COMMAND_NVALUE_ACTIVE_NVALUE[] PROGMEM =    "{\"%s\":\"%d (" D_JSON_ACTIVE " %d)\"}";

const char S_JSON_COMMAND_NVALUE[] PROGMEM =                  "{\"%s\":%d}";
const char S_JSON_COMMAND_SVALUE[] PROGMEM =                  "{\"%s\":\"%s\"}";
const char S_JSON_COMMAND_XVALUE[] PROGMEM =                  "{\"%s\":%s}";  // %s must provide quotes on non-number

const char S_JSON_COMMAND_INDEX_NVALUE[] PROGMEM =            "{\"%s%d\":%d}";
const char S_JSON_COMMAND_INDEX_SVALUE[] PROGMEM =            "{\"%s%d\":\"%s\"}";
const char S_JSON_COMMAND_INDEX_SVALUE_SVALUE[] PROGMEM =     "{\"%s%d\":\"%s%s\"}";

const char JSON_SNS_TEMPHUM[] PROGMEM = "%s,\"%s\":{\"" D_JSON_TEMPERATURE "\":%s,\"" D_JSON_HUMIDITY "\":%s}";

const char S_LOG_I2C_FOUND_AT[] PROGMEM = D_LOG_I2C "%s " D_FOUND_AT " 0x%x";

const char S_LOG_HTTP[] PROGMEM = D_LOG_HTTP;
const char S_LOG_WIFI[] PROGMEM = D_LOG_WIFI;
const char S_LOG_MQTT[] PROGMEM = D_LOG_MQTT;
const char S_RSLT_POWER[] PROGMEM = D_RSLT_POWER;
const char S_RSLT_RESULT[] PROGMEM = D_RSLT_RESULT;
const char S_RSLT_WARNING[] PROGMEM = D_RSLT_WARNING;
const char S_LWT[] PROGMEM = D_LWT;
const char S_OFFLINE[] PROGMEM = D_OFFLINE;

// sonoff.ino
#define MAX_BUTTON_COMMANDS  5  // Max number of button commands supported
const char kCommands[MAX_BUTTON_COMMANDS][14] PROGMEM = {
  D_CMND_WIFICONFIG " 1",   // Press button three times
  D_CMND_WIFICONFIG " 2",   // Press button four times
  D_CMND_WIFICONFIG " 3",   // Press button five times
  D_CMND_RESTART " 1",      // Press button six times
  D_CMND_UPGRADE " 1" };    // Press button seven times
const char kWifiConfig[MAX_WIFI_OPTION][WCFG_MAX_STRING_LENGTH] PROGMEM = {
  D_WCFG_0_RESTART,
  D_WCFG_1_SMARTCONFIG,
  D_WCFG_2_WIFIMANAGER,
  D_WCFG_3_WPSCONFIG,
  D_WCFG_4_RETRY,
  D_WCFG_5_WAIT };
const char kPrefixes[3][PRFX_MAX_STRING_LENGTH] PROGMEM = {
  D_CMND,
  D_STAT,
  D_TELE };

// support.ino
static const char kMonthNames[] = D_MONTH3LIST;

// webserver.ino
#ifdef USE_WEBSERVER
const char HTTP_SNS_TEMP[] PROGMEM = "%s{s}%s " D_TEMPERATURE "{m}%s&deg;%c{e}";                             // {s} = <tr><th>, {m} = </th><td>, {e} = </td></tr>
const char HTTP_SNS_HUM[] PROGMEM = "%s{s}%s " D_HUMIDITY "{m}%s%{e}";                                       // {s} = <tr><th>, {m} = </th><td>, {e} = </td></tr>
const char HTTP_SNS_PRESSURE[] PROGMEM = "%s{s}%s " D_PRESSURE "{m}%s " D_UNIT_PRESSURE "{e}";               // {s} = <tr><th>, {m} = </th><td>, {e} = </td></tr>
const char HTTP_SNS_SEAPRESSURE[] PROGMEM = "%s{s}%s " D_PRESSUREATSEALEVEL "{m}%s " D_UNIT_PRESSURE "{e}";  // {s} = <tr><th>, {m} = </th><td>, {e} = </td></tr>
const char HTTP_SNS_ANALOG[] PROGMEM = "%s{s}%s " D_ANALOG_INPUT "%d{m}%d{e}";                               // {s} = <tr><th>, {m} = </th><td>, {e} = </td></tr>

#if defined(USE_MHZ19) || defined(USE_SENSEAIR)
const char HTTP_SNS_CO2[] PROGMEM = "%s{s}%s " D_CO2 "{m}%d " D_UNIT_PPM "{e}";                              // {s} = <tr><th>, {m} = </th><td>, {e} = </td></tr>
#endif  // USE_WEBSERVER

const char S_MAIN_MENU[] PROGMEM = D_MAIN_MENU;
const char S_CONFIGURATION[] PROGMEM = D_CONFIGURATION;
const char S_CONFIGURE_MODULE[] PROGMEM = D_CONFIGURE_MODULE;
const char S_CONFIGURE_WIFI[] PROGMEM = D_CONFIGURE_WIFI;
const char S_NO_NETWORKS_FOUND[] PROGMEM = D_NO_NETWORKS_FOUND;
const char S_CONFIGURE_MQTT[] PROGMEM = D_CONFIGURE_MQTT;
const char S_CONFIGURE_LOGGING[] PROGMEM = D_CONFIGURE_LOGGING;
const char S_CONFIGURE_OTHER[] PROGMEM = D_CONFIGURE_OTHER;
const char S_SAVE_CONFIGURATION[] PROGMEM = D_SAVE_CONFIGURATION;
const char S_RESET_CONFIGURATION[] PROGMEM = D_RESET_CONFIGURATION;
const char S_RESTORE_CONFIGURATION[] PROGMEM = D_RESTORE_CONFIGURATION;
const char S_FIRMWARE_UPGRADE[] PROGMEM = D_FIRMWARE_UPGRADE;
const char S_CONSOLE[] PROGMEM = D_CONSOLE;
const char S_INFORMATION[] PROGMEM = D_INFORMATION;
const char S_RESTART[] PROGMEM = D_RESTART;
#endif  // USE_WEBSERVER

#endif  // _I18N_H_