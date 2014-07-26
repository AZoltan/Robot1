/* Blink led with button */

const uint8_t LED_PIN = 13;
const uint8_t BUTTON_PIN = 12;
uint8_t button_state = 0;
uint8_t led_state = LOW;

void setup() {
    pinMode(LED_PIN, OUTPUT);
    pinMode(BUTTON_PIN, INPUT);
}

void loop() {
    button_state = digitalRead(BUTTON_PIN);
    if (button_state == LOW)
    {
      led_state = !led_state;
      digitalWrite(LED_PIN, led_state);
    }
    delay(100);
}
