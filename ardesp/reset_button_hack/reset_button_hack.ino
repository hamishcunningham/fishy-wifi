#include <FS.h>

boolean getMode();
boolean regularLoop;
void setMode(boolean b);

void setup() {
  Serial.begin(115200);
  SPIFFS.begin(); 
}

void loop() {
  if (!regularLoop) {
    if(getMode()) {
      Serial.println("\ndouble click");
      Serial.println("Running config code");
      setMode(false);
    } else {
      Serial.print("\nwaiting to see...");
      setMode(true);
      delay(4000);
      Serial.println("single click");
      setMode(false);
    }
    regularLoop=true;
  }
  Serial.println("Running main loop code");
  delay(4000);
}

boolean getMode() {
  boolean b = false;
  File f = SPIFFS.open("/mode.txt", "r");
  if(f) {
    b = true;
    f.close();
  }
  return b;
}
void setMode(boolean b) {
  if(b) {
    File f = SPIFFS.open("/mode.txt", "w");
    f.println(" ");
    f.close();
  } else {
    SPIFFS.remove("/mode.txt");
  }
}
