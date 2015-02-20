# Makefile - toplevel build for Whiffy Fishy

# store the SDK etc. either in ~/esp8266-stuff, or use ./esp8266-local-sdk
STUFFDIR=${HOME}/esp8266-stuff
ESPDIR=$(shell [ -d $(STUFFDIR) ] && \
  echo $(STUFFDIR) || echo `pwd`/esp8266-local-sdk)

# SDK and dev tool paths
ESPLORERDIR=${ESPDIR}/ESPlorer
ESPSDKDIR=$(ESPDIR)/esp-open-sdk
ESPTOOLDIR=$(ESPDIR)/esptool
NODEMCUDIR=$(ESPDIR)/nodemcu-firmware
XTENSABIN=$(ESPSDKDIR)/xtensa-lx106-elf/bin
MQTTSPYDIR=$(ESPDIR)/mqtt-spy
FIXPATH=PATH=${PATH}:$(XTENSABIN)

# which serial port?
# for the Pi: ESPPORT=/dev/ttyAMA0
ESPPORT=/dev/ttyUSB0

# an innocuous default target
help:
	@echo using SDK in $(ESPDIR)
	@echo make targets: nodemcu, nodemcu-with-dns, flashnodemcu, \
          flasholimex, esplorer, mqtt-spy...

# rebuild and flash the firmware to the ESP8266
nodemcu:
	cd $(NODEMCUDIR); $(FIXPATH) $(MAKE)
nodemcu-with-dns:
	cd $(NODEMCUDIR); $(FIXPATH) $(MAKE) UNIVERSAL_TARGET_DEFINES=-DUSE_DNS
flashnodemcu:
	cd $(NODEMCUDIR); $(FIXPATH) $(MAKE) flash ESPPORT=$(ESPPORT)
flasholimex:
	cd $(NODEMCUDIR); $(ESPTOOLDIR)/esptool.py --port $(ESPPORT) \
          write_flash -fs 16m 0 bin/0x00000.bin 0x10000 bin/0x10000.bin
flashold:
	./esp8266-local-sdk/esptool/esptool.py --port $(ESPPORT) write_flash \
          -fs 16m 0 ../nodemcu-firmware/pre_build/0.9.5/nodemcu_20150127.bin 

# run ESPlorer
esplorer:
	cd $(ESPLORERDIR) && java -jar ./ESPlorer.jar &
seed-esplorer-prefs:
	PREFSDIR=${HOME}/.java/.userPrefs/com/; \
	[ -d $$PREFSDIR ] || mkdir -p $$PREFSDIR; cd $$PREFSDIR; \
        tar xvzf $(ESPDIR)/esplorer-snippets.tgz

# run mqtt-spy (note: install mosquitto broker first!)
mqtt-spy:
	cd $(MQTTSPYDIR) && java -jar mqtt-spy-0.1.7-jar-with-dependencies.jar
