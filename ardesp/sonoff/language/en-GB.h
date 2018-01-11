/*
  en-GB.h - localization for English - United Kingdom for Sonoff-Tasmota

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

#ifndef _LANGUAGE_EN_GB_H_
#define _LANGUAGE_EN_GB_H_

/*************************** ATTENTION *******************************\
 *
 * Due to memory constraints only UTF-8 is supported.
 * To save code space keep text as short as possible.
 * Time and Date provided by SDK can not be localized (yet).
 * Use online command StateText to translate ON, OFF, HOLD and TOGGLE.
 * Use online command Prefix to translate cmnd, stat and tele.
 *
\*********************************************************************/

// "2017-03-07T11:08:02" - ISO8601:2004
#define D_YEAR_MONTH_SEPARATOR "-"
#define D_MONTH_DAY_SEPARATOR "-"
#define D_DATE_TIME_SEPARATOR "T"
#define D_HOUR_MINUTE_SEPARATOR ":"
#define D_MINUTE_SECOND_SEPARATOR ":"

#define D_DAY3LIST "SunMonTueWedThuFriSat"
#define D_MONTH3LIST "JanFebMarAprMayJunJulAugSepOctNovDec"

// Non JSON decimal separator
#define D_DECIMAL_SEPARATOR "."

// Common
#define D_ADMIN "Admin"
#define D_AIR_QUALITY "Air quality"
#define D_AP "AP"                    // Access Point
#define D_AS "as"
#define D_AUTO "AUTO"
#define D_BLINK "Blink"
#define D_BLINKOFF "BlinkOff"
#define D_BOOT_COUNT "Boot Count"
#define D_BRIGHTLIGHT "Bright"
#define D_BUTTON "Button"
#define D_BY "by"                    // Written by me
#define D_BYTES "Bytes"
#define D_CELSIUS "Celsius"
#define D_CO2 "Carbon dioxide"
#define D_CODE "code"                // Button code
#define D_COLDLIGHT "Cold"
#define D_COMMAND "Command"
#define D_CONNECTED "Connected"
#define D_COUNT "Count"
#define D_COUNTER "Counter"
#define D_CURRENT "Current"          // As in Voltage and Current
#define D_DATA "Data"
#define D_DARKLIGHT "Dark"
#define D_DEBUG "Debug"
#define D_DISABLED "Disabled"
#define D_DNS_SERVER "DNS Server"
#define D_DONE "Done"
#define D_DST_TIME "DST"
#define D_EMULATION "Emulation"
#define D_ENABLED "Enabled"
#define D_ERASE "Erase"
#define D_ERROR "Error"
#define D_FAHRENHEIT "Fahrenheit"
#define D_FAILED "Failed"
#define D_FALLBACK "Fallback"
#define D_FALLBACK_TOPIC "Fallback Topic"
#define D_FALSE "False"
#define D_FILE "File"
#define D_FREE_MEMORY "Free Memory"
#define D_GAS "Gas"
#define D_GATEWAY "Gateway"
#define D_GROUP "Group"
#define D_HOST "Host"
#define D_HOSTNAME "Hostname"
#define D_HUMIDITY "Humidity"
#define D_ILLUMINANCE "Illuminance"
#define D_IMMEDIATE "immediate"      // Button immediate
#define D_INDEX "Index"
#define D_INFO "Info"
#define D_INITIALIZED "Initialized"
#define D_IP_ADDRESS "IP Address"
#define D_LIGHT "Light"
#define D_LWT "LWT"
#define D_MODULE "Module"
#define D_MQTT "MQTT"
#define D_MULTI_PRESS "multi-press"
#define D_NOISE "Noise"
#define D_NONE "None"
#define D_OFF "Off"
#define D_OFFLINE "Offline"
#define D_OK "Ok"
#define D_ON "On"
#define D_ONLINE "Online"
#define D_PASSWORD "Password"
#define D_PORT "Port"
#define D_POWER_FACTOR "Power Factor"
#define D_POWERUSAGE "Power"
#define D_PRESSURE "Pressure"
#define D_PRESSUREATSEALEVEL "SeaPressure"
#define D_PROGRAM_FLASH_SIZE "Program Flash Size"
#define D_PROGRAM_SIZE "Program Size"
#define D_PROJECT "Project"
#define D_RECEIVED "Received"
#define D_RESTART "Restart"
#define D_RESTARTING "Restarting"
#define D_RESTART_REASON "Restart Reason"
#define D_RESTORE "restore"
#define D_RETAINED "retained"
#define D_SAVE "Save"
#define D_SENSOR "Sensor"
#define D_SSID "SSId"
#define D_START "Start"
#define D_STD_TIME "STD"
#define D_STOP "Stop"
#define D_SUBNET_MASK "Subnet Mask"
#define D_SUBSCRIBE_TO "Subscribe to"
#define D_SUCCESSFUL "Successful"
#define D_TEMPERATURE "Temperature"
#define D_TO "to"
#define D_TOGGLE "Toggle"
#define D_TOPIC "Topic"
#define D_TRANSMIT "Transmit"
#define D_TRUE "True"
#define D_UPGRADE "upgrade"
#define D_UPLOAD "Upload"
#define D_UPTIME "Uptime"
#define D_USER "User"
#define D_UTC_TIME "UTC"
#define D_UV_LEVEL "UV Level"
#define D_VERSION "Version"
#define D_VOLTAGE "Voltage"
#define D_WARMLIGHT "Warm"
#define D_WEB_SERVER "Web Server"

