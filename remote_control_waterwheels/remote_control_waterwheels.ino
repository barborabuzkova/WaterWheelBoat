#include <IRremote.h> // library for remote

#include <AFMotor.h> // library for DC motors
#include <Servo.h> // library for servo motors

AF_DCMotor motorleft(1); //DC motor setup
AF_DCMotor motorright(2);
int speedint = 0;
boolean forwards_backwards = true; // true = forwards, false = backwards
const int SPEED_INCREMENT = 50;

Servo servomotor; // servo motor (contorls turning/rudder)
int servo_position = 120; // servo starts perpendicular to boat body, but the servo is at an angle  
const int SERVO_INCREMENT = 5;

const int RECV_PIN = A14; // IR reciever starting code
IRrecv irrecv(RECV_PIN);
decode_results results;
unsigned long key_value = 0;
int reading = 0;
void setup() {
  Serial.begin(9600); // IR remote setup
  irrecv.enableIRIn();
  irrecv.blink13(true);  
  Serial.println("setuped");

  motorleft.setSpeed(speedint); //DC motor setup
  motorleft.run(RELEASE);
  motorright.setSpeed(speedint);
  motorright.run(RELEASE);

  servomotor.attach(9); // servo motor setup
}

void loop() {
  if (irrecv.decode(&results)){
 
        if (results.value == 0XFFFFFFFF)
          results.value = key_value;

        switch(results.value){
          case 0xFFA25D:   // stops motors
          Serial.println("Power");
          motorleft.setSpeed(0);
          motorleft.run(FORWARD);
          motorright.setSpeed(0);
          motorright.run(FORWARD);
          break;
          
          case 0xFF629D:   // increases speed
          Serial.println("+");
          if (speedint == 0) {  // if the speed is zero, it can just increase normally
            speedint += SPEED_INCREMENT;
            forwards_backwards = true;
          } else if (speedint < 250) { // 250 is the max speed, so having it go above that doesn't make any sense.
            if ( forwards_backwards == true) { // if the direction is forwards, it can also increase normally
              speedint += SPEED_INCREMENT;
            } else {                  // if the direction is backwards, it has to increase by decreasing the negative speed
              speedint -= SPEED_INCREMENT;
            }
          }
          Serial.println(speedint); // prints speed
          if (forwards_backwards == true) { // prints the direction
            Serial.println("forwards");
          } else {
            Serial.println("backwards");
          }
          motorright.setSpeed(speedint); // just sets the speed
          motorleft.setSpeed(speedint);
          if (speedint > 0) { // to move the boat in either direction
            if (forwards_backwards == true) { // to move the boat forwards
              motorright.run(BACKWARD); // since the fans are on opposite sides of the boat, these will be swapped
            motorleft.run(FORWARD);
            } else { // to move the boat backwards
              motorright.run(FORWARD); // like above, swapped
            motorleft.run(BACKWARD);
            }
          } else { // to stop the motors from spining
            motorright.run(RELEASE);
            motorleft.run(RELEASE);
          }
          break;
          
//          case 0xFFE21D:
//          Serial.println("F/St");
//          break;
          
          case 0xFF22DD:   // turns servo motor to the right to turn the boat to the left
          Serial.println("|<<");
          servo_position -= SERVO_INCREMENT;
          Serial.println(servo_position);
          Serial.println(servo_position);
          servomotor.write(servo_position);
          break;
          
//          case 0xFF02FD:
//          Serial.println(">|");
//          break;
  
          case 0xFFC23D:   // turns servo motor to the left to turn the boat to the right
          Serial.println(">>|");
          servo_position += SERVO_INCREMENT;
          Serial.println(servo_position);
          Serial.println(servo_position);
          servomotor.write(servo_position);
          break;
                             
//          case 0xFFE01F:
//          Serial.println("v");
//          break ; 
          case 0xFFA857:
          Serial.println("-"); // decreases speed
          speedint -= SPEED_INCREMENT;
          if (speedint == 0) {  // if the speed is zero, it can just decrease normally
            speedint -= SPEED_INCREMENT;
            forwards_backwards = true;
          } else if (speedint < 250) { // 250 is the max speed, so having it go above that doesn't make any sense.
            if ( forwards_backwards == true) { // if the direction is forwards, it has to decrease by lowering the positive speed
              speedint -= SPEED_INCREMENT;
            } else {                  // if the direction is backwards, it can also decrease normally 
              speedint += SPEED_INCREMENT;
            }
          }
            Serial.println(speedint); // prints speed
          if (forwards_backwards == true) { // prints the direction
            Serial.println("forwards");
          } else {
            Serial.println("backwards");
          }
          motorright.setSpeed(speedint); // just sets the speed
          motorleft.setSpeed(speedint);
          if (speedint > 0) { // to move the boat in either direction
            if (forwards_backwards == true) { // to move the boat forwards
              motorright.run(BACKWARD); // since the fans are on opposite sides of the boat, these will be swapped
            motorleft.run(FORWARD);
            } else { // to move the boat backwards
              motorright.run(FORWARD); // like above, swapped
            motorleft.run(BACKWARD);
            }
          } else { // to stop the motors from spining
            motorright.run(RELEASE);
            motorleft.run(RELEASE);
          }
          break;
          
//          case 0xFF906F:
//          Serial.println("^");
//          break ;  
//          case 0xFF6897:
//          Serial.println("0");
//          break ;  
//          case 0xFF9867:
//          Serial.println("1EQ");
//          break ;
//          case 0xFFB04F:
//          Serial.println("St/Rept");
//          break ;
//          case 0xFF30CF:
//          Serial.println("1");
//          break ;
//          case 0xFF18E7:
//          Serial.println("2");
//          break ;
//          case 0xFF7A85:
//          Serial.println("3");
//          break ;
//          case 0xFF10EF:
//          Serial.println("4");
//          break ;
//          case 0xFF38C7:
//          Serial.println("5");
//          break ;
//          case 0xFF5AA5:
//          Serial.println("6");
//          break ;
//          case 0xFF42BD:
//          Serial.println("7");
//          break ;
//          case 0xFF4AB5:
//          Serial.println("8");
//          break ;
//          case 0xFF52AD:
//          Serial.println("9");
//          break ;
          default:
          Serial.println(results.value); // not nessesary, but it's just a check      
        }
        key_value = results.value;
        irrecv.resume(); 
  }

}
