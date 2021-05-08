#include "SerialReading.h"

#define DEFAULT_SPEED 5000

void setup() {
  Serial.begin(9600);
  pinMode(EN, OUTPUT);
  digitalWrite(EN, LOW);
  // Configure each stepper
  stepperX.setMaxSpeed(DEFAULT_SPEED);
  stepperY.setMaxSpeed(DEFAULT_SPEED);
  stepperZ.setMaxSpeed(DEFAULT_SPEED);
  stepperA.setMaxSpeed(DEFAULT_SPEED);
  // Then give them to MultiStepper to manage
  steppers.addStepper(stepperX);
  steppers.addStepper(stepperY);
  steppers.addStepper(stepperZ);
  steppers.addStepper(stepperA);
}
char moveLeft[10] = {'G',32,'L',32,'3','2','0','0',';','\0'};
  char moveUp[10] = {'G',32,'U',32,'3','2','0','0',';','\0'};
  char moveOut[10] = {'G',32,'O',32,'3','2','0','0',';','\0'};
  char moveRight[10] = {'G',32,'R',32,'3','2','0','0',';','\0'};
  char moveDown[10] = {'G',32,'D',32,'3','2','0','0',';','\0'};
  char moveIn[10] = {'G',32,'I',32,'3','2','0','0',';','\0'};
void loop() {
  //uncomment line below to control over Serial
//  controlRobot(moveLeft,0);

  //Demo Mode
  controlRobot(moveLeft,sizeof(moveLeft));
  delay(10);
  controlRobot(moveUp,sizeof(moveUp));
  delay(10);
  controlRobot(moveOut,sizeof(moveOut));
  delay(10);
  controlRobot(moveRight,sizeof(moveLeft));
  delay(10);
  controlRobot(moveDown,sizeof(moveDown));
  delay(10);
  controlRobot(moveIn,sizeof(moveIn));
  delay(10);
}
