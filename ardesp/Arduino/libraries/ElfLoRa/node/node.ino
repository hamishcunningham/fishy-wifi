#include <ElfLora.h>

ElfNode myNode(0xBB, 0xFF);

void setup() {
  myNode.initialize();
}

void loop() {
  myNode.sendData("Hi gateway!");
  delay(3000);
}
