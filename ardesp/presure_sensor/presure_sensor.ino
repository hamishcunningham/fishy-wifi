void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
//  pinMode(A0,INPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
int analogValue = analogRead(A0);
float pressure = (analogValue - 25)* .19; // conversion/"calibration" because sensor 4.5v=1.2MPa
Serial.print("Value: ");
Serial.print(analogValue);
Serial.print("    Pressure: ");
Serial.print(pressure);
Serial.println(" PSI");
delay(400);
}
