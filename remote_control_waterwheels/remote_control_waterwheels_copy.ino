#include <IRremote.h> // library for remote

#include <AFMotor.h> // library for DC motors
#include <Servo.h> // library for servo motors

AF_DCMotor motorleft(1); //DC motor setup
AF_DCMotor motorright(2);
int speedint = 0;
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
          speedint += SPEED_INCREMENT;
          Serial.println(speedint);
          if (speedint >= 0) { // if-else function sets speed of both fans
            motorright.setSpeed(speedint);
            motorleft.setSpeed(speedint);
          } else {
            motorright.setSpeed(abs(speedint));
            motorleft.setSpeed(abs(speedint));
          }
          
          if (speedint > 0) { // to move the boat forward
            motorright.run(BACKWARD); // since the fans are on opposite sides of the boat, these will be swapped
            motorleft.run(FORWARD);
          } else if (speedint = 0) { // to release the fans
            motorright.run(RELEASE);
            motorleft.run(RELEASE);
          } else { // to move the boat backwards
            motorright.run(FORWARD); // like above, swapped
            motorleft.run(BACKWARD);
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
          Serial.println(speedint);
          if (speedint >= 0) { // if-else function sets speed of both fans
            motorright.setSpeed(speedint);
            motorleft.setSpeed(speedint);
          } else {
            motorright.setSpeed(abs(speedint));
            motorleft.setSpeed(abs(speedint));
          }
          
          if (speedint > 0) { // to move the boat forwards
            motorright.run(BACKWARD); // since the fans are on opposite sides of the boat, these will be swapped
            motorleft.run(FORWARD);
          } else if (speedint = 0) { // to release the fans
            motorright.run(RELEASE);
            motorleft.run(RELEASE);
          } else { // to move the boat backwards
            motorright.run(FORWARD); // like above, swapped
            motorleft.run(BACKWARD);
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
