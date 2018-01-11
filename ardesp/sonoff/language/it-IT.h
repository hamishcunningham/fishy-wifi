/*
  it-IT.h - localization for Italian - Italy for Sonoff-Tasmota

  Copyright (C) 2018 Gennaro Tortone

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

#ifndef _LANGUAGE_IT_IT_H_
#define _LANGUAGE_IT_IT_H_

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

#define D_DAY3LIST "DomLunMarMerGioVenSab"
#define D_MONTH3LIST "GenFebMarAprMagGiuLugAgoSetOttNovDic"

// Non JSON decimal separator
#define D_DECIMAL_SEPARATOR "."

// Common
#define D_ADMIN "Admin"
#define D_AIR_QUALITY "Qualita' dell'aria"
#define D_AP "AP"                    // Access Point
#define D_AS "come"
#define D_AUTO "AUTO"
#define D_BLINK "Blink"
#define D_BLINKOFF "BlinkOff"
#define D_BOOT_COUNT "Boot Count"
#define D_BRIGHTLIGHT "Luminoso"
#define D_BUTTON "Pulsante"
#define D_BY "da"                    // Written by me
#define D_BYTES "Bytes"
#define D_CELSIUS "Celsius"
#define D_CO2 "CO2"
#define D_CODE "codice"                // Button code
#define D_COLDLIGHT "Fredda"
#define D_COMMAND "Command"
#define D_CONNECTED "Connesso"
#define D_COUNT "Conteggio"
#define D_COUNTER "Contatore"
#define D_CURRENT "Corrente"          // As in Voltage and Current
#define D_DATA "Dati"
#define D_DARKLIGHT "Scuro"
#define D_DEBUG "Debug"
#define D_DISABLED "Disabilitato"
#define D_DNS_SERVER "DNS Server"
#define D_DONE "Fatto"
#define D_DST_TIME "DST"
#define D_EMULATION "Emulazione"
#define D_ENABLED "Abilitato"
#define D_ERASE "Erase"
#define D_ERROR "Error"
#define D_FAHRENHEIT "Fahrenheit"
#define D_FAILED "Fallito"
#define D_FALLBACK "Fallback"
#define D_FALLBACK_TOPIC "Topic Riserva"
#define D_FALSE "False"
#define D_FILE "File"
#define D_FREE_MEMORY "Memoria Libera"
#define D_GAS "Gas"
#define D_GATEWAY "Gateway"
#define D_GROUP "Gruppo"
#define D_HOST "Host"
#define D_HOSTNAME "Hostname"
#define D_HUMIDITY "Umidita'"
#define D_ILLUMINANCE "Illuminazione"
#define D_IMMEDIATE "immediato"      // Button immediate
#define D_INDEX "Index"
#define D_INFO "Info"
#define D_INITIALIZED "Inizializzato"
#define D_IP_ADDRESS "Indirizzo IP"
#define D_LIGHT "Luce"
#define D_LWT "LWT"
#define D_MODULE "Module"
#define D_MQTT "MQTT"
#define D_MULTI_PRESS "multi-pressione"
#define D_NOISE "Rumore"
#define D_NONE "Nessuno"
#define D_OFF "Off"
#define D_OFFLINE "Offline"
#define D_OK "Ok"
#define D_ON "On"
#define D_ONLINE "Online"
#define D_PASSWORD "Password"
#define D_PORT "Porta"
#define D_POWER_FACTOR "Fattore di potenza"
#define D_POWERUSAGE "Potenza"
#define D_PRESSURE "Pressione"
#define D_PRESSUREATSEALEVEL "Pressione al livello del mare"
#define D_PROGRAM_FLASH_SIZE "Dimensione Flash Programma"
#define D_PROGRAM_SIZE "Dimensione Programma"
#define D_PROJECT "Project"
#define D_RECEIVED "Ricevuto"
#define D_RESTART "Riavvio"
#define D_RESTARTING "Restarting"
#define D_RESTART_REASON "Causa Riavvio"
#define D_RESTORE "ripristino"
#define D_RETAINED "salvato"
#define D_SAVE "Salva"
#define D_SENSOR "Sensore"
#define D_SSID "SSId"
#define D_START "Start"
#define D_STD_TIME "STD"
#define D_STOP "Stop"
#define D_SUBNET_MASK "Maschera sottorete"
#define D_SUBSCRIBE_TO "Sottoscrivi a"
#define D_SUCCESSFUL "Successful"
#define D_TEMPERATURE "Temperature"
#define D_TO "a"
#define D_TOGGLE "Toggle"
#define D_TOPIC "Topic"
#define D_TRANSMIT "Trasmesso"
#define D_TRUE "True"
#define D_UPGRADE "aggiornamento"
#define D_UPLOAD "Invio"
#define D_UPTIME "Uptime"
#define D_USER "Utente"
#define D_UTC_TIME "UTC"
#define D_UV_LEVEL "Livello UV"
#define D_VERSION "Version"
#define D_VOLTAGE "Tensione"
#define D_WARMLIGHT "Calda"
#define D_WEB_SERVER "Web Server"

// sonoff.ino
#define D_LEVEL_10 "level 1-0"
#define D_LEVEL_01 "level 0-1"
#define D_SERIAL_LOGGING_DISABLED "Log seriale disabilitato"
#define D_SYSLOG_LOGGING_REENABLED "Syslog ri-abilitato"

#define D_SET_BAUDRATE_TO "Baudrate impostato a"
#define D_RECEIVED_TOPIC "Topic Ricevuto"
#define D_DATA_SIZE "Dimensione Dati"
#define D_ANALOG_INPUT "Ingresso Analogico"

#define D_FINGERPRINT "Verifica TLS fingerprint..."
#define D_TLS_CONNECT_FAILED_TO "Connessione TLS fallita a"
#define D_RETRY_IN "Nuovo tentativo in"
#define D_VERIFIED "Verificato"
#define D_INSECURE "Connessione insicura a causa di Fingerprint non valido"
#define D_CONNECT_FAILED_TO "Connessione Fallita a"

// support.ino
#define D_OSWATCH "osWatch"
#define D_BLOCKED_LOOP "Ciclo Bloccato"
#define D_WPS_FAILED_WITH_STATUS "WPSconfig Fallito con stato"
#define D_ACTIVE_FOR_3_MINUTES "attivo per 3 minuti"
#define D_FAILED_TO_START "partenza fallita"
#define D_PATCH_ISSUE_2186 "Patch issue 2186"
#define D_CONNECTING_TO_AP "Connessione ad AP"
#define D_IN_MODE "in modalita'"
#define D_CONNECT_FAILED_NO_IP_ADDRESS "Connessione fallita, indirizzo IP non ricevuto"
#define D_CONNECT_FAILED_AP_NOT_REACHED "Connessione fallita, AP non raggiungibile"
#define D_CONNECT_FAILED_WRONG_PASSWORD "Connessione fallita, password AP non corretta"
#define D_CONNECT_FAILED_AP_TIMEOUT "Connessione fallita, timeout AP"
#define D_ATTEMPTING_CONNECTION "Tentativo di connessione..."
#define D_CHECKING_CONNECTION "Controllo connessione..."
#define D_QUERY_DONE "Query eseguita. Servizio MQTT trovato"
#define D_MQTT_SERVICE_FOUND "Servizio MQTT trovato su"
#define D_FOUND_AT "trovato a"
#define D_SYSLOG_HOST_NOT_FOUND "Syslog Host non trovato"

// settings.ino
#define D_SAVED_TO_FLASH_AT "Salvato nella flash in"
#define D_LOADED_FROM_FLASH_AT "Caricato dalla flash da"
#define D_USE_DEFAULTS "Utilizzo valori default"
#define D_ERASED_SECTOR "Settore cancellato"

// webserver.ino
#define D_MINIMAL_FIRMWARE_PLEASE_UPGRADE "MINIMAL firmware - effettuare aggiornamento"
#define D_WEBSERVER_ACTIVE_ON "Web server attivo su"
#define D_WITH_IP_ADDRESS "con indirizzo IP"
#define D_WEBSERVER_STOPPED "Web server arrestato"
#define D_FILE_NOT_FOUND "File Non Trovato"
#define D_REDIRECTED "Redirezione al captive portal"
#define D_WIFIMANAGER_SET_ACCESSPOINT_AND_STATION "Impostazione Wifimanager come AccessPoint e Station"
#define D_WIFIMANAGER_SET_ACCESSPOINT "Impostazione Wifimanager come AccessPoint"
#define D_TRYING_TO_CONNECT "Tentativo di connessione del dispositivo alla rete"

#define D_RESTART_IN "Riavvio in"
#define D_SECONDS "secondi"
#define D_DEVICE_WILL_RESTART "Il dispositivo verra' riavviato tra pochi secondi"
#define D_BUTTON_TOGGLE "On/Off"
#define D_CONFIGURATION "Configurazione"
#define D_INFORMATION "Informazioni"
#define D_FIRMWARE_UPGRADE "Aggiornamento Firmware"
#define D_CONSOLE "Console"
#define D_CONFIRM_RESTART "Conferma Riavvio"

#define D_CONFIGURE_MODULE "Configurazione Modulo"
#define D_CONFIGURE_WIFI "Configurazione WiFi"
#define D_CONFIGURE_MQTT "Configurazione MQTT"
#define D_CONFIGURE_DOMOTICZ "Configurazione Domoticz"
#define D_CONFIGURE_LOGGING "Configurazione Logging"
#define D_CONFIGURE_OTHER "Configurazione Extra"
#define D_CONFIRM_RESET_CONFIGURATION "Conferma Reset Configurazione"
#define D_RESET_CONFIGURATION "Reset Configurazione"
#define D_BACKUP_CONFIGURATION "Backup Configurazione"
#define D_RESTORE_CONFIGURATION "Ripristino Configurazione"
#define D_MAIN_MENU "Menu' Principale"

#define D_MODULE_PARAMETERS "Parametri del modulo"
#define D_MODULE_TYPE "Tipo modulo"
#define D_GPIO "GPIO"
#define D_SERIAL_IN "Serial In"
#define D_SERIAL_OUT "Serial Out"

#define D_WIFI_PARAMETERS "Parametri Wifi"
#define D_SCAN_FOR_WIFI_NETWORKS "Scansione delle reti wifi"
#define D_SCAN_DONE "Scansione effettuata"
#define D_NO_NETWORKS_FOUND "Nessuna rete trovata"
#define D_REFRESH_TO_SCAN_AGAIN "Ricarica per nuova scansione"
#define D_DUPLICATE_ACCESSPOINT "AccessPoint duplicato"
#define D_SKIPPING_LOW_QUALITY "Ignorato a causa di bassa qualita'"
#define D_RSSI "RSSI"
#define D_WEP "WEP"
#define D_WPA_PSK "WPA PSK"
#define D_WPA2_PSK "WPA2 PSK"
#define D_AP1_SSID "AP1 SSId"
#define D_AP1_PASSWORD "AP1 Password"
#define D_AP2_SSID "AP2 SSId"
#define D_AP2_PASSWORD "AP2 Password"

#define D_MQTT_PARAMETERS "Parametri MQTT"
#define D_CLIENT "Client"
#define D_FULL_TOPIC "Full Topic"

#define D_LOGGING_PARAMETERS "Parametri Logging"
#define D_SERIAL_LOG_LEVEL "Seriale livello di log"
#define D_WEB_LOG_LEVEL "Web livello di log"
#define D_SYS_LOG_LEVEL "Sys livello di log"
#define D_MORE_DEBUG "Debug aggiuntivo"
#define D_SYSLOG_HOST "Syslog host"
#define D_SYSLOG_PORT "Syslog porta"
#define D_TELEMETRY_PERIOD "Periodo Telemetria"

#define D_OTHER_PARAMETERS "Altri parametri"
#define D_WEB_ADMIN_PASSWORD "Password Amministratore Web"
#define D_MQTT_ENABLE "Abilita MQTT"
#define D_FRIENDLY_NAME "Nome confidenziale"
#define D_BELKIN_WEMO "Belkin WeMo"
#define D_HUE_BRIDGE "Hue Bridge"
#define D_SINGLE_DEVICE "dispositivo singolo"
#define D_MULTI_DEVICE "dispositivo multiplo"

#define D_SAVE_CONFIGURATION "Salva configurazione"
#define D_CONFIGURATION_SAVED "Configurazione salvata"
#define D_CONFIGURATION_RESET "Configurazione azzerata"

#define D_PROGRAM_VERSION "Versione del programma"
#define D_BUILD_DATE_AND_TIME "Data e ora compilazione"
#define D_CORE_AND_SDK_VERSION "Versione Core/SDK"
#define D_FLASH_WRITE_COUNT "Contatore scrittura Flash"
#define D_MAC_ADDRESS "Indirizzo MAC"
#define D_MQTT_HOST "MQTT Host"
#define D_MQTT_PORT "MQTT Porta"
#define D_MQTT_CLIENT "MQTT Client"
#define D_MQTT_USER "MQTT Utente"
#define D_MQTT_TOPIC "MQTT Topic"
#define D_MQTT_GROUP_TOPIC "MQTT Group Topic"
#define D_MQTT_FULL_TOPIC "MQTT Full Topic"
#define D_MDNS_DISCOVERY "mDNS Discovery"
#define D_MDNS_ADVERTISE "mDNS Advertise"
#define D_ESP_CHIP_ID "ESP Chip Id"
#define D_FLASH_CHIP_ID "Flash Chip Id"
#define D_FLASH_CHIP_SIZE "Dimensione Flash"
#define D_FREE_PROGRAM_SPACE "Spazio Libero Memoria Programma"

#define D_UPGRADE_BY_WEBSERVER "Aggiornamento da web server"
#define D_OTA_URL "OTA Url"
#define D_START_UPGRADE "Avvio aggiornamento"
#define D_UPGRADE_BY_FILE_UPLOAD "Aggiornamento tramite invio file"
#define D_UPLOAD_STARTED "Invio iniziato"
#define D_UPGRADE_STARTED "Aggiornamento avviato"
#define D_UPLOAD_DONE "Invio effettuato"
#define D_UPLOAD_ERR_1 "Nessun file selezionato"
#define D_UPLOAD_ERR_2 "Spazio insufficiente"
#define D_UPLOAD_ERR_3 "Magic byte non corrispondente a 0xE9"
#define D_UPLOAD_ERR_4 "Dimensione della memoria programma maggiore della dimensione reale della flash"
#define D_UPLOAD_ERR_5 "Errore di comparazione del buffer di invio"
#define D_UPLOAD_ERR_6 "Invio fallito. Abilitare logging 3"
#define D_UPLOAD_ERR_7 "Invio annullato"
#define D_UPLOAD_ERR_8 "File non valido"
#define D_UPLOAD_ERR_9 "File troppo grande"
#define D_UPLOAD_ERROR_CODE "Codice errore invio"

#define D_ENTER_COMMAND "Inserire comando"
#define D_ENABLE_WEBLOG_FOR_RESPONSE "Abilitare weblog 2 se e' attesa una risposta"
#define D_NEED_USER_AND_PASSWORD "Richiesto user=<username>&password=<password>"

// xdrv_wemohue.ino
#define D_MULTICAST_DISABLED "Multicast disabilitato"
#define D_MULTICAST_REJOINED "Multicast (ri)associato"
#define D_MULTICAST_JOIN_FAILED "Associazione Multicast fallita"
#define D_FAILED_TO_SEND_RESPONSE "Invio risposta fallito"

#define D_WEMO "WeMo"
#define D_WEMO_BASIC_EVENT "WeMo evento base"
#define D_WEMO_EVENT_SERVICE "WeMo servizio eventi"
#define D_WEMO_META_SERVICE "WeMo meta service"
#define D_WEMO_SETUP "Impostazione WeMo"
#define D_RESPONSE_SENT "Risposta inviata"

#define D_HUE "Hue"
#define D_HUE_BRIDGE_SETUP "Impostazione Hue"
#define D_HUE_API_NOT_IMPLEMENTED "Hue API non implementata"
#define D_HUE_API "Hue API"
#define D_HUE_POST_ARGS "Hue POST argomenti"
#define D_3_RESPONSE_PACKETS_SENT "3 pacchetti di risposta inviati"

// xdrv_02_irremote.ino
#define D_JSON_INVALID_JSON "JSON non valido"
#define D_JSON_PROTOCOL_NOT_SUPPORTED "Protocollo non supportato"
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
#define D_DOMOTICZ_PARAMETERS "Parametri Domoticz"
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
#define D_DOMOTICZ_UPDATE_TIMER "Intervallo di aggiornamento"

// xsns_03_energy.ino
#define D_ENERGY_TODAY "Energia Oggi"
#define D_ENERGY_YESTERDAY "Energia Ieri"
#define D_ENERGY_TOTAL "Energia Totale"

// xsns_05_ds18b20.ino
#define D_SENSOR_BUSY "Sensore occupato"
#define D_SENSOR_CRC_ERROR "Sensore errore CRC"
#define D_SENSORS_FOUND "Sensori trovati"

// xsns_06_dht.ino
#define D_TIMEOUT_WAITING_FOR "Attesa timeout per"
#define D_START_SIGNAL_LOW "inizio segnale basso"
#define D_START_SIGNAL_HIGH "inizio segnale alto"
#define D_PULSE "impulso"
#define D_CHECKSUM_FAILURE "Checksum fallito"

// xsns_07_sht1x.ino
#define D_SENSOR_DID_NOT_ACK_COMMAND "Sensore non ha eseguito il comando ACK"
#define D_SHT1X_FOUND "SHT1X trovato"

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
#define D_UNIT_SECTORS "settori"
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

#endif  // _LANGUAGE_IT_IT_H_
