#ifndef SERIAL_READING
#define SERIAL_READING

#include <AccelStepper.h>
#include <MultiStepper.h>

//define serial reading vars
const int BUFFER_SIZE = 100;
int rlen;
char buf[BUFFER_SIZE];
char targetMotor;
char command;
char motorCommand;
bool listenToSerial = true;

// define stepper pins
#define EN        8           //The step motor makes the power end, low level is effective
#define X_DIR     5           //The x-axis moves in the direction of the motor
#define Y_DIR     6           //The y-axis moves in the direction of the motor
#define Z_DIR     7           //The z-axis moves in the direction of the motor
#define A_DIR     13          //The A-axis moves in the direction of the motor
#define X_STP     2           //The x axis stepper control
#define Y_STP     3           //The y axis stepper control
#define Z_STP     4           //The z axis stepper control
#define A_STP     12          //The A axis stepper control
#define motorInterfaceType 1  //1 for using a driver

//initilize steppers
AccelStepper stepperX(motorInterfaceType, X_STP, X_DIR);
AccelStepper stepperY(motorInterfaceType, Y_STP, Y_DIR);
AccelStepper stepperZ(motorInterfaceType, Z_STP, Z_DIR);
AccelStepper stepperA(motorInterfaceType, A_STP, A_DIR);
MultiStepper steppers;

//initilize stepper vars
long positions[4] = {0, 0, 0, 0};
/* Index labeling
 * X - 0
 * Y - 1
 * Z - 2
 * A - 3
 */
int extractMotorCommandData(int _startIndex, char endChar){ 
  char dataInput[BUFFER_SIZE];
  int data;
  if(rlen > 5){
    int startIndex = _startIndex;
    char currChar = buf[startIndex];
    int currIndex = startIndex;
    while (currChar != endChar && currIndex < rlen){
      currChar = buf[currIndex];
      dataInput[currIndex - _startIndex] = currChar;
      currIndex ++;
    }
    data = atoi(dataInput);
  }else{
    data = 0;
  }
  return data;
}

int findIndexOfChar(int numberToReport, char targetChar){
    int numCharFound = 0;
    int currIndex = 0;
    char currChar = buf[currIndex];
    while (numCharFound != numberToReport && currIndex < rlen && currChar != '\n'){
      currChar = buf[currIndex];
      if(currChar == targetChar){
        numCharFound += 1;
      }
      currIndex ++;
    }
    return currIndex;
}

void moveAllSteppers(int XData, int YData, int ZData, int AData){
    positions[0] = positions[0] + XData;
    positions[1] = positions[1] + YData;
    positions[2] = positions[2] + ZData;
    positions[3] = positions[3] + AData;
    steppers.moveTo(positions);
    steppers.runSpeedToPosition(); // Blocks until all are in position
    Serial.print("Motor X moving:\t"); Serial.print(XData); Serial.print("\tNew X Position:\t"); Serial.println(positions[0]);
    Serial.print("Motor Y moving:\t"); Serial.print(YData); Serial.print("\tNew Y Position:\t"); Serial.println(positions[1]);
    Serial.print("Motor Z moving:\t"); Serial.print(ZData); Serial.print("\tNew Z Position:\t"); Serial.println(positions[2]);
    Serial.print("Motor A moving:\t"); Serial.print(AData); Serial.print("\tNew A Position:\t"); Serial.println(positions[3]);
}

void serialRead(){
  if (Serial.available() > 0) {
    // read the incoming bytes:
    rlen = Serial.readBytes(buf, BUFFER_SIZE);
  }
}

void setSerialReadBuffer(char input[], int inputSize){
  rlen = inputSize;
  for (int i = 0; i < inputSize; i++){
    buf[i] = input[i];
  }
  delay(50);
}

