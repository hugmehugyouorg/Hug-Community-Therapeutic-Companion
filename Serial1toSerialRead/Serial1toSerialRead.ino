void waitForSerialMonitor() {
 // allow a little time to connect the serialMonitor before running the rest of the setup.
  for (int i = 10; i>0; i--) {
    delay(1000);
    Serial.print(F(" "));
    Serial.print(i);
  } 
  Serial.println(F(" "));
  Serial.println(F("------- SERIAL MONITOR READY -------"));
}

void setup()
{
    Serial.begin(9600);
    Serial1.begin(9600);
    waitForSerialMonitor();
}

void loop()
{
    while (1) {
	if (Serial1.available() > 0) {
	    Serial.write(Serial1.read());
	}

	if (Serial.available() > 0) {
             Serial1.write(Serial.read());
	}
    }
}
