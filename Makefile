# Makefile - toplevel build for Whiffy Fishy

ESPDIR=${HOME}/esp8266-stuff
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