bool controlRobot(char input[], int inputSize){
    //override Serial? 
    if (inputSize > 0){
      Serial.println("Not Reading Serial");
      setSerialReadBuffer(input, inputSize);
    }else{
      serialRead();
    }
    
    command = buf[0];
    int data;
    switch(command){
      case 'X':
        Serial.print("Motor X ");
        data = extractMotorCommandData(4, '\n');
        motorCommand = buf[2];
        if(motorCommand == 'P'){
          positions[0] = positions[0] + data;
          Serial.print("moving: "); Serial.print(data); Serial.print(" Steps\n");
          Serial.print("New X Position: "); Serial.print(positions[0]); Serial.print("\n");
        }else if(motorCommand == 'S'){
          stepperX.setMaxSpeed(data);
          Serial.print("speed set to "); Serial.print(data); Serial.print(" \n");
        }
        steppers.moveTo(positions);
        steppers.runSpeedToPosition(); // Blocks until all are in position
      break;
      case 'Y':
        Serial.print("Motor Y ");
        data = extractMotorCommandData(4, '\n');
        motorCommand = buf[2];
        if(motorCommand == 'P'){
          positions[1] = positions[1] + data;
          Serial.print("moving: "); Serial.print(data); Serial.print(" Steps\n");
          Serial.print("New Y Position: "); Serial.print(positions[1]); Serial.print("\n");
        }else if(motorCommand == 'S'){
          stepperY.setMaxSpeed(data);
          Serial.print("speed set to "); Serial.print(data); Serial.print(" \n");
        }
        steppers.moveTo(positions);
        steppers.runSpeedToPosition(); // Blocks until all are in position
      break;
      case 'Z':
        Serial.print("Motor Z ");
        data = extractMotorCommandData(4, '\n');
        motorCommand = buf[2];
        if(motorCommand == 'P'){
          positions[2] = positions[2] + data;
          Serial.print("moving: "); Serial.print(data); Serial.print(" Steps\n");
          Serial.print("New Z Position: "); Serial.print(positions[2]); Serial.print("\n");
        }else if(motorCommand == 'S'){
          stepperZ.setMaxSpeed(data);
          Serial.print("speed set to "); Serial.print(data); Serial.print(" \n");
        }
        steppers.moveTo(positions);
        steppers.runSpeedToPosition(); // Blocks until all are in position
      break;
      case 'A':
        Serial.print("Motor A ");
        data = extractMotorCommandData(4, '\n');
        motorCommand = buf[2];
        if(motorCommand == 'P'){
          positions[3] = positions[3] + data;
          Serial.print("moving: "); Serial.print(data); Serial.print(" Steps\n");
          Serial.print("New A Position: "); Serial.print(positions[3]); Serial.print("\n");
        }else if(motorCommand == 'S'){
          stepperA.setMaxSpeed(data);
          Serial.print("speed set to "); Serial.print(data); Serial.print(" \n");
        }
        steppers.moveTo(positions);
        steppers.runSpeedToPosition(); // Blocks until all are in position
      break;
      case '?':
        Serial.println("SINGLE MOTOR COMMANDS");
        Serial.println("\tX [P:S] [Data] - Motor X command");
        Serial.println("\tY [P:S] [Data] - Motor Y command");
        Serial.println("\tZ [P:S] [Data] - Motor Z command");
        Serial.println("\tA [P:S] [Data] - Motor A command");
        Serial.println("\tPERAMERTERS:\n\tP - Set Target Position \n\tS - Set Max Speed \n\tData - int for the new value of P/S\n");
        Serial.println("MULTI MOTOR COMMANDS");
        Serial.println("\tM :[X Data]:[Y Data]:[Z Data]:[A Data]: - Move ALL motors needs all data every time and ':'");
        Serial.println("\tPERAMERTERS:\n\tData - int for the amount for each motor to move\n");
        Serial.println("\tG [Dir] [Data]; - Group Move (Move the end effector)\n\tPERAMERTERS:\n\tDir - Direction to move\n\t\tU - Up\n\t\tD - Down\n\t\tL - Left\n\t\tR - Right\n\t\tI - In\n\t\tO - Out\n\tData - int for the amount for each motor to move\n");
        Serial.println("\tS [Data]; = Set speed for all motors\n\tPERAMERTERS:\n\tData - int for the speed\n");
      break;
      case 'S':
        data = extractMotorCommandData(2, ';');
        Serial.print("Set all motors to:\t"); Serial.println(data);
        stepperX.setMaxSpeed(data);
        stepperY.setMaxSpeed(data);
        stepperZ.setMaxSpeed(data);
        stepperA.setMaxSpeed(data);
      break;
      case 'G':
        data = extractMotorCommandData(4, ';');
        char moveDir = buf[2];
        Serial.print("Group Move:\t"); Serial.print(moveDir);Serial.print("\t"); Serial.println(data);
        switch(moveDir){
          case 'U':
            moveAllSteppers(0,0,data,0);
          break;
          case 'D':
            moveAllSteppers(0,0,-data,0);
          break;
          case 'L':
            moveAllSteppers(-data,-data,-data,0);
          break;
          case 'R':
            moveAllSteppers(data,data,data,0);
          break;
          case 'I':
            moveAllSteppers(data,-data,-data,0);
          break;
          case 'O':
            moveAllSteppers(-data,data,data,0);
          break;
        }
        Serial.print('\n');
      break;
      case 'M':
        Serial.println("Moving all steppers");
        int XData = extractMotorCommandData(findIndexOfChar(1,':'), ':');
        int YData = extractMotorCommandData(findIndexOfChar(2,':'), ':');
        int ZData = extractMotorCommandData(findIndexOfChar(3,':'), ':');
        int AData = extractMotorCommandData(findIndexOfChar(4,':'), ':');
        moveAllSteppers(XData,YData,ZData,AData);
      break;
      case '*':
      default:
      Serial.println("Unknown command try '?'");
      break;
    }
    buf[0] = '\0';
    return true;
  }

