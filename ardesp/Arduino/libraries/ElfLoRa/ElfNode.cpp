#include <ElfLora.h>

ElfNode::ElfNode(byte localAddr, byte gateway)
{
  localAddress = localAddr;
  gatewayAddress = gateway;
  spreadFactor = 7;
  num_sent = 0;
  num_received = 0;
}

void ElfNode::initialize()
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
  oledPrint(" WaterElf Node", 0);
  oledPrint("Sent:"+String(num_sent), 2);
  oledPrint("Gateway:"+String(gatewayAddress, HEX), 4);
}

void ElfNode::initOLED()
{
  pinMode(16,OUTPUT);
  digitalWrite(16, LOW);
  delay(50);
  digitalWrite(16, HIGH);
  Wire.begin(4,15);
  oled.init();
  oled.clearDisplay();
}

void ElfNode::oledPrint(String text, int line)
{
  oled.setTextXY(line,0);
  oled.putString(text);
}

void ElfNode::sendData(String data)
{
  LoRa.beginPacket();
  LoRa.write(gatewayAddress);
  LoRa.write(localAddress);
  LoRa.write(num_sent);
  LoRa.write(data.length());
  LoRa.print(data);
  LoRa.endPacket();
  ++num_sent;
  oledPrint("Sent:"+String(num_sent), 2);
}

void ElfNode::sendRequested()
{
  // Reply with requested data
  Serial.println("Request received, sending...");
}

void ElfNode::receiveData()
{
  int packetSize = LoRa.parsePacket();
  if (packetSize) {
    // read packet header bytes:
    int recipient = LoRa.read();          // recipient address
    byte sender = LoRa.read();            // sender address
    byte incomingMsgId = LoRa.read();     // incoming msg ID
    byte incomingLength = LoRa.read();   // incoming msg length

    if (recipient != localAddress && recipient != gatewayAddress) {
      return;
    }

    String data = "";
    while (LoRa.available()) {
      data += (char)LoRa.read();
    }

    if (data == "RD") {
      sendRequested();
    }

    ++num_received;
    oledPrint("Received:"+String(num_received), 4);
  }
}

void ElfNode::setAddr(byte address)
{
  localAddress = address;
}

void ElfNode::setGatewayAddr(byte address)
{
  gatewayAddress = address;
}

void ElfNode::setSF(int sf)
{
  if (sf > 0 && sf < 13) {
    spreadFactor = sf;
    LoRa.setSpreadingFactor(sf);
  } else {
    Serial.println("Invalid spread factor!");
  }
}

byte ElfNode::getAddr()
{
  return localAddress;
}

byte ElfNode::getGatewayAddr()
{
  return gatewayAddress;
}

int ElfNode::getSF()
{
  return spreadFactor;
}