// sonoff.ino
#define D_LEVEL_10 "level 1-0"
#define D_LEVEL_01 "level 0-1"
#define D_SERIAL_LOGGING_DISABLED "Serial logging disabled"
#define D_SYSLOG_LOGGING_REENABLED "Syslog logging re-enabled"

#define D_SET_BAUDRATE_TO "Set Baudrate to"
#define D_RECEIVED_TOPIC "Received Topic"
#define D_DATA_SIZE "Data Size"
#define D_ANALOG_INPUT "Analog"

#define D_FINGERPRINT "Verify TLS fingerprint..."
#define D_TLS_CONNECT_FAILED_TO "TLS Connect failed to"
#define D_RETRY_IN "Retry in"
#define D_VERIFIED "Verified"
#define D_INSECURE "Insecure connection due to invalid Fingerprint"
#define D_CONNECT_FAILED_TO "Connect failed to"

// support.ino
#define D_OSWATCH "osWatch"
#define D_BLOCKED_LOOP "Blocked Loop"
#define D_WPS_FAILED_WITH_STATUS "WPSconfig FAILED with status"
#define D_ACTIVE_FOR_3_MINUTES "active for 3 minutes"
#define D_FAILED_TO_START "failed to start"
#define D_PATCH_ISSUE_2186 "Patch issue 2186"
#define D_CONNECTING_TO_AP "Connecting to AP"
#define D_IN_MODE "in mode"
#define D_CONNECT_FAILED_NO_IP_ADDRESS "Connect failed as no IP address received"
#define D_CONNECT_FAILED_AP_NOT_REACHED "Connect failed as AP cannot be reached"
#define D_CONNECT_FAILED_WRONG_PASSWORD "Connect failed with AP incorrect password"
#define D_CONNECT_FAILED_AP_TIMEOUT "Connect failed with AP timeout"
#define D_ATTEMPTING_CONNECTION "Attempting connection..."
#define D_CHECKING_CONNECTION "Checking connection..."
#define D_QUERY_DONE "Query done. MQTT services found"
#define D_MQTT_SERVICE_FOUND "MQTT service found on"
#define D_FOUND_AT "found at"
#define D_SYSLOG_HOST_NOT_FOUND "Syslog Host not found"

// settings.ino
#define D_SAVED_TO_FLASH_AT "Saved to flash at"
#define D_LOADED_FROM_FLASH_AT "Loaded from flash at"
#define D_USE_DEFAULTS "Use defaults"
#define D_ERASED_SECTOR "Erased sector"

// webserver.ino
#define D_MINIMAL_FIRMWARE_PLEASE_UPGRADE "MINIMAL firmware - please upgrade"
#define D_WEBSERVER_ACTIVE_ON "Web server active on"
#define D_WITH_IP_ADDRESS "with IP address"
#define D_WEBSERVER_STOPPED "Web server stopped"
#define D_FILE_NOT_FOUND "File Not Found"
#define D_REDIRECTED "Redirected to captive portal"
#define D_WIFIMANAGER_SET_ACCESSPOINT_AND_STATION "Wifimanager set AccessPoint and keep Station"
#define D_WIFIMANAGER_SET_ACCESSPOINT "Wifimanager set AccessPoint"
#define D_TRYING_TO_CONNECT "Trying to connect device to network"

#define D_RESTART_IN "Restart in"
#define D_SECONDS "seconds"
#define D_DEVICE_WILL_RESTART "Device will restart in a few seconds"
#define D_BUTTON_TOGGLE "Toggle"
#define D_CONFIGURATION "Configuration"
#define D_INFORMATION "Information"
#define D_FIRMWARE_UPGRADE "Firmware Upgrade"
#define D_CONSOLE "Console"
#define D_CONFIRM_RESTART "Confirm Restart"

#define D_CONFIGURE_MODULE "Configure Module"
#define D_CONFIGURE_WIFI "Configure WiFi"
#define D_CONFIGURE_MQTT "Configure MQTT"
#define D_CONFIGURE_DOMOTICZ "Configure Domoticz"
#define D_CONFIGURE_LOGGING "Configure Logging"
#define D_CONFIGURE_OTHER "Configure Other"
#define D_CONFIRM_RESET_CONFIGURATION "Confirm Reset Configuration"
#define D_RESET_CONFIGURATION "Reset Configuration"
#define D_BACKUP_CONFIGURATION "Backup Configuration"
#define D_RESTORE_CONFIGURATION "Restore Configuration"
#define D_MAIN_MENU "Main Menu"

#define D_MODULE_PARAMETERS "Module parameters"
#define D_MODULE_TYPE "Module type"
#define D_GPIO "GPIO"
#define D_SERIAL_IN "Serial In"
#define D_SERIAL_OUT "Serial Out"

#define D_WIFI_PARAMETERS "Wifi parameters"
#define D_SCAN_FOR_WIFI_NETWORKS "Scan for wifi networks"
#define D_SCAN_DONE "Scan done"
#define D_NO_NETWORKS_FOUND "No networks found"
#define D_REFRESH_TO_SCAN_AGAIN "Refresh to scan again"
#define D_DUPLICATE_ACCESSPOINT "Duplicate AccessPoint"
#define D_SKIPPING_LOW_QUALITY "Skipping due to low quality"
#define D_RSSI "RSSI"
#define D_WEP "WEP"
#define D_WPA_PSK "WPA PSK"
#define D_WPA2_PSK "WPA2 PSK"
#define D_AP1_SSID "AP1 SSId"
#define D_AP1_PASSWORD "AP1 Password"
#define D_AP2_SSID "AP2 SSId"
#define D_AP2_PASSWORD "AP2 Password"

#define D_MQTT_PARAMETERS "MQTT parameters"
#define D_CLIENT "Client"
#define D_FULL_TOPIC "Full Topic"

#define D_LOGGING_PARAMETERS "Logging parameters"
#define D_SERIAL_LOG_LEVEL "Serial log level"
#define D_WEB_LOG_LEVEL "Web log level"
#define D_SYS_LOG_LEVEL "Syslog level"
#define D_MORE_DEBUG "More debug"
#define D_SYSLOG_HOST "Syslog host"
#define D_SYSLOG_PORT "Syslog port"
#define D_TELEMETRY_PERIOD "Telemetry period"

#define D_OTHER_PARAMETERS "Other parameters"
#define D_WEB_ADMIN_PASSWORD "Web Admin Password"
#define D_MQTT_ENABLE "MQTT enable"
#define D_FRIENDLY_NAME "Friendly Name"
#define D_BELKIN_WEMO "Belkin WeMo"
#define D_HUE_BRIDGE "Hue Bridge"
#define D_SINGLE_DEVICE "single device"
#define D_MULTI_DEVICE "multi device"

#define D_SAVE_CONFIGURATION "Save configuration"
#define D_CONFIGURATION_SAVED "Configuration saved"
#define D_CONFIGURATION_RESET "Configuration reset"

#define D_PROGRAM_VERSION "Program Version"
#define D_BUILD_DATE_AND_TIME "Build Date & Time"
#define D_CORE_AND_SDK_VERSION "Core/SDK Version"
#define D_FLASH_WRITE_COUNT "Flash write Count"
#define D_MAC_ADDRESS "MAC Address"
#define D_MQTT_HOST "MQTT Host"
#define D_MQTT_PORT "MQTT Port"
#define D_MQTT_CLIENT "MQTT Client"
#define D_MQTT_USER "MQTT User"
#define D_MQTT_TOPIC "MQTT Topic"
#define D_MQTT_GROUP_TOPIC "MQTT Group Topic"
#define D_MQTT_FULL_TOPIC "MQTT Full Topic"
#define D_MDNS_DISCOVERY "mDNS Discovery"
#define D_MDNS_ADVERTISE "mDNS Advertise"
#define D_ESP_CHIP_ID "ESP Chip Id"
#define D_FLASH_CHIP_ID "Flash Chip Id"
#define D_FLASH_CHIP_SIZE "Flash Size"
#define D_FREE_PROGRAM_SPACE "Free Program Space"

#define D_UPGRADE_BY_WEBSERVER "Upgrade by web server"
#define D_OTA_URL "OTA Url"
#define D_START_UPGRADE "Start upgrade"
#define D_UPGRADE_BY_FILE_UPLOAD "Upgrade by file upload"
#define D_UPLOAD_STARTED "Upload started"
#define D_UPGRADE_STARTED "Upgrade started"
#define D_UPLOAD_DONE "Upload done"
#define D_UPLOAD_ERR_1 "No file selected"
#define D_UPLOAD_ERR_2 "Not enough space"
#define D_UPLOAD_ERR_3 "Magic byte is not 0xE9"
#define D_UPLOAD_ERR_4 "Program flash size is larger than real flash size"
#define D_UPLOAD_ERR_5 "Upload buffer miscompare"
#define D_UPLOAD_ERR_6 "Upload failed. Enable logging 3"
#define D_UPLOAD_ERR_7 "Upload aborted"
#define D_UPLOAD_ERR_8 "File invalid"
#define D_UPLOAD_ERR_9 "File too large"
#define D_UPLOAD_ERROR_CODE "Upload error code"

#define D_ENTER_COMMAND "Enter command"
#define D_ENABLE_WEBLOG_FOR_RESPONSE "Enable weblog 2 if response expected"
#define D_NEED_USER_AND_PASSWORD "Need user=<username>&password=<password>"

// xdrv_wemohue.ino
#define D_MULTICAST_DISABLED "Multicast disabled"
#define D_MULTICAST_REJOINED "Multicast (re)joined"
#define D_MULTICAST_JOIN_FAILED "Multicast join failed"
#define D_FAILED_TO_SEND_RESPONSE "Failed to send response"

#define D_WEMO "WeMo"
#define D_WEMO_BASIC_EVENT "WeMo basic event"
#define D_WEMO_EVENT_SERVICE "WeMo event service"
#define D_WEMO_META_SERVICE "WeMo meta service"
#define D_WEMO_SETUP "WeMo setup"
#define D_RESPONSE_SENT "Response sent"

#define D_HUE "Hue"
#define D_HUE_BRIDGE_SETUP "Hue setup"
#define D_HUE_API_NOT_IMPLEMENTED "Hue API not implemented"
#define D_HUE_API "Hue API"
#define D_HUE_POST_ARGS "Hue POST args"
#define D_3_RESPONSE_PACKETS_SENT "3 response packets sent"

// xdrv_02_irremote.ino
#define D_JSON_INVALID_JSON "Invalid JSON"
#define D_JSON_PROTOCOL_NOT_SUPPORTED "Protocol not supported"
#define D_JSON_IR_PROTOCOL "PROTOCOL"
#define D_JSON_IR_BITS "BITS"
#define D_JSON_IR_DATA "DATA"
#define D_JSON_IRHVAC_VENDOR "VENDOR"
#define D_JSON_IRHVAC_POWER "POWER"
#define D_JSON_IRHVAC_MODE "MODE"
#define D_JSON_IRHVAC_FANSPEED "FANSPEED"
#define D_JSON_IRHVAC_TEMP "TEMP"
#define D_JSON_IRRECEIVED "IrReceived"

// xdrv_05_domoticz.ino
#define D_DOMOTICZ_PARAMETERS "Domoticz parameters"
#define D_DOMOTICZ_IDX "Idx"
#define D_DOMOTICZ_KEY_IDX "Key idx"
#define D_DOMOTICZ_SWITCH_IDX "Switch idx"
#define D_DOMOTICZ_SENSOR_IDX "Sensor idx"
  #define D_DOMOTICZ_TEMP "Temp"
  #define D_DOMOTICZ_TEMP_HUM "Temp,Hum"
  #define D_DOMOTICZ_TEMP_HUM_BARO "Temp,Hum,Baro"
  #define D_DOMOTICZ_POWER_ENERGY "Power,Energy"
  #define D_DOMOTICZ_ILLUMINANCE "Illuminance"
  #define D_DOMOTICZ_COUNT "Count"
  #define D_DOMOTICZ_VOLTAGE "Voltage"
  #define D_DOMOTICZ_CURRENT "Current"
  #define D_DOMOTICZ_AIRQUALITY "AirQuality"
#define D_DOMOTICZ_UPDATE_TIMER "Update timer"

// xsns_03_energy.ino
#define D_ENERGY_TODAY "Energy Today"
#define D_ENERGY_YESTERDAY "Energy Yesterday"
#define D_ENERGY_TOTAL "Energy Total"

// xsns_05_ds18b20.ino
#define D_SENSOR_BUSY "Sensor busy"
#define D_SENSOR_CRC_ERROR "Sensor CRC error"
#define D_SENSORS_FOUND "Sensors found"

// xsns_06_dht.ino
#define D_TIMEOUT_WAITING_FOR "Timeout waiting for"
#define D_START_SIGNAL_LOW "start signal low"
#define D_START_SIGNAL_HIGH "start signal high"
#define D_PULSE "pulse"
#define D_CHECKSUM_FAILURE "Checksum failure"

// xsns_07_sht1x.ino
#define D_SENSOR_DID_NOT_ACK_COMMAND "Sensor did not ACK command"
#define D_SHT1X_FOUND "SHT1X found"

// sonoff_template.h
// Max string length is 8 characters including suffixes
#define D_SENSOR_NONE     "None"
#define D_SENSOR_DHT11    "DHT11"
#define D_SENSOR_AM2301   "AM2301"
#define D_SENSOR_SI7021   "SI7021"
#define D_SENSOR_DS18X20  "DS18x20"
#define D_SENSOR_I2C_SCL  "I2C SCL"
#define D_SENSOR_I2C_SDA  "I2C SDA"
#define D_SENSOR_WS2812   "WS2812"
#define D_SENSOR_IRSEND   "IRsend"
#define D_SENSOR_SWITCH   "Switch"   // Suffix "1"
#define D_SENSOR_BUTTON   "Button"   // Suffix "1"
#define D_SENSOR_RELAY    "Relay"    // Suffix "1i"
#define D_SENSOR_LED      "Led"      // Suffix "1i"
#define D_SENSOR_PWM      "PWM"      // Suffix "1"
#define D_SENSOR_COUNTER  "Counter"  // Suffix "1"
#define D_SENSOR_IRRECV   "IRrecv"
#define D_SENSOR_MHZ_RX   "MHZ Rx"
#define D_SENSOR_MHZ_TX   "MHZ Tx"
#define D_SENSOR_PZEM_RX  "PZEM Rx"
#define D_SENSOR_PZEM_TX  "PZEM Tx"
#define D_SENSOR_SAIR_RX  "SAir Rx"
#define D_SENSOR_SAIR_TX  "SAir Tx"
#define D_SENSOR_SPI_CS   "SPI CS"
#define D_SENSOR_SPI_DC   "SPI DC"
#define D_SENSOR_BACKLIGHT "BkLight"

// Units
#define D_UNIT_AMPERE "A"
#define D_UNIT_HOUR "Hr"
#define D_UNIT_KILOOHM "kOhm"
#define D_UNIT_KILOWATTHOUR "kWh"
#define D_UNIT_LUX "lx"
#define D_UNIT_MICROSECOND "us"
#define D_UNIT_MILLIAMPERE "mA"
#define D_UNIT_MILLISECOND "ms"
#define D_UNIT_MINUTE "Min"
#define D_UNIT_PPM "ppm"
#define D_UNIT_PRESSURE "hPa"
#define D_UNIT_SECOND "sec"
#define D_UNIT_SECTORS "sectors"
#define D_UNIT_VOLT "V"
#define D_UNIT_WATT "W"
#define D_UNIT_WATTHOUR "Wh"

// Log message prefix
#define D_LOG_APPLICATION "APP: "  // Application
#define D_LOG_BRIDGE "BRG: "       // Bridge
#define D_LOG_CONFIG "CFG: "       // Settings
#define D_LOG_COMMAND "CMD: "      // Command
#define D_LOG_DEBUG "DBG: "        // Debug
#define D_LOG_DHT "DHT: "          // DHT sensor
#define D_LOG_DOMOTICZ "DOM: "     // Domoticz
#define D_LOG_DSB "DSB: "          // DS18xB20 sensor
#define D_LOG_HTTP "HTP: "         // HTTP webserver
#define D_LOG_I2C "I2C: "          // I2C
#define D_LOG_IRR "IRR: "          // Infra Red Received
#define D_LOG_LOG "LOG: "          // Logging
#define D_LOG_MODULE "MOD: "       // Module
#define D_LOG_MDNS "DNS: "         // mDNS
#define D_LOG_MQTT "MQT: "         // MQTT
#define D_LOG_OTHER "OTH: "        // Other
#define D_LOG_RESULT "RSL: "       // Result
#define D_LOG_RFR "RFR: "          // RF Received
#define D_LOG_SERIAL "SER: "       // Serial
#define D_LOG_SHT1 "SHT: "         // SHT1x sensor
#define D_LOG_UPLOAD "UPL: "       // Upload
#define D_LOG_UPNP "UPP: "         // UPnP
#define D_LOG_WIFI "WIF: "         // Wifi

#endif  // _LANGUAGE_EN_GB_H_
