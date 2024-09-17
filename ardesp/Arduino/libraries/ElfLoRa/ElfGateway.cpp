#include <ElfLora.h>

ElfGateway::ElfGateway(byte addr)
{
  address = addr;
}

void ElfGateway::initialize()
{
  initOLED();

  SPI.begin(5,19,27,18);
  LoRa.setPins(SS,RST,DI0);

  oledPrint("Starting LoRa...", 0);
  if (!LoRa.begin(FREQ)) {
    oledPrint("[FAILED]", 1);
    while (1);
  }

  LoRa.setSpreadingFactor(spreadFactor); // Only radios with same SF will be able to communicate

  oledPrint("[OK] Started.", 1);
  delay(1500);

  oled.clearDisplay();
  oledPrint(" WaterElf Gateway", 0);
  oledPrint("Received:"+String(num_received), 2);
  oledPrint("Sent:"+String(num_sent), 4);
}

void ElfGateway::initOLED()
{
  pinMode(16,OUTPUT);
  digitalWrite(16, LOW);
  delay(50);
  digitalWrite(16, HIGH);
  Wire.begin(4,15);
  oled.init();
  oled.clearDisplay();
}

void ElfGateway::oledPrint(String text, int line)
{
  oled.setTextXY(line,0);
  oled.putString(text);
}

void ElfGateway::sendData(String data, byte nodeAddr)
{
  LoRa.beginPacket();
  LoRa.write(nodeAddr);
  LoRa.write(address);
  LoRa.write(num_sent);
  LoRa.write(data.length());
  LoRa.print(data);
  LoRa.endPacket();
  ++num_sent;
  oledPrint("Sent:"+String(num_sent), 4);
}

void ElfGateway::receiveData()
{
  int packetSize = LoRa.parsePacket();
  if (packetSize) {
    // read packet header bytes:
    int recipient = LoRa.read();          // recipient address
    byte sender = LoRa.read();            // sender address
    byte incomingMsgId = LoRa.read();     // incoming msg ID
    byte incomingLength = LoRa.read();   // incoming msg length

    if (recipient != address && recipient != 0xFF) {
      return;
    }

    String data = "";
    while (LoRa.available()) {
      data += (char)LoRa.read();
    }

    ++num_received;
    oledPrint("Received:"+String(num_received), 2);
    Serial.print("Received: ");
    Serial.println(data);
  }
}

void uploadToSever(String data, byte nodeAddr)
{
  // Upload data to server of choice
}

void ElfGateway::requestData(byte nodeAddr)
{
  sendData("RD", nodeAddr);
}

void ElfGateway::setSF(int sf)
{
  if (sf > 0 && sf < 13) {
    spreadFactor = sf;
    LoRa.setSpreadingFactor(sf);
  } else {
    Serial.println("Invalid spread factor!");
  }
}

int ElfGateway::getSF()
{
  return spreadFactor;
}

void ElfGateway::setAddr(byte addr)
{
  address = addr;
}

byte ElfGateway::getAddr()
{
  return address;
}
