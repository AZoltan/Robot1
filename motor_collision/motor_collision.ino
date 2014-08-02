#include <SerialLCD.h>
#include <SoftwareSerial.h>
#include <Servo.h>

const uint8_t LED_PIN = 13;
const uint8_t MOTOR_LEFT_DIR_PIN = 4;
const uint8_t MOTOR_LEFT_SPEED_PIN = 5;
const uint8_t MOTOR_RIGHT_DIR_PIN = 7;
const uint8_t MOTOR_RIGHT_SPEED_PIN = 6;

const uint8_t COLL_RIGHT_PIN = 2;    // Right collision detector PIN
const uint8_t COLL_RIGHT_ISR = 0;   // interrupt ID for PIN 2
const uint8_t COLL_LEFT_PIN = 3;   // Left collision detector PIN
const uint8_t COLL_LEFT_ISR = 1;   // interrupt ID for PIN 3

const uint8_t ULTRASONIC_PIN = 9;
long duration = 0; 
float cm = 0;

const uint8_t LCD_RX_PIN = 8;
const uint8_t LCD_TX_PIN = 13;   // collide with LED
SerialLCD slcd(LCD_RX_PIN, LCD_TX_PIN);

const uint8_t SERVO_PIN = 10;
Servo s;            // create servo objet;
int pos = 0;        // position

const uint8_t FORWARD = HIGH;
const uint8_t BACKWARD = LOW;

const uint8_t MAX_SPEED = 250;

volatile bool coll_left = false;
volatile bool coll_right = false;

void motor_set_speed(uint8_t left, uint8_t right) {
    analogWrite(MOTOR_LEFT_SPEED_PIN, left);
    analogWrite(MOTOR_RIGHT_SPEED_PIN, right);
}

void motor_set_direction(uint8_t left, uint8_t right) {
    digitalWrite(MOTOR_LEFT_DIR_PIN, left);
    digitalWrite(MOTOR_RIGHT_DIR_PIN, right);
}

void motor_stop() {
    motor_set_speed(0, 0);
}

void motor_go_forward() {
    motor_set_direction(FORWARD, FORWARD);
    motor_set_speed(MAX_SPEED, MAX_SPEED);
}

void motor_go_backward(unsigned long time) {
    motor_set_direction(BACKWARD, BACKWARD);
    motor_set_speed(MAX_SPEED, MAX_SPEED);
    delay(time);
}

void motor_turn(int degree) {
    if (degree >= 0) {
        motor_set_direction(FORWARD, BACKWARD);
    } else {
        motor_set_direction(BACKWARD, FORWARD);
    }
    motor_set_speed(MAX_SPEED, MAX_SPEED);
    delay(abs(degree)*15);
}


void left_collision_callback() {
    int coll_state = digitalRead(COLL_LEFT_PIN);
    if (coll_state == LOW && coll_left == false) {
        coll_left = true;
    }
}

void right_collision_callback() {
    int coll_state = digitalRead(COLL_RIGHT_PIN);
    if (coll_state == LOW && coll_right == false) {
        coll_right = true;
    }
}

void setup() {
    // inital states
    pinMode(MOTOR_LEFT_DIR_PIN, OUTPUT);
    pinMode(MOTOR_LEFT_SPEED_PIN, OUTPUT);
    pinMode(MOTOR_RIGHT_DIR_PIN, OUTPUT);
    pinMode(MOTOR_RIGHT_SPEED_PIN, OUTPUT); 

    digitalWrite(MOTOR_LEFT_DIR_PIN, LOW);
    digitalWrite(MOTOR_LEFT_SPEED_PIN, LOW);
    digitalWrite(MOTOR_RIGHT_DIR_PIN, LOW);
    digitalWrite(MOTOR_RIGHT_SPEED_PIN, LOW);
  
    pinMode(LED_PIN, OUTPUT); 
    digitalWrite(LED_PIN, LOW); 
    pinMode(COLL_LEFT_PIN, INPUT);
    
    slcd.begin();
    
    s.attach(SERVO_PIN);
    s.write(90);
    
    attachInterrupt(COLL_RIGHT_ISR, right_collision_callback, FALLING);
    attachInterrupt(COLL_LEFT_ISR, left_collision_callback, FALLING);
    
    // start
    motor_go_forward();
    slcd.setCursor(1, 0);
    slcd.print("MOVING FORWARD");
}


void handle_collision(bool coll) {
    digitalWrite(LED_PIN, HIGH);
    motor_stop();
    if (coll == true) {
      motor_go_backward(2000);
      motor_stop();
    }
    s.write(0);
    int left = measure_distance();
    slcd.setCursor(0, 0);
    slcd.print("l=");
    slcd.print((float)left, DEC);
    slcd.print("  ");
    delay(1000);
    s.write(180);
    int right = measure_distance();
    slcd.setCursor(11, 0);
    slcd.print("r=");
    slcd.print((float)right, DEC);
    slcd.print("  ");
    delay(1000);
    if (left > right) {
      slcd.setCursor(7, 0);
      slcd.print(">");
      slcd.setCursor(2, 1);
      slcd.print("TURNING LEFT");
      motor_turn(90);
    }
    else {
      slcd.setCursor(7, 0);
      slcd.print("<");
      slcd.setCursor(2, 1);
      slcd.print("TURNING RIGHT");
      motor_turn(-90);
    }
    motor_stop();
    s.write(90);
    slcd.clear();
    motor_go_forward();
    slcd.setCursor(1, 0);
    slcd.print("MOVING FORWARD");
    digitalWrite(LED_PIN, LOW);  
}

int measure_distance() {
  pinMode(ULTRASONIC_PIN, OUTPUT);
  digitalWrite(ULTRASONIC_PIN, LOW);
  delayMicroseconds(10);
  digitalWrite(ULTRASONIC_PIN, HIGH);
  delayMicroseconds(5);
  
  pinMode(ULTRASONIC_PIN, INPUT);
  duration = pulseIn(ULTRASONIC_PIN, HIGH);
  cm = duration / 57.8;   // v_s = 346 m / s
  return cm;
}

void loop() {
    if (coll_left || coll_right) {
      if (coll_left) {
          handle_collision(true);
      } else {
          handle_collision(true);
      }
      coll_left = false;
      coll_right = false;
    }
    if (measure_distance() <= 15) {
        handle_collision(false); 
    }
    slcd.setCursor(0, 0);
    slcd.print("d = ");
    slcd.print((float)measure_distance(), DEC);
    slcd.print("  ");
    delay(500);
}
