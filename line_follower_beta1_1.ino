#include <AutoPID.h>

#define OUTPUT_MIN -255
#define OUTPUT_MAX 255
#define KP 0.5
#define KI 0
#define KD 0.2

/**********                   pinout                   **********/

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

bool serial_enabled = false;
unsigned long time_between_serial = 500;

/**********             Don't change this!             **********/
double difference, outputVal;
double setPoint = 0;

AutoPID pidko(&difference, &setPoint, &outputVal, OUTPUT_MIN, OUTPUT_MAX, KP, KI, KD);


unsigned long time_now;

int right_receiver = 0;
int left_receiver = 0;


/**********                  Setup                     **********/
void setup() {
  if(serial_enabled){
    Serial.begin(9600);
  }

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

  time_now = millis();
}


/**********        Loop  (uncomment wanted mod)        **********/
void loop() {
  //bumping();  
  //difference_with_pid(); 

  if (serial_enabled && millis() > time_now + time_between_serial){
    time_now = millis();
    whatToPrint();
  }
}


/**********        Chose what do you wanna see         **********/
void whatToPrint(){
  Serial.print("right:");Serial.println(right_receiver);
  Serial.print("left:");Serial.println(left_receiver);
  Serial.print("difference:");Serial.println(difference);
  Serial.print("PIDko: ");Serial.println(outputVal);
}

/**********              Different modes               **********/
void difference_with_pid() {
  updateDifference();
  pidko.run();
  motorVeerControl(outputVal);
}
void bumping() {
  if(analogRead(A0) > 600){
    motorVeerControl(-80);
  } else if(analogRead(A0) < 600){
    motorVeerControl(90);
  } else{
    motorVeerControl(0);
  }
}

/**********              This is for pid               **********/
void updateDifference() {
  right_receiver = analogRead(ir_receiver);
  digitalWrite(left_ir_receiver_enable, HIGH);
  digitalWrite(right_ir_receiver_enable, LOW);
  delay(5);
  left_receiver = analogRead(ir_receiver);
  digitalWrite(left_ir_receiver_enable, LOW);
  digitalWrite(right_ir_receiver_enable, HIGH);
  delay(1);
  difference = right_receiver - left_receiver;
}

/**********              Function to veer              **********/
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

/*******  Function to chose which dirction will motors go *******/
void motorSideControl (bool leftMotor, bool rightMotor) {
  digitalWrite(left_motor_forward, (leftMotor? HIGH:LOW));
  digitalWrite(left_motor_backward, (leftMotor? LOW:HIGH));
  digitalWrite(right_motor_forward, (rightMotor? HIGH:LOW));
  digitalWrite(right_motor_backward, (rightMotor? LOW:HIGH));
}


/**********   If you're reading this, I love you <3    **********/
