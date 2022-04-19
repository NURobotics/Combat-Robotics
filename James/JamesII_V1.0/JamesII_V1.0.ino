#include <ESP32Servo.h>
#include <analogWrite.h>
//#include <tone.h>
#include <ESP32Tone.h>
#include <ESP32PWM.h>

#include <PS4Controller.h>
//#include <Servo.h>


static const int MRPin = A0; // Right motor PWM (speed)
static const int MLPin = A4; // Left motor PWM (speed)

Servo rightMotor;
Servo leftMotor;

void receivePacket() { 

  int L_motor, R_motor;

//   if (PS4.event.button_down.up){
//    L_mapped++;
//    R_mapped++;
//    delay(100);
//   }
//   if (PS4.event.button_down.down){
//    L_mapped--;
//    R_mapped--;
//    delay(100);
//   }
  int L_stick, R_stick;

  if (PS4.data.button.r1){
    Serial.println("R1 Pressed!");
    L_motor = 1575;
    R_motor = 1575;
  }
  else if(PS4.data.button.l1){
    Serial.println("L1 Pressed!");
    L_motor = 1425;
    R_motor = 1425;
  }
  else {
    L_stick = PS4.data.analog.stick.ly;
    R_stick = PS4.data.analog.stick.ly;
    
    L_motor = map(L_stick, 127, -128, 1650, 1350);
    R_motor = map(R_stick, 127, -128, 1350, 1650);

    Serial.print("Stick = ");
    Serial.print(L_stick, DEC);

    Serial.print("  ->  PULSE: ");
    Serial.println(R_motor, DEC);
  }
 
  leftMotor.writeMicroseconds(L_motor);
  rightMotor.writeMicroseconds(R_motor);
  
}

void controllerConnect() {
  Serial.println("Connected!.");
}

void setup() {
  Serial.begin(115200);
  rightMotor.attach(MRPin); // 1, 0, 180, 1000, 2000);
  leftMotor.attach(MLPin);  //, 2, 0, 180, 1000, 2000);
  
  //PS4.attach(receivePacket);
  
  PS4.attachOnConnect(controllerConnect);
  //PS4.begin("55:b2:ec:3a:45:ca");
  PS4.begin("01:02:03:04:05:07");
}

void loop() {
  if(PS4.isConnected()){
    receivePacket();
  }
}
