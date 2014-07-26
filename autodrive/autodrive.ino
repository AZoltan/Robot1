const uint8_t MOTOR_LEFT_DIR_PIN = 4;
const uint8_t MOTOR_LEFT_SPEED_PIN = 5;
const uint8_t MOTOR_RIGHT_DIR_PIN = 7;
const uint8_t MOTOR_RIGHT_SPEED_PIN = 6;
const uint8_t COLL_RIGHT_PIN = 2;    // Right collision detector PIN
const uint8_t COLL_RIGHT_ISR = 0;   // interrupt ID for PIN 2
const uint8_t COLL_LEFT_PIN = 3;   // Left collision detector PIN
const uint8_t COLL_LEFT_ISR = 1;   // interrupt ID for PIN 3

const uint8_t FORWARD = HIGH;
const uint8_t BACKWARD = LOW;

int speed = 200;
//360 fok = 3630 ms, 1 fok = 10,0833 ms
//1 m = 7000 ms , 1 cm = 70 ms

void stop() {
    if (digitalRead(COLL_RIGHT_PIN) == LOW || digitalRead(COLL_LEFT_PIN) == LOW) {
      set_direction(BACKWARD, BACKWARD);
      delay(1750);
      set_direction(FORWARD, FORWARD);
      if (random(2) == 0) {
        set_speed(speed, 0);        
      }
      else {
        set_speed(0, speed);
      }
      delay(random(908, 2723));
      set_speed(speed, speed);
    }   
}

void set_speed(uint8_t left, uint8_t right) {
    analogWrite(MOTOR_LEFT_SPEED_PIN, left);
    analogWrite(MOTOR_RIGHT_SPEED_PIN, right);
}

void set_direction(uint8_t left, uint8_t right) {
    digitalWrite(MOTOR_LEFT_DIR_PIN, left);
    digitalWrite(MOTOR_RIGHT_DIR_PIN, right);
}

void setup() {
    // inital states
    
    pinMode(COLL_LEFT_PIN, INPUT);
    pinMode(COLL_RIGHT_PIN, INPUT);
    
    pinMode(MOTOR_LEFT_DIR_PIN, OUTPUT);
    pinMode(MOTOR_LEFT_SPEED_PIN, OUTPUT);
    pinMode(MOTOR_RIGHT_DIR_PIN, OUTPUT);
    pinMode(MOTOR_RIGHT_SPEED_PIN, OUTPUT); 

    digitalWrite(MOTOR_LEFT_DIR_PIN, LOW);
    digitalWrite(MOTOR_LEFT_SPEED_PIN, LOW);
    digitalWrite(MOTOR_RIGHT_DIR_PIN, LOW);
    digitalWrite(MOTOR_RIGHT_SPEED_PIN, LOW);
    
    set_direction(FORWARD, FORWARD);
    set_speed(speed, speed);

    attachInterrupt(COLL_RIGHT_ISR, stop, FALLING);
    attachInterrupt(COLL_LEFT_ISR, stop, FALLING);    
}

void loop() {
    delay(1000);
}