//void serialRead() {
//  if (Serial.available() > 0) {
//    // read the incoming bytes:
//    rlen = Serial.readBytes(buf, BUFFER_SIZE);
//    targetMotor = buf[0];
//    if(rlen <= 5 && targetMotor != '?'){
//      targetMotor = '*';
//    }
//    if(rlen > 5){
//      motorCommand = buf[2];
//      int currIndex = 4;
//      char currChar = buf[currIndex];
//      while (currChar != '\n' && currIndex < rlen){
//        dataInput[currIndex - 5] = currChar;
//        currChar = buf[currIndex];
//        currIndex ++;
//      }
//      data = atoi(dataInput);
//    }
//    
//    switch(targetMotor){
//      case 'X':
//        if(motorCommand == 'P'){
//          positions[0] = positions[0] + data;
//          Serial.print("Motor X moving: "); Serial.print(data); Serial.print(" Steps\n");
//          Serial.print("New X Position: "); Serial.print(positions[0]); Serial.print("\n");
//        }else if(motorCommand == 'S'){
//          stepperX.setMaxSpeed(data);
//          Serial.print("Motor X speed set to "); Serial.print(data); Serial.print(" \n");
//        }
//      break;
//      case 'Y':
//        if(motorCommand == 'P'){
//          positions[1] = positions[1] + data;
//          Serial.print("Motor Y moving: "); Serial.print(data); Serial.print(" Steps\n");
//          Serial.print("New Y Position: "); Serial.print(positions[1]); Serial.print("\n");
//        }else if(motorCommand == 'S'){
//          stepperY.setMaxSpeed(data);
//          Serial.print("Motor Y speed set to "); Serial.print(data); Serial.print(" \n");
//        }
//      break;
//      case 'Z':
//        if(motorCommand == 'P'){
//          positions[2] = positions[2] + data;
//          Serial.print("Motor Z moving: "); Serial.print(data); Serial.print(" Steps\n");
//          Serial.print("New Z Position: "); Serial.print(positions[2]); Serial.print("\n");
//        }else if(motorCommand == 'S'){
//          stepperZ.setMaxSpeed(data);
//          Serial.print("Motor Z speed set to "); Serial.print(data); Serial.print(" \n");
//        }
//      break;
//      case 'A':
//        if(motorCommand == 'P'){
//          positions[3] = positions[3] + data;
//          Serial.print("Motor A moving: "); Serial.print(data); Serial.print(" Steps\n");
//          Serial.print("New A Position: "); Serial.print(positions[3]); Serial.print("\n");
//        }else if(motorCommand == 'S'){
//          stepperA.setMaxSpeed(data);
//          Serial.print("Motor A speed set to "); Serial.print(data); Serial.print(" \n");
//        }
//      break;
//      case '?':
//        Serial.println("X [P:S] [Data]");
//        Serial.println("Y [P:S] [Data]");
//        Serial.println("Z [P:S] [Data]");
//        Serial.println("A [P:S] [Data]");
//        Serial.println("\n PERAMERTERS:\nP - Set Target Position \nS - Set Max Speed \nData - int for the new value of P/S");
//      break;
//      case '*':
//      default:
//      Serial.println("Unknown command try '?'");
//      break;
//    }
//  
//    steppers.moveTo(positions);
//    steppers.runSpeedToPosition(); // Blocks until all are in position
//    Serial.println("DONE");
//  }
//}

#endif
