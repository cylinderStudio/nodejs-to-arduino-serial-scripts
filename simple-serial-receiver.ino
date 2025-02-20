String inData;
boolean running = false;

void setup() {
  Serial.begin(9600);
  // Serial.println("Waiting for NodeJS to send a signal...\n");
  pinMode(13, OUTPUT);
}

void loop() {
  while (Serial.available() > 0) {
    char received = Serial.read();
    inData += received;

    // Process message when new line character is received
    if (received == '\n') {
      Serial.println("Arduino Received: ");
      Serial.println(inData);

      inData = ""; // Clear received buffer

      if (running) {
        running = false;
      } else {
        running = true;
      }
    }
  }

  if (running) {
    digitalWrite(13, HIGH);   // turn the LED on (HIGH is the voltage level)
    delay(1000);              // wait for a second
    digitalWrite(13, LOW);    // turn the LED off by making the voltage LOW
    delay(1000);              // wait for a second
  }
}
