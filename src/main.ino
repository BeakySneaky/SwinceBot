/*
Projet: Le nom du script
Equipe: Votre numero d'equipe
Auteurs: Les membres auteurs du script
Description: Breve description du script
Date: Derniere date de modification
*/

/* ****************************************************************************
Inclure les librairies de functions que vous voulez utiliser
**************************************************************************** */

#include <LibRobus.h> // Essentielle pour utiliser RobUS


/* ****************************************************************************
Variables globales et defines
**************************************************************************** */
// -> defines...
// L'ensemble des fonctions y ont acces

bool maxDistance;
double maxSpeed;
float speed, pCorrection;
int cycle, pulseCounterMaster, pulseCounterSlave, distanceMotor0, distanceMotor1;

/* ****************************************************************************
Vos propres fonctions sont creees ici
**************************************************************************** */

//Makes the robot move forward.
void Forward(int distance){

  float distanceConstant = distance * 133.673443;
  
  maxDistance = false;
  maxSpeed = 0.4;
  pulseCounterMaster = 0;
  pulseCounterSlave = speed = cycle = 0;
  
  while(!maxDistance){ 

    distanceMotor0 = ENCODER_Read(0);
    distanceMotor1 = ENCODER_Read(1);
    pulseCounterMaster += distanceMotor0;

    if(pulseCounterMaster > distanceConstant){
      MOTOR_SetSpeed(0,0);
      MOTOR_SetSpeed(1,0);
      maxDistance = true;
    }
    else {
      
      //Acceleration and decceleration condition

      if(pulseCounterMaster > 0.7 * distanceConstant && speed > 0.2){
        if(distance > 80){
           speed -= 0.0016;
           pCorrection -= 0.0016;
        }
        else{
          pCorrection = speed -= 0.015;
        }
      }
      else if (speed < maxSpeed){
         pCorrection = speed += 0.05;
      }
      else
      {
        cycle++;
        speed = maxSpeed;  
        pulseCounterSlave += distanceMotor1;
        pCorrection = speed + PID(distanceMotor1, distanceMotor0, pulseCounterSlave, cycle);
      }
      

      MOTOR_SetSpeed(0,speed);
      MOTOR_SetSpeed(1,pCorrection);
    }
      //Read pulsation graph with serial plotter & reset.
      ENCODER_ReadReset(0);
      ENCODER_ReadReset(1);
      delay(10);

    }
}

//Makes the robot turn in fonction of the angle.
//If the angle is negative the robot will turn to the left.
void Turn(int angle){

  int id = 0;
  int distanceMotor;
  float angleConstant = 44.2;
  //44.2 pour A4
  speed = 0;
  maxSpeed = 0.4;
  maxDistance = false;
 
  if(angle < 0){
    id = 1;
    angle = angle * -1;
  }

  //Seulement pour les moteurs bizarre de B4
  if(id == 1){
    angleConstant = 42.6;
  }
  
  while(!maxDistance){
    
    distanceMotor =  ENCODER_Read(id);

      if(distanceMotor > angle * angleConstant){
        MOTOR_SetSpeed(id,0);
        maxDistance = true;
      }
      else {

       //Acceleration and decceleration condition
      if(distanceMotor > 0.5 * (angle * angleConstant) && speed > 0.2){
        speed -= 0.005;

      }
      else if (speed < maxSpeed){
        speed += 0.005;
      }

        MOTOR_SetSpeed(id,speed);
      }
      
      delay(10);
  }
  ENCODER_Reset(id);
}

void Full180(){

  int distanceMotor;
  float angleConstant = 44.2;

  int angle = 90;
  //85 pour A4
  speed = 0;
  maxSpeed = 0.3;
  maxDistance = false;
  
  while(!maxDistance){
    
    distanceMotor =  ENCODER_Read(0);

    if(distanceMotor > angle * angleConstant){
      MOTOR_SetSpeed(0,0);
      MOTOR_SetSpeed(1,0);
      maxDistance = true;
      delay(200);

    }
    else {
      if(distanceMotor > 0.5 * (angle * angleConstant) && speed > 0.2){
        speed -= 0.005;

      }
      else if (speed < maxSpeed){
        speed += 0.005;
      }

        MOTOR_SetSpeed(0,speed);
        MOTOR_SetSpeed(1,speed*-1);

      }
      
      delay(10);
  }
  ENCODER_Reset(0);
  ENCODER_Reset(1);
}

//Pondarion part of the PID.
float Ponderation(int readPulse, int desiredPulse)
{
  float kP = 0.0001;
  int error = desiredPulse - readPulse;
  return error*kP;
}

//Integration part of the PID
float Integration(int desiredPulse, int counter, int cycles){
  //0.000008 pour A4;
  float kI = -0.000004;
  int correction = (cycles*desiredPulse - counter);
  return correction*kI;
}

//PID Function
float PID(int readPulse, int desiredPulse, int counter, int cycles){
  return Ponderation(readPulse, desiredPulse) + Integration(desiredPulse, counter, cycles);
}

/* ****************************************************************************
Fonctions d'initialisation (setup)
**************************************************************************** */
// -> Se fait appeler au debut du programme
// -> Se fait appeler seulement un fois
// -> Generalement on y initilise les varibbles globales

void setup(){
  BoardInit();
}

/* ****************************************************************************
Fonctions de boucle infini (loop())
**************************************************************************** */
// -> Se fait appeler perpetuellement suite au "setup"

void loop() {

  //Array of all movements with fine tuned values.
  int movementArray1[2][9] = {{200,25,25,25,15,35,55,35,70},{-90,90,90,-90,45,-90,45,15,0}};
  //Should get the number of rows for the first column.
  int arraySize = sizeof(movementArray1[0]) / sizeof(int);

  //Loops through the 2d array to get forward and turn values.
  //The first pair of brackets of the array is the columns, the second is the rows.
  for (int i = 0; i < arraySize; i++){
    Forward(movementArray1[0][i]);
    Turn(movementArray1[1][i]);
  }

  //180 turn:
  Full180();
  //first we reverse the turning angles. 
  for (size_t j = 0; j < arraySize; j++)
  {
    movementArray1[1][j] = movementArray1[1][j] * -1;
    Serial.println(movementArray1[1][j]);
  }
  //Then we make it go from the last distance to the first.
  for (int i = arraySize - 1; i >= 0; i--)
  {
    Turn(movementArray1[1][i]);
    Forward(movementArray1[0][i]);
  }

  exit(0);
}

//Hardcoded way
  /*
  Forward(215);
  
  Turn(-90);
  
  Forward(31);
  
  Turn(90);
  
  Forward(34);
  
  Turn(90);
  
  Forward(30);
  
  Turn(-90);
  
  Forward(18);
  
  Turn(45);
  
  Forward(38);
  
  Turn(-90);

  Forward(57);
  
  Turn(45);

  Forward(28);

  Turn(13);

  Forward(84);*/