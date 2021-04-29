#include <AutoPID.h>

#define OUTPUT_MIN -255
#define OUTPUT_MAX 255
#define KP 0.5
#define KI 0
#define KD 0.2

double difference, outputVal;
double setPoint = 0;

AutoPID pidko(&difference, &setPoint, &outputVal, OUTPUT_MIN, OUTPUT_MAX, KP, KI, KD);

const int left_motor_forward = 7;
const int left_motor_backward = 8;
const int right_motor_forward = 10;
const int right_motor_backward = 9;

const int left_pwm_pin = 5;
const int right_pwm_pin = 6;

const int left_ir_receiver_enable = A2;
const int right_ir_receiver_enable = A1;

const int ir_receiver = A0;

const int max_speed = 81;



void setup() {
  Serial.begin(9600);

  pinMode(left_motor_forward, OUTPUT);
  pinMode(left_motor_backward, OUTPUT);
  pinMode(right_motor_forward, OUTPUT);
  pinMode(right_motor_backward, OUTPUT);
  
  pinMode(left_ir_receiver_enable, OUTPUT);
  pinMode(right_ir_receiver_enable, OUTPUT);

  pinMode(left_pwm_pin, OUTPUT);
  pinMode(right_pwm_pin, OUTPUT);

  digitalWrite(left_motor_forward, HIGH);
  digitalWrite(left_motor_backward, LOW);
  digitalWrite(right_motor_forward, HIGH);
  digitalWrite(right_motor_backward, LOW);

  digitalWrite(left_ir_receiver_enable, LOW);
  digitalWrite(right_ir_receiver_enable, HIGH);
}

void loop() {
  //updateDifference();
  //pidko.run();
  //motorVeerControl(outputVal);
  //motorVeerControl(0);
  //delay(500);

  //980/240

  if(analogRead(A0) > 600){
    motorVeerControl(-80);
  } else if(analogRead(A0) < 600){
    motorVeerControl(90);
  } else{
    motorVeerControl(0);
  }
}

/****************************************************************/
void updateDifference() {
  int right_receiver = analogRead(ir_receiver);
  Serial.print("right:");Serial.println(right_receiver);
  digitalWrite(left_ir_receiver_enable, HIGH);
  digitalWrite(right_ir_receiver_enable, LOW);
  delay(5);
  int left_receiver = analogRead(ir_receiver);
  Serial.print("left:");Serial.println(left_receiver);
  digitalWrite(left_ir_receiver_enable, LOW);
  digitalWrite(right_ir_receiver_enable, HIGH);
  delay(1);
  difference = right_receiver - left_receiver;
  //Serial.print("difference:");Serial.println(difference);
  Serial.print("PIDko: ");Serial.println(outputVal);
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
