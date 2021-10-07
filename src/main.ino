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



/* ****************************************************************************
Vos propres fonctions sont creees ici
**************************************************************************** */

//Makes the robot move forward.
void Forward(int distance){

  double distanceConstant = distance * 133.673443;
  double maxSpeed = 0.4;
  double speed = 0.4;
  int cycle = 0;
  int pulseCounter = 0;
  int distanceMotor0;
  int distanceMotor1;
  double pCorrection;
  bool maxDistance = false;
  
  while(!maxDistance){ 

    cycle++;  
    distanceMotor0 = ENCODER_Read(0);
    distanceMotor1 = ENCODER_Read(1);
    pulseCounter += distanceMotor0;
    
    if(pulseCounter >= distanceConstant){
      MOTOR_SetSpeed(0,0);
      MOTOR_SetSpeed(1,0);
      maxDistance = true;
      delay(200);
    }
    else {
      
      //Acceleration and decceleration condition
      /*if(pulseCounter <= 0.2 * distanceConstant && speed < maxSpeed){
        speed += 0.05;
      }
      else*/
      if (pulseCounter >= 0.8 * distanceConstant && speed > 0.2){
        speed = speed / 1.05;
        pCorrection = speed;
      }
      else{
        pCorrection = Ponderation(distanceMotor1, distanceMotor0, pulseCounter, cycle, speed);
      }

      //Serial.println(distanceMotor0);
      MOTOR_SetSpeed(0,speed);
      MOTOR_SetSpeed(1,pCorrection);
    }
      //Read pulsation graph with serial plotter.
      ENCODER_Reset(0);
      ENCODER_Reset(1);
      delay(20);
  }
}

//Makes the robot turn in fonction of the angle.
//If the angle is negative the robot will turn to the left.
void Turn(int angle){
  int distanceMotor;
  int id = 0;
  double speed = 0.2;
  double maxSpeed = 0.4;
  int angleConstant = 44.4;
  bool maxDistance = false;
 
  if(angle < 0){
    id = 1;
    angle = angle * -1;
  }
  
  while(!maxDistance){
    distanceMotor =  ENCODER_Read(id);
      
      if(distanceMotor > angle * angleConstant){
        MOTOR_SetSpeed(id,0);
        maxDistance = true;
        delay(200);
      }
      else {

        //Acceleration and decceleration condition
      /*if(distanceMotor <= 0.3 * angleConstant && speed < maxSpeed){
        speed += 0.1;
      }
      else if (distanceMotor >= 0.7 * angleConstant && speed > 0.2){
        speed = speed / 1.1;
      }*/

        MOTOR_SetSpeed(id,maxSpeed);
      }
  }
  
  ENCODER_Reset(0);
  ENCODER_Reset(1);
}

//Pondarion part of the PID.
//Will adjust the pulse of motor 1 according to motor 0. 
//(error + correction to get desired speed)
double Ponderation(int readPulse, int desiredPulse, int counter, int cycles, double speed)
{
  double kP = 0.0001;
  double kI = 0.00002;
  double error = desiredPulse - readPulse;
  double correction = (cycles*desiredPulse - counter);

  return speed + ((error*kP) + (correction*kI));
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
  int movementArray1[2][9] = {{215,34,28,30,18,38,57,28,84},{-90,90,90,-90,45,-90,45,13,0}};
  //Should get the number of rows for the first column.
  int arraySize = sizeof(movementArray1[0]) / sizeof(int);

  //Loops through the 2d array to get forward and turn values.
  //The first pair of brackets of the array is the columns, the second is the rows.
  /*for (size_t i = 0; i < arraySize; i++){
    Forward(movementArray1[0][i]);
    Turn(movementArray1[1][i]);
  }*/

  Forward(100);
  Turn(180);
  Forward(100);

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

  exit(0);
}