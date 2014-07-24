// Blinking LED on PIN 13

void setup() {
    pinMode(13, 1);        // set PIN 13 to digital OUTPUT mode
}

void loop() {
    digitalWrite(13, 1);    // write digital 1 to PIN 13
    delay(1000);            // wait for 1000 millisec (1sec)
    digitalWrite(13, 0);    // write digital 0 to PIN 13
    delay(1000);            // wait for 1000 millisec (1sec)
}
