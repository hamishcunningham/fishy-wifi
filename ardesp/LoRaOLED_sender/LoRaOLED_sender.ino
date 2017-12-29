#include <SPI.h>
#include <LoRa.h>

// WIFI_LoRa_32 ports
// GPIO5 — SX1278’s SCK
// GPIO19 — SX1278’s MISO
// GPIO27 — SX1278’s MOSI
// GPIO18 — SX1278’s CS
// GPIO14 — SX1278’s RESET
// GPIO26 — SX1278’s IRQ(Interrupt Request)

#define SS 18
#define RST 14
#define DI0 26
#define BAND 866E6

#define spreadingFactor 9
// #define SignalBandwidth 62.5E3 // lower range?
#define SignalBandwidth 31.25E3
#define preambleLength 8
#define codingRateDenominator 8

/////////////////////////////////////////////////////////////////////////////
// OLED display stuff ///////////////////////////////////////////////////////
#include <SSD1306.h>
//OLED pins to ESP32 GPIOs via this connecthin:
//OLED_SDA — GPIO4
//OLED_SCL — GPIO15
//OLED_RST — GPIO16
SSD1306 display(0x3c, 4, 15);

/////////////////////////////////////////////////////////////////////////////
// humidity sensor stuff ////////////////////////////////////////////////////
#include <DHT.h>
DHT dht(21, DHT22); // pin for humidity/temp data, plus type DHT22 aka AM2302
float airCelsius, airHumid;
boolean GOT_HUMID_SENSOR = false;  // we'll change later if we detect sensor

/////////////////////////////////////////////////////////////////////////////
// light sensor stuff ///////////////////////////////////////////////////////
#include <Adafruit_Sensor.h>
#include <Adafruit_TSL2591.h>
Adafruit_TSL2591 tsl = Adafruit_TSL2591(2591); // sensor id
int lux;
boolean GOT_LIGHT_SENSOR = false; // we'll change later if we detect sensor

/////////////////////////////////////////////////////////////////////////////
// temperature sensor stuff /////////////////////////////////////////////////
#include <OneWire.h>
#include <DallasTemperature.h>
OneWire oneWire(13); // pin 13 for temp sensor data
DallasTemperature tempSensor(&oneWire);
DeviceAddress tempAddr; // array to hold device address
float temp;
boolean GOT_TEMP_SENSOR = false; // we'll change later if we detect sensor

