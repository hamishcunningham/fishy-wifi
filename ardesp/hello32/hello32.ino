
uint64_t chipid;  
void ledOn();
void ledOff();

void setup() {
  Serial.begin(115200);
  pinMode(BUILTIN_LED, OUTPUT); // turn built-in LED on
  blink(3);                     // signal we're starting setup
}

void loop() {
  ledOff();
  chipid=ESP.getEfuseMac(); // chip ID is essentially its MAC address (6 bytes)
  Serial.printf("ESP32 Chip ID is %04X", (uint16_t) (chipid>>32)); // high 2
  Serial.printf("%08X\n",(uint32_t)chipid); // print low 4 bytes

  delay(1500);
  ledOn();
  delay(1500);
}

void ledOn()  { digitalWrite(BUILTIN_LED, LOW); }
void ledOff() { digitalWrite(BUILTIN_LED, HIGH); }
void blink(int times) {
  ledOff();
  for(int i=0; i<times; i++) {
    ledOn(); delay(300); ledOff(); delay(300);
  }
  ledOff();
}
