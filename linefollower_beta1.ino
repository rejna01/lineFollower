#include <AutoPID.h>

#define OUTPUT_MIN -255
#define OUTPUT_MAX 255
#define KP 5
#define KI 3
#define KD 3
// white > black
double difference, outputVal;
double setPoint = 0;

AutoPID pidko(&difference, &setPoint, &outputVal, OUTPUT_MIN, OUTPUT_MAX, KP, KI, KD);

const int left_motor_forward = 7;
const int left_motor_backward = 8;
const int right_motor_forward = 9;
const int right_motor_backward = 10;

const int left_pwm_pin = 6;
const int right_pwm_pin = 7;

const int left_ir_receiver = A0;
const int right_ir_receiver = A1;

const int max_speed = 64;



void setup() {
  Serial.begin(9600);

  pinMode(left_motor_forward, OUTPUT);
  pinMode(left_motor_backward, OUTPUT);
  pinMode(right_motor_forward, OUTPUT);
  pinMode(right_motor_backward, OUTPUT);

  pinMode(left_pwm_pin, OUTPUT);
  pinMode(right_pwm_pin, OUTPUT);

  digitalWrite(left_motor_forward, HIGH);
  digitalWrite(left_motor_backward, LOW);
  digitalWrite(right_motor_forward, HIGH);
  digitalWrite(right_motor_backward, LOW);
}

void loop() {
  updateDifference();
  pidko.run();
  motorVeerControl(outputVal);
}

/****************************************************************/
void updateDifference() {
  difference = analogRead(left_ir_receiver) - analogRead(right_ir_receiver);
}

/****************************************************************/
void motorVeerControl (double turnin) {
  int turning_speed;
  if (turnin > 50) {   // turning right
    analogWrite(left_pwm_pin, max_speed);
    turning_speed = map(255-turnin, 0, 255, -max_speed, max_speed);
    analogWrite(right_pwm_pin, abs(turning_speed));
    if(turning_speed > 0){
      motorSideControl(true, true);
    }
    else if(turning_speed < 0){
      motorSideControl(true, false);
    }
    
  }
  if (turnin < 50) {   // turning left
    analogWrite(right_pwm_pin, max_speed);

    turning_speed = map(255+turnin, 0, 255, -max_speed, max_speed);
    analogWrite(left_pwm_pin, abs(turning_speed));
    if(turning_speed > 0){
      motorSideControl(true, true);
    }
    else if(turning_speed < 0){
      motorSideControl(false, true);
    }
  }
}

void motorSideControl (bool leftMotor, bool rightMotor) {
  digitalWrite(left_motor_forward, (leftMotor? HIGH:LOW));
  digitalWrite(left_motor_backward, (leftMotor? LOW:HIGH));
  digitalWrite(right_motor_forward, (rightMotor? HIGH:LOW));
  digitalWrite(right_motor_backward, (rightMotor? LOW:HIGH));
}
