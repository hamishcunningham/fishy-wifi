EESchema Schematic File Version 2
LIBS:power
LIBS:device
LIBS:transistors
LIBS:conn
LIBS:linear
LIBS:regul
LIBS:74xx
LIBS:cmos4000
LIBS:adc-dac
LIBS:memory
LIBS:xilinx
LIBS:microcontrollers
LIBS:dsp
LIBS:microchip
LIBS:analog_switches
LIBS:motorola
LIBS:texas
LIBS:intel
LIBS:audio
LIBS:interface
LIBS:digital-audio
LIBS:philips
LIBS:display
LIBS:cypress
LIBS:siliconi
LIBS:opto
LIBS:atmel
LIBS:contrib
LIBS:valves
LIBS:ESP8266
LIBS:waterelf
LIBS:waterelf-cache
EELAYER 25 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 1 1
Title "Waterelf.sch"
Date "2 jun 2016"
Rev "1"
Comp "WeGrow.social"
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
$Comp
L ESP-12E U?
U 1 1 574FED3F
P 5150 2250
F 0 "U?" H 5150 2150 50  0000 C CNN
F 1 "ESP-12E" H 5150 2350 50  0000 C CNN
F 2 "" H 5150 2250 60  0001 C CNN
F 3 "" H 5150 2250 60  0001 C CNN
	1    5150 2250
	1    0    0    -1  
$EndComp
$Comp
L +5V #PWR?
U 1 1 575438F9
P 1200 1300
F 0 "#PWR?" H 1200 1150 50  0001 C CNN
F 1 "+5V" H 1200 1440 50  0000 C CNN
F 2 "" H 1200 1300 50  0000 C CNN
F 3 "" H 1200 1300 50  0000 C CNN
	1    1200 1300
	1    0    0    -1  
$EndComp
$Comp
L C C?
U 1 1 57543D01
P 1450 1650
F 0 "C?" H 1475 1750 50  0000 L CNN
F 1 "10uF" H 1475 1550 50  0000 L CNN
F 2 "" H 1488 1500 50  0000 C CNN
F 3 "" H 1450 1650 50  0000 C CNN
	1    1450 1650
	1    0    0    -1  
$EndComp
$Comp
L C C?
U 1 1 57543D6A
P 2350 1650
F 0 "C?" H 2375 1750 50  0000 L CNN
F 1 "22uF" H 2375 1550 50  0000 L CNN
F 2 "" H 2388 1500 50  0000 C CNN
F 3 "" H 2350 1650 50  0000 C CNN
	1    2350 1650
	1    0    0    -1  
$EndComp
Connection ~ 2350 1400
Wire Wire Line
	1450 1400 1450 1500
Connection ~ 1450 1400
$Comp
L GND #PWR?
U 1 1 5754435F
P 1200 2000
F 0 "#PWR?" H 1200 1750 50  0001 C CNN
F 1 "GND" H 1200 1850 50  0000 C CNN
F 2 "" H 1200 2000 50  0000 C CNN
F 3 "" H 1200 2000 50  0000 C CNN
	1    1200 2000
	1    0    0    -1  
$EndComp
Wire Wire Line
	1900 1700 1900 1900
$Comp
L +3V3 #PWR?
U 1 1 5754456B
P 2600 1300
F 0 "#PWR?" H 2600 1150 50  0001 C CNN
F 1 "+3V3" H 2600 1440 50  0000 C CNN
F 2 "" H 2600 1300 50  0000 C CNN
F 3 "" H 2600 1300 50  0000 C CNN
	1    2600 1300
	1    0    0    -1  
$EndComp
Wire Wire Line
	2600 1400 2600 1300
$Comp
L AZ1117CH U?
U 1 1 575442BE
P 1900 1400
F 0 "U?" H 2000 1150 50  0000 C CNN
F 1 "AZ1117CH" H 1900 1650 50  0000 C CNN
F 2 "SOT223" H 1900 1050 50  0001 C CNN
F 3 "http://www.diodes.com/_files/datasheets/AZ1117C.pdf" H 2000 1150 50  0001 C CNN
	1    1900 1400
	1    0    0    -1  
$EndComp
Wire Wire Line
	2350 1500 2350 1400
Wire Wire Line
	1200 1900 1200 2000
Wire Wire Line
	1200 1400 1200 1300
Wire Wire Line
	2200 1400 2600 1400
Wire Wire Line
	1200 1400 1600 1400
Wire Wire Line
	1200 1900 2350 1900
Wire Wire Line
	2350 1900 2350 1800
Connection ~ 1900 1900
Wire Wire Line
	1450 1800 1450 1900
Connection ~ 1450 1900
$EndSCHEMATC
