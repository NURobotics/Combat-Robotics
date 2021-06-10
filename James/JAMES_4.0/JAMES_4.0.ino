
#include <PS3BT.h>
#include <usbhub.h>
#include <Servo.h>

#ifdef dobogusinclude
#include <spi4teensy3.h>
#endif
#include <SPI.h>

USB Usb;
BTD Btd(&Usb);

//PS3BT PS3(&Btd);
//04:76:6E:1A:C2:D6

PS3BT PS3(&Btd, 0x04, 0x76, 0x6E, 0x1A, 0xC2, 0xD6);

Servo rightDrive;
Servo leftDrive;
Servo weapon;

int rightDrivePin = 4;
int leftDrivePin = 5;
int weaponPin = 3;

float rightDriveTarget = 90;
float rightDriveSpeed = 90;
float rightDriveChange = 0;

float leftDriveSpeed = 90;
float leftDriveTarget = 90;
float leftDriveChange = 0;

float weaponTarget = 0;
float weaponSpeed = 0;
float weaponMultiplier = 0.8;

bool printTemperature, printAngle;
void setup() {
  pinMode(rightDrivePin, OUTPUT);
  pinMode(leftDrivePin, OUTPUT);
  pinMode(weaponPin, OUTPUT);

  rightDrive.attach(rightDrivePin, 1000, 2000);
  leftDrive.attach(leftDrivePin, 1000, 2000);
  weapon.attach(weaponPin, 1000, 2000);

  rightDrive.write(90);
  leftDrive.write(90);
  weapon.write(0);

  Serial.begin(115200);
#if !defined(__MIPSEL__)
  while (!Serial);
#endif
  if (Usb.Init() == -1) {
    while (1); //halt
  }
}

void loop() {
  Usb.Task();
  Usb.Task();

  if (PS3.PS3Connected) {
    leftDriveTarget = (255-PS3.getAnalogHat(LeftHatY))/127.5 - 1;
    rightDriveTarget = (255 - PS3.getAnalogHat(RightHatY))/127.5 -1;
    weaponSpeed = (PS3.getAnalogButton(R2))/1.4166667;
    
    if (PS3.getAnalogHat(LeftHatY) > 137 || PS3.getAnalogHat(LeftHatY) < 117 || PS3.getAnalogHat(RightHatY) > 137 || PS3.getAnalogHat(RightHatY) < 117){
      leftDriveTarget = leftDriveTarget * abs(leftDriveTarget * leftDriveTarget);
      leftDriveTarget = (leftDriveTarget + 1) * 90;
  
      leftDriveChange = leftDriveTarget - leftDriveSpeed;
      
      leftDriveChange = min(leftDriveChange, 1);
      leftDriveChange = max(leftDriveChange, -1);
      
      leftDriveSpeed = leftDriveSpeed + leftDriveChange;

      rightDriveTarget = rightDriveTarget * abs(rightDriveTarget * rightDriveTarget);
      rightDriveTarget = (rightDriveTarget + 1) * 90;
    
      rightDriveChange = rightDriveTarget - rightDriveSpeed;
      
      rightDriveChange = min(rightDriveChange, 1);
      rightDriveChange = max(rightDriveChange, -1);
      rightDriveSpeed = rightDriveSpeed + rightDriveChange;
      
      leftDrive.write(leftDriveSpeed);
      rightDrive.write(rightDriveSpeed); 
    }
    else{
      leftDrive.write(90);
      rightDrive.write(90);
    }

    weaponSpeed = weaponSpeed * weaponMultiplier;
    weapon.write(weaponSpeed);    

    if (PS3.getButtonClick(PS)) {
      Serial.print(F("\r\nPS"));
      weapon.write(0);
      rightDrive.write(90);
      leftDrive.write(90);
      PS3.disconnect();
    }
  }
  else{
    weapon.write(0);
    rightDrive.write(90);
    leftDrive.write(90);
  }
}
