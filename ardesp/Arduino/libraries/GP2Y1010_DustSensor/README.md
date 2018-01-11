# GP2Y1010_DustSensor  
Arduino IDE library for operating SHARP GP2Y1010AU0F dust sensor in ESP-WROOM-02/32(esp8266/ESP32) or Arduino  
version 0.4

# Credits and license  
License MIT

# How to use

* Include this library to your Arduino IDE.
* Wiring GP2Y1010AU0F sensor to your Arduino or ESP-WROOM-02/32.

    From left side,  
    GP2Y1010AU0F blue wire   -> Arduino Vout(5V or 3.3V) via 150 ohm register, and Arduino GND via 220uF Capacitor  
    GP2Y1010AU0F green wire  -> Arduino GND  
    GP2Y1010AU0F white wire  -> Arduino Digital pin  
    GP2Y1010AU0F yellow wire -> Arduino GND  
    GP2Y1010AU0F black wire  -> Arduino Analog pin (if you use ESP-WROOM-02(esp8266), connect TOUT pin.) # show below warning.  
    GP2Y1010AU0F red wire    -> Arduino Vout(5V or 3.3V)  

    warning:
    - ESP-WROOM-02(esp8266) analog(TOUT) pin support under 1V , you must make 1V from 5V or 3.3V  using a voltage divider.  
    - you must blue/red wires(Vin) and black wire(Analog out) are wiring same voltage pins. (DO not wire 5V input and 3.3V outout.)  
    - ESP32 ADC GPIOs support under 3.3V. so, it's easy to blue/red wires(Vin) wiring to 3.3V output, and black wire(analog out) wiring to ADC GPIO.

* Read sample source code. It's very simple !

# caution

* this library DOES NOT support GP2Y101**4**AU0F dust sensor.

# GP2Y1010_DustSensor library function

## Constractor

* GP2Y1010_DustSensor  
  normal constractor. if you use this constractor, you must execute begin() function after this constractor execute.

* GP2Y1010_DustSensor(int ledPin, int measurePin)  
  setting pins, and initialize sensor.  
    ledPin: set white wire digital pin no.  
    measurePin: set black wire analog pin no.  

## public function

* void begin(int ledPin, int measurePin)  
  setting pins, and initialize sensor.  
    ledPin: set white wire digital pin no.  
    measurePin: set black wire analog pin no.  

* void setADCbit(int bit)  
  set ADC bit digit.  
  ESP-WROOM-02(esp8266) and AVR Arduino platform are 10bit, ESP-WROOM-32(ESP32) is 12bit.  
  default value is 10(bit).
  
* int  getADCbit()  
  get established ADC bit digit.
  
* void setInputVolts(float volts)  
  set input voltage (5V or 3.3V).  
  if you set input voltage to 5V, Analog output voltage is 5V, too.  
  if you get analog voltage to 3,3V, you set input voltage to 5V, or 5V to 3.3V using a voltage divider.   
  default value is 5(V).
  
* float getInputVolts()  
  get established input voltage value.

* float getDustDensity()  
  get dust density (the unit is ug/m3)

# link
* GP2Y1010AU0F Data sheet  
  http://www.sharp-world.com/products/device-china/lineup/data/pdf/datasheet/gp2y1010au_e.pdf (English)  
  http://www.sharp.co.jp/products/device/doc/opto/gp2y1010au_j.pdf (Japanese)

* GP2Y1010AU0F Application note  
  http://www.sharp-world.com/products/device-china/lineup/data/pdf/datasheet/gp2y1010au_appl_e.pdf (English)  
  http://www.sharp.co.jp/products/device/doc/opto/gp2y1010au_appl_j.pdf (Japanese)

# history
* ver. 0.1: closed version.
* ver. 0.2: first release version.
* ver. 0.3: support ESP-WROOM-32(ESP32), add set/getADCbit, set/getInputVolts functions.
* ver. 0.31: write "does not support GP2Y1014AU0F", not minus value as getDustDensity().
* ver. 0.4: support AVR Arduino (and clones)  platforms, and delete getDustDensityAvg() and setDustDensityHistoryMaxSize() function.