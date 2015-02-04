# Makefile - toplevel build for Whiffy Fishy

STUFFDIR=${HOME}/esp8266-stuff
ESPDIR=$(shell [ -d $(STUFFDIR) ] && echo $(STUFFDIR) || echo ./esp8266-stuff)
ESPLORERDIR=${HOME}/esp8266-stuff/ESPlorer
ESPSDKDIR=$(ESPDIR)/esp-open-sdk
NODEMCUDIR=$(ESPDIR)/nodemcu-firmware
XTENSABIN=$(ESPSDKDIR)/xtensa-lx106-elf/bin
FIXPATH=PATH=${PATH}:$(XTENSABIN)
# for the Pi: ESPPORT=/dev/ttyAMA0
ESPPORT=/dev/ttyUSB0

# rebuild and flash the firmware to the ESP8266
nodemcu:
	cd $(NODEMCUDIR); $(FIXPATH) $(MAKE)
nodemcu-with-dns:
	cd $(NODEMCUDIR); $(FIXPATH) $(MAKE) UNIVERSAL_TARGET_DEFINES=-DUSE_DNS
flashnodemcu:
	cd $(NODEMCUDIR); $(FIXPATH) $(MAKE) flash ESPPORT=$(ESPPORT)
flasholimex:
	cd $(NODEMCUDIR); ../esptool/esptool.py --port $(ESPPORT) \
          write_flash -fs 16m 0 bin/0x00000.bin 0x10000 bin/0x10000.bin

# run ESPlorer
esplorer:
	cd $(ESPLORERDIR) && java -jar ./ESPlorer.jar
