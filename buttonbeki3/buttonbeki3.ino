/* Blink led with button */

const uint8_t LED_PIN = 13;
const uint8_t BUTTONR_PIN = 2;
const uint8_t BUTTONL_PIN = 3;
uint8_t buttonr_state = 0;
uint8_t buttonl_state = 0;
uint8_t led_state = LOW;

void setup() {
    pinMode(LED_PIN, OUTPUT);
    pinMode(BUTTONR_PIN, INPUT);
    pinMode(BUTTONL_PIN, INPUT);
}

void loop() {
    buttonr_state = digitalRead(BUTTONR_PIN);
    buttonl_state = digitalRead(BUTTONL_PIN);
    if (buttonr_state == LOW && buttonl_state == LOW)
    {
      led_state = !led_state;
      digitalWrite(LED_PIN, led_state);
    }
    delay(100);
}