void setup() {
  pinMode(25,OUTPUT); //Send success, LED will bright 1 second
  
  pinMode(16,OUTPUT);
  digitalWrite(16, LOW); // set GPIO16 low to reset OLED
  delay(50);
  digitalWrite(16, HIGH);
  
  Serial.begin(115200);
//  while (!Serial); //If just the the basic function, must connect to a computer

// Initialising the UI will init the display too.
  display.init();
  
//  display.flipScreenVertically();
  display.setFont(ArialMT_Plain_10);
  display.setTextAlignment(TEXT_ALIGN_LEFT);
  display.drawString(5,0,"LoRa Sender");
  display.display();
  Serial.println("LoRa Sender");

  SPI.begin(5,19,27,18);
  LoRa.setPins(SS,RST,DI0);
  
  if (!LoRa.begin(BAND)) {
    Serial.println("Starting LoRa failed!");
    while (1);
  }
  
  Serial.print("LoRa Spreading Factor: ");
  Serial.println(spreadingFactor);
  LoRa.setSpreadingFactor(spreadingFactor);
  
  Serial.print("LoRa Signal Bandwidth: ");
  Serial.println(SignalBandwidth);
  LoRa.setSignalBandwidth(SignalBandwidth);

  LoRa.setCodingRate4(codingRateDenominator);

  LoRa.setPreambleLength(preambleLength);
  
  Serial.println("LoRa Initial OK");
  display.drawString(5,10,"LoRa Initializing OK");
  display.display();
  delay(2000);

   
  byte error=1;
  /*
  Wire.beginTransmission(0x29);
  error = Wire.endTransmission();
  */
  if(error==0){
    GOT_LIGHT_SENSOR = true;
    Serial.println("Light sensor found");
    
    tsl.begin();  // startup light sensor
    // can change gain of light sensor on the fly, to adapt 
    // brighter/dimmer light situations
    // tsl.setGain(TSL2591_GAIN_LOW);    // 1x gain (bright light)
    tsl.setGain(TSL2591_GAIN_MED);       // 25x gain
    // tsl.setGain(TSL2591_GAIN_HIGH);   // 428x gain
  
    // changing the integration time gives you a longer time over which to
    // sense light longer timelines are slower, but are good in very low light
    // situtations!
    // tsl.setTiming(TSL2591_INTEGRATIONTIME_100MS); // shortest (bright)
    tsl.setTiming(TSL2591_INTEGRATIONTIME_200MS);
    // tsl.setTiming(TSL2591_INTEGRATIONTIME_300MS);
    // tsl.setTiming(TSL2591_INTEGRATIONTIME_400MS);
    // tsl.setTiming(TSL2591_INTEGRATIONTIME_500MS);
    // tsl.setTiming(TSL2591_INTEGRATIONTIME_600MS); // longest (dim)
   
    display.setFont(ArialMT_Plain_16);
    display.drawString(15,0,"LoRa Sender");
    display.setFont(ArialMT_Plain_10);

    display.drawString(17,20,"Light sensor found");
  } else {
    Serial.println("NO light sensor!");
    display.drawString(17,20,"NO light sensor!");
  }
  
taskDISABLE_INTERRUPTS();
  tempSensor.begin();     // start the onewire temperature sensor
  taskENABLE_INTERRUPTS();
  if(tempSensor.getDeviceCount()==1) {
    GOT_TEMP_SENSOR = true;
    taskDISABLE_INTERRUPTS();
    tempSensor.getAddress(tempAddr, 0);
    tempSensor.setResolution(tempAddr, 12); // 12 bit res (DS18B20 does 9-12)
    taskENABLE_INTERRUPTS();
    Serial.println("Temp sensor found");
    display.drawString(15,30,"Temp sensor found");
  } else {
    Serial.println("NO temp sensor!");
    display.drawString(15,30,"NO temp sensor!");
  }
  
  dht.begin();    // start the humidity and air temperature sensor
  airHumid = dht.readHumidity();
  airCelsius = dht.readTemperature();
  if (isnan(airHumid) || isnan(airCelsius)) {
    Serial.println("NO humidity sensor!");
    display.drawString(10,40,"NO humidity sensor!");
  } else {
    GOT_HUMID_SENSOR = true;
    Serial.println("Humidity sensor found");
    display.drawString(10,40,"Humidity sensor found");
  }
  display.display();
  delay(2000);
}

void loop() {
  if (GOT_LIGHT_SENSOR) {
    sensors_event_t event;
    tsl.getEvent(&event);
    lux = event.light;
  }
  if (GOT_TEMP_SENSOR) {
    taskDISABLE_INTERRUPTS(); 
    tempSensor.requestTemperatures(); // Send the command to get temperatures
    taskENABLE_INTERRUPTS();
    temp = tempSensor.getTempCByIndex(0);
  }
  if (GOT_HUMID_SENSOR) {
    airCelsius = dht.readTemperature();
    airHumid = dht.readHumidity();
  }  
  Serial.print("Sending packet: ");
  display.clear();
  display.setFont(ArialMT_Plain_16);
  display.drawString(10, 0, "Sending packet ");
  display.setFont(ArialMT_Plain_10);
  LoRa.beginPacket();
  if (GOT_LIGHT_SENSOR) {
    Serial.println(lux);
    display.drawString(10, 20, String(lux));
    LoRa.print("Lux: ");
    LoRa.println(lux); 
  }
  if (GOT_TEMP_SENSOR) {
    Serial.println(temp,1);
    display.drawString(10, 35, String(temp,1));
    LoRa.print("Temp: ");
    LoRa.println(temp,1);
  }
  if (GOT_HUMID_SENSOR) {
    Serial.println(airCelsius,1);
    Serial.println(airHumid,1);
    display.drawString(50, 35, String(airCelsius,1));
    display.drawString(90, 35, String(airHumid,1));
    LoRa.print("Air Temp: ");
    LoRa.println(airCelsius,1);
    LoRa.print("Air Humid: ");
    LoRa.println(airHumid,1);
  }

  display.display();
  LoRa.endPacket();

  digitalWrite(25, HIGH); // turn the LED on (HIGH is the voltage level)
  delay(1000); // wait for a second
  digitalWrite(25, LOW); // turn the LED off by making the voltage LOW
  delay(1000); // wait for a second
  }
