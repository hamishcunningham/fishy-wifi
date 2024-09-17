#include <ElfLora.h>

ElfGateway myGateway(0xFF);

void setup() {
  myGateway.initialize();
}

void loop() {
  myGateway.receiveData();
}
