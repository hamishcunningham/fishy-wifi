
uint64_t chipid;  

void setup() {
  Serial.begin(115200);
}

void loop() {
  chipid=ESP.getEfuseMac(); // chip ID is essentially its MAC address (6 bytes)
  Serial.printf("ESP32 Chip ID is %04X", (uint16_t) (chipid>>32)); // high 2
  Serial.printf("%08X\n",(uint32_t)chipid); // print low 4 bytes

  delay(3000);
}
