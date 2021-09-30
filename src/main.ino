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
void Forward(int distance){
  int distanceMotor0;
  int distanceMotor1;
  int distanceConstant = 133.673443;
  bool maxDistance = false;
  
  while(!maxDistance){    
    distanceMotor0 = ENCODER_Read(0);
    distanceMotor1 = ENCODER_Read(1);
    
    if(distanceMotor0 >= distance * distanceConstant){
      MOTOR_SetSpeed(0,0);
      MOTOR_SetSpeed(1,0);
      maxDistance = true;
      delay(200);
    }
    else if(distanceMotor1 > distanceMotor0){
      MOTOR_SetSpeed(1, 0.30);
    }
    else {
      MOTOR_SetSpeed(0,0.4);
      MOTOR_SetSpeed(1,0.4);
    }
  }
  ENCODER_Reset(0);
  ENCODER_Reset(1);
}

// id 0 = right, id 1 = left.
void Turn(int id, int angle){
  int distanceMotor;
  int angleConstant = 44.4;
  bool distanceMax = false;
  
  while(!distanceMax){
    distanceMotor =  ENCODER_Read(id);
      
      if(distanceMotor > angle * angleConstant){
        MOTOR_SetSpeed(id,0);
        distanceMax = true;
        delay(200);
      }
      else {
        MOTOR_SetSpeed(id,0.3);
      }
  }
  ENCODER_Reset(0);
  ENCODER_Reset(1);

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

  int left = 1;
  int right = 0;

  //int movementArray[9][9] = {{215,34,28,30,18,38,57,28,84},{-90,90,90,-90,45,-90,45,13}};

  
  Forward(215);
  
  Turn(left, 90);
  
  Forward(34);
  
  Turn(right, 90);
  
  Forward(28);
  
  Turn(right,90);
  
  Forward(30);
  
  Turn(left, 90);
  
  Forward(18);
  
  Turn(right,45);
  
  Forward(38);
  
  Turn(left, 90);

  Forward(57);
  
  Turn(right, 45);

  Forward(28);

  Turn(right,13);

  Forward(84);

  exit(0);
}