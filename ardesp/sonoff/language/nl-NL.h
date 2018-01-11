/*
  nl-NL.h - localization for Dutch - Nederland for Sonoff-Tasmota

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

#ifndef _LANGUAGE_NL_NL_H_
#define _LANGUAGE_NL_NL_H_

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

#define D_DAY3LIST "Zo Ma Di Wo Do Vr Za "
#define D_MONTH3LIST "JanFebMarAprMeiJunJulAugSepOktNovDec"

// Non JSON decimal separator
#define D_DECIMAL_SEPARATOR ","

// Common
#define D_ADMIN "Admin"
#define D_AIR_QUALITY "Lucht kwalitiet"
#define D_AP "AP"                    // Access Point
#define D_AS "als"
#define D_AUTO "AUTO"
#define D_BLINK "Knipper"
#define D_BLINKOFF "KnipperUit"
#define D_BOOT_COUNT "Herstarts"
#define D_BRIGHTLIGHT "Fel"
#define D_BUTTON "DrukKnop"
#define D_BY "door"                  // Written by me
#define D_BYTES "Bytes"
#define D_CELSIUS "Celsius"
#define D_CO2 "Koolstofdioxide"
#define D_CODE "code"                // Button code
#define D_COLDLIGHT "Koud"
#define D_COMMAND "Opdracht"
#define D_CONNECTED "Verbonden"
#define D_COUNT "Aantal"
#define D_COUNTER "Teller"
#define D_CURRENT "Stroom"          // As in Voltage and Current
#define D_DATA "Data"
#define D_DARKLIGHT "Donker"
#define D_DEBUG "Debug"
#define D_DISABLED "Uitgeschakeld"
#define D_DNS_SERVER "DNS Server"
#define D_DONE "Klaar"
#define D_DST_TIME "ZT"
#define D_EMULATION "Emulatie"
#define D_ENABLED "Geactiveerd"
#define D_ERASE "Wissen"
#define D_ERROR "Fout"
#define D_FAHRENHEIT "Fahrenheit"
#define D_FAILED "Mislukt"
#define D_FALLBACK "Fallback"
#define D_FALLBACK_TOPIC "Fallback Topic"
#define D_FALSE "Onwaar"
#define D_FILE "Bestand"
#define D_FREE_MEMORY "Vrij geheugen"
#define D_GAS "Gas"
#define D_GATEWAY "Gateway"
#define D_GROUP "Groep"
#define D_HOST "Host"
#define D_HOSTNAME "Hostnaam"
#define D_HUMIDITY "Luchtvochtigheid"
#define D_ILLUMINANCE "Verlichtingssterkte"
#define D_IMMEDIATE "onmiddelijk"      // Button immediate
#define D_INDEX "Index"
#define D_INFO "Info"
#define D_INITIALIZED "Geinitialiseerd"
#define D_IP_ADDRESS "IP Adres"
#define D_LIGHT "Licht"
#define D_LWT "LWT"
#define D_MODULE "Module"
#define D_MQTT "MQTT"
#define D_MULTI_PRESS "meervoudig"
#define D_NOISE "Lawaai"
#define D_NONE "Geen"
#define D_OFF "Uit"
#define D_OFFLINE "Offline"
#define D_OK "Ok"
#define D_ON "Ann"
#define D_ONLINE "Online"
#define D_PASSWORD "Wachtwoord"
#define D_PORT "Poort"
#define D_POWER_FACTOR "Arbeidsfactor"
#define D_POWERUSAGE "Vermogen"
#define D_PRESSURE "Luchtdruk"
#define D_PRESSUREATSEALEVEL "ZeeLuchtdruk"
#define D_PROGRAM_FLASH_SIZE "Programma Flash Grootte"
#define D_PROGRAM_SIZE "Programma Grootte"
#define D_PROJECT "Project"
#define D_RECEIVED "Ontvangen"
#define D_RESTART "Herstart"
#define D_RESTARTING "Herstarten"
#define D_RESTART_REASON "Reden herstart"
#define D_RESTORE "herstellen"
#define D_RETAINED "retained"
#define D_SAVE "Opslaan"
#define D_SENSOR "Sensor"
#define D_SSID "SSId"
#define D_START "Start"
#define D_STD_TIME "WT"
#define D_STOP "Stop"
#define D_SUBNET_MASK "Subnet Masker"
#define D_SUBSCRIBE_TO "Abonneer op"
#define D_SUCCESSFUL "Gelukt"
#define D_TEMPERATURE "Temperatuur"
#define D_TO "naar"
#define D_TOGGLE "Toggle"  // Wissel, Tuimel
#define D_TOPIC "Topic"  // Onderwerp
#define D_TRANSMIT "Verzend"
#define D_TRUE "Waar"
#define D_UPGRADE "opwaarderen"
#define D_UPLOAD "Verzenden"
#define D_UPTIME "Bedrijfstijd"
#define D_USER "Gebruiker"
#define D_UTC_TIME "UTC"
#define D_UV_LEVEL "UV niveau"
#define D_VERSION "Versie"
#define D_VOLTAGE "Spanning"
#define D_WARMLIGHT "Warm"
#define D_WEB_SERVER "Webserver"

// sonoff.ino
#define D_LEVEL_10 "niveau 1-0"
#define D_LEVEL_01 "niveau 0-1"
#define D_SERIAL_LOGGING_DISABLED "Serieel logging uitgeschakeld"
#define D_SYSLOG_LOGGING_REENABLED "Syslog logging weer ingeschakeld"

#define D_SET_BAUDRATE_TO "Zet baudrate op"
#define D_RECEIVED_TOPIC "Ontvangen topic"
#define D_DATA_SIZE "Data lengte"
#define D_ANALOG_INPUT "Analoog"

#define D_FINGERPRINT "Controleer TLS vingerafdruk..."
#define D_TLS_CONNECT_FAILED_TO "TLS Verbinding mislukt naar"
#define D_RETRY_IN "Opnieuw proberen over"
#define D_VERIFIED "Gecontroleerd"
#define D_INSECURE "Door ongeldige vingerafdruk een onveilige verbinding"
#define D_CONNECT_FAILED_TO "Verbinding mislukt naar"

// support.ino
#define D_OSWATCH "osWatch"
#define D_BLOCKED_LOOP "Blocked Loop"
#define D_WPS_FAILED_WITH_STATUS "WPSconfig mislukt met status"
#define D_ACTIVE_FOR_3_MINUTES "3 minuten actief"
#define D_FAILED_TO_START "mislukt"
#define D_PATCH_ISSUE_2186 "Patch issue 2186"
#define D_CONNECTING_TO_AP "Verbinden met AP"
#define D_IN_MODE "in stand"
#define D_CONNECT_FAILED_NO_IP_ADDRESS "Verbinding mislukt omdat geen IP adres werd ontvangen"
#define D_CONNECT_FAILED_AP_NOT_REACHED "Verbinding mislukt omdat AP onbereikbaar is"
#define D_CONNECT_FAILED_WRONG_PASSWORD "Verbinding mislukt door fout wachtwoord"
#define D_CONNECT_FAILED_AP_TIMEOUT "Verbinding mislukt door AP time-out"
#define D_ATTEMPTING_CONNECTION "Verbinden..."
#define D_CHECKING_CONNECTION "Controleer verbinding..."
#define D_QUERY_DONE "Aanvraag klaar. Aantal MQTT diensten gevonden"
#define D_MQTT_SERVICE_FOUND "MQTT dienst gevonden op"
#define D_FOUND_AT "gevonden op"
#define D_SYSLOG_HOST_NOT_FOUND "Syslog Host niet gevonden"

// settings.ino
#define D_SAVED_TO_FLASH_AT "Opgeslagen in flash op"
#define D_LOADED_FROM_FLASH_AT "Geladen vanuit flash op"
#define D_USE_DEFAULTS "Gebruik standaardwaarden"
#define D_ERASED_SECTOR "Wis sector"

// webserver.ino
#define D_MINIMAL_FIRMWARE_PLEASE_UPGRADE "MINIMAL firmware - opwaarderen"
#define D_WEBSERVER_ACTIVE_ON "Webserver actief op"
#define D_WITH_IP_ADDRESS "met IP adres"
#define D_WEBSERVER_STOPPED "Webserver gestopt"
#define D_FILE_NOT_FOUND "Bestand niet gevonden"
#define D_REDIRECTED "Redirected to captive portal"
#define D_WIFIMANAGER_SET_ACCESSPOINT_AND_STATION "Wifimanager start AccessPoint en hou Station"
#define D_WIFIMANAGER_SET_ACCESSPOINT "Wifimanager start AccessPoint"
#define D_TRYING_TO_CONNECT "Apparaat probeert te verbinden met netwerk"

#define D_RESTART_IN "Herstart over"
#define D_SECONDS "seconden"
#define D_DEVICE_WILL_RESTART "Herstart over enkele seconden"
#define D_BUTTON_TOGGLE "Toggle"  // Wissel, Tuimel
#define D_CONFIGURATION "Configuratie"
#define D_INFORMATION "Informatie"
#define D_FIRMWARE_UPGRADE "Opwaarderen"
#define D_CONSOLE "Console"
#define D_CONFIRM_RESTART "Bevestig herstart"

#define D_CONFIGURE_MODULE "Configureer Module"
#define D_CONFIGURE_WIFI "Configureer WiFi"
#define D_CONFIGURE_MQTT "Configureer MQTT"
#define D_CONFIGURE_DOMOTICZ "Configureer Domoticz"
#define D_CONFIGURE_LOGGING "Configureer Logging"
#define D_CONFIGURE_OTHER "Configureer Overige"
#define D_CONFIRM_RESET_CONFIGURATION "Bevestig reset configuratie"
#define D_RESET_CONFIGURATION "Reset Configuratie"
#define D_BACKUP_CONFIGURATION "Bewaar Configuratie"
#define D_RESTORE_CONFIGURATION "Herstel Configuration"
#define D_MAIN_MENU "Hoofdmenu"

#define D_MODULE_PARAMETERS "Module parameters"
#define D_MODULE_TYPE "Module soort"
#define D_GPIO "GPIO"
#define D_SERIAL_IN "Serieel In"
#define D_SERIAL_OUT "Serieel Uit"

#define D_WIFI_PARAMETERS "Wifi parameters"
#define D_SCAN_FOR_WIFI_NETWORKS "Zoek wifi netwerken"
#define D_SCAN_DONE "Scan klaar"
#define D_NO_NETWORKS_FOUND "Geen netwerken gevonden"
#define D_REFRESH_TO_SCAN_AGAIN "Vernieuw om opnieuw te zoeken"
#define D_DUPLICATE_ACCESSPOINT "Dubbel AccessPoint"
#define D_SKIPPING_LOW_QUALITY "Overslaan agv lage ontvangskwaliteit"
#define D_RSSI "RSSI"
#define D_WEP "WEP"
#define D_WPA_PSK "WPA PSK"
#define D_WPA2_PSK "WPA2 PSK"
#define D_AP1_SSID "AP1 SSId"
#define D_AP1_PASSWORD "AP1 Wachtwoord"
#define D_AP2_SSID "AP2 SSId"
#define D_AP2_PASSWORD "AP2 Wachtwoord"

#define D_MQTT_PARAMETERS "MQTT parameters"
#define D_CLIENT "Client"
#define D_FULL_TOPIC "Volledig Topic"

#define D_LOGGING_PARAMETERS "Logging parameters"
#define D_SERIAL_LOG_LEVEL "Serieel log niveau"
#define D_WEB_LOG_LEVEL "Web log niveau"
#define D_SYS_LOG_LEVEL "Syslog niveau"
#define D_MORE_DEBUG "Meer debug"
#define D_SYSLOG_HOST "Syslog host"
#define D_SYSLOG_PORT "Syslog poort"
#define D_TELEMETRY_PERIOD "Telemetry periode"

#define D_OTHER_PARAMETERS "Overige parameters"
#define D_WEB_ADMIN_PASSWORD "Web Admin Wachtwoord"
#define D_MQTT_ENABLE "MQTT ingeschakeld"
#define D_FRIENDLY_NAME "Beschrijvende naam"
#define D_BELKIN_WEMO "Belkin WeMo"
#define D_HUE_BRIDGE "Hue Bridge"
#define D_SINGLE_DEVICE "een apparaat"
#define D_MULTI_DEVICE "meer apparaten"

#define D_SAVE_CONFIGURATION "Bewaar configuratie"
#define D_CONFIGURATION_SAVED "Configuratie opgeslagen"
#define D_CONFIGURATION_RESET "Configuratie ge-reset"

#define D_PROGRAM_VERSION "Programma Versie"
#define D_BUILD_DATE_AND_TIME "Compileer Datum & Tijd"
#define D_CORE_AND_SDK_VERSION "Core/SDK Versie"
#define D_FLASH_WRITE_COUNT "Aantal Flash opslagen"
#define D_MAC_ADDRESS "MAC Adres"
#define D_MQTT_HOST "MQTT Host"
#define D_MQTT_PORT "MQTT Poort"
#define D_MQTT_CLIENT "MQTT Client"
#define D_MQTT_USER "MQTT Gebruiker"
#define D_MQTT_TOPIC "MQTT Topic"
#define D_MQTT_GROUP_TOPIC "MQTT Groep Topic"
#define D_MQTT_FULL_TOPIC "MQTT Volledig Topic"
#define D_MDNS_DISCOVERY "mDNS Discovery"
#define D_MDNS_ADVERTISE "mDNS Advertise"
#define D_ESP_CHIP_ID "ESP Chip Id"
#define D_FLASH_CHIP_ID "Flash Chip Id"
#define D_FLASH_CHIP_SIZE "Flash Grootte"
#define D_FREE_PROGRAM_SPACE "Programma ruimte over"

#define D_UPGRADE_BY_WEBSERVER "Opwaarderen dmv webserver"
#define D_OTA_URL "OTA Url"
#define D_START_UPGRADE "Start opwaarderen"
#define D_UPGRADE_BY_FILE_UPLOAD "Opwaarderen dmv verzenden bestand"
#define D_UPLOAD_STARTED "Verzending gestart"
#define D_UPGRADE_STARTED "Opwaarderen gestart"
#define D_UPLOAD_DONE "Opwaarderen klaar"
#define D_UPLOAD_ERR_1 "Geen bestand gekozen"
#define D_UPLOAD_ERR_2 "Onvoldoende geheugen ruimte"
#define D_UPLOAD_ERR_3 "Magische byte is niet 0xE9"
#define D_UPLOAD_ERR_4 "Programma flash grootte is groter dan werkelijke flash grootte"
#define D_UPLOAD_ERR_5 "Opwaardeer buffer verschil"
#define D_UPLOAD_ERR_6 "Opwaarderen mislukt. Schakel logging 3 in"
#define D_UPLOAD_ERR_7 "Opwaarderen afgebroken"
#define D_UPLOAD_ERR_8 "Ongeldig bestand"
#define D_UPLOAD_ERR_9 "Bestand is te groot"
#define D_UPLOAD_ERROR_CODE "Opwaardeer foutcode"

#define D_ENTER_COMMAND "Geef opdracht"
#define D_ENABLE_WEBLOG_FOR_RESPONSE "Schakel weblog 2 in indien antwoord verwacht"
#define D_NEED_USER_AND_PASSWORD "Benodig user=<gebruiker>&password=<webwachtwoord>"

// xdrv_wemohue.ino
#define D_MULTICAST_DISABLED "Multicast uitgeschakeld"
#define D_MULTICAST_REJOINED "Multicast verbonden"
#define D_MULTICAST_JOIN_FAILED "Multicast verbinding mislukt"
#define D_FAILED_TO_SEND_RESPONSE "Antwoord versturen mislukt"

#define D_WEMO "WeMo"
#define D_WEMO_BASIC_EVENT "WeMo basis gebeurtenis"
#define D_WEMO_EVENT_SERVICE "WeMo gebeurtenis dienst"
#define D_WEMO_META_SERVICE "WeMo meta service"
#define D_WEMO_SETUP "WeMo installatie"
#define D_RESPONSE_SENT "Antwoord verstuurd"

#define D_HUE "Hue"
#define D_HUE_BRIDGE_SETUP "Hue installatie"
#define D_HUE_API_NOT_IMPLEMENTED "Hue API niet beschikbaar"
#define D_HUE_API "Hue API"
#define D_HUE_POST_ARGS "Hue POST argumenten"
#define D_3_RESPONSE_PACKETS_SENT "3 antwoord paketten verstuurd"

// xdrv_02_irremote.ino
#define D_JSON_INVALID_JSON "Ongeldig JSON"
#define D_JSON_PROTOCOL_NOT_SUPPORTED "Protocol wordt niet ondersteund"
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
#define D_DOMOTICZ_KEY_IDX "Toets idx"
#define D_DOMOTICZ_SWITCH_IDX "Schakelaar idx"
#define D_DOMOTICZ_SENSOR_IDX "Sensor idx"
  #define D_DOMOTICZ_TEMP "Temp"
  #define D_DOMOTICZ_TEMP_HUM "Temp,Hum"
  #define D_DOMOTICZ_TEMP_HUM_BARO "Temp,Hum,Baro"
  #define D_DOMOTICZ_POWER_ENERGY "Power,Energy"
  #define D_DOMOTICZ_ILLUMINANCE "Illuminance"
  #define D_DOMOTICZ_COUNT "Count"
  #define D_DOMOTICZ_VOLTAGE "Spanning"
  #define D_DOMOTICZ_CURRENT "Stroom"
  #define D_DOMOTICZ_AIRQUALITY "AirQuality"
#define D_DOMOTICZ_UPDATE_TIMER "Bijwerk timer"

// xsns_03_energy.ino
#define D_ENERGY_TODAY "Verbruik vandaag"
#define D_ENERGY_YESTERDAY "Verbruik gisteren"
#define D_ENERGY_TOTAL "Verbruik totaal"

// xsns_05_ds18b20.ino
#define D_SENSOR_BUSY "Sensor bezet"
#define D_SENSOR_CRC_ERROR "Sensor CRC fout"
#define D_SENSORS_FOUND "Aantal sensoren"

// xsns_06_dht.ino
#define D_TIMEOUT_WAITING_FOR "Te lang wachten op"
#define D_START_SIGNAL_LOW "laag start signaal"
#define D_START_SIGNAL_HIGH "hoog start signaal"
#define D_PULSE "signaal"
#define D_CHECKSUM_FAILURE "Controle mislukt"

// xsns_07_sht1x.ino
#define D_SENSOR_DID_NOT_ACK_COMMAND "Geen opdracht ACK van sensor"
#define D_SHT1X_FOUND "SHT1X gevonden"

// sonoff_template.h
// Max string length is 8 characters including suffixes
#define D_SENSOR_NONE     "Geen"
#define D_SENSOR_DHT11    "DHT11"
#define D_SENSOR_AM2301   "AM2301"
#define D_SENSOR_SI7021   "SI7021"
#define D_SENSOR_DS18X20  "DS18x20"
#define D_SENSOR_I2C_SCL  "I2C SCL"
#define D_SENSOR_I2C_SDA  "I2C SDA"
#define D_SENSOR_WS2812   "WS2812"
#define D_SENSOR_IRSEND   "IRsend"
#define D_SENSOR_SWITCH   "Switch"  // Suffix "1"
#define D_SENSOR_BUTTON   "Button"  // Suffix "1"
#define D_SENSOR_RELAY    "Relais"  // Suffix "1i"
#define D_SENSOR_LED      "Led"     // Suffix "1i"
#define D_SENSOR_PWM      "PWM"     // Suffix "1"
#define D_SENSOR_COUNTER  "Teller"  // Suffix "1"
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
#define D_UNIT_HOUR "h"
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
#define D_UNIT_SECTORS "sectoren"
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

#endif  // _LANGUAGE_NL_NL_H_
