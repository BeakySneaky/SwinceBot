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
#include <pk04_library.h>

/* ****************************************************************************
Variables globales et defines
**************************************************************************** */
// -> defines...
// L'ensemble des fonctions y ont acces



/* ****************************************************************************
Vos propres fonctions sont creees ici
**************************************************************************** */



/* ****************************************************************************
Fonctions d'initialisation (setup)
**************************************************************************** */
// -> Se fait appeler au debut du programme
// -> Se fait appeler seulement un fois
// -> Generalement on y initilise les varibbles globales

void setup(){
  BoardInit();

    pinMode(redpin, OUTPUT);
    pinMode(greenpin, OUTPUT);
    pinMode(bluepin, OUTPUT);
    pinMode(ledr, OUTPUT);
    pinMode(ledb, OUTPUT);
    pinMode(ledj, OUTPUT);
    pinMode(LedVerte, OUTPUT);
    pinMode(mic, INPUT);
    pinMode(TEST1, INPUT);//Droite
    pinMode(TEST2, INPUT);//Milieu
    pinMode(TEST3, INPUT);//Gauche
    pinMode(cote, INPUT);//cote
    SERVO_SetAngle(0,165);


}

/* ****************************************************************************
Fonctions de boucle infini (loop())
**************************************************************************** */
// -> Se fait appeler perpetuellement suite au "setup"

void loop() {
  /*Serial.print(digitalRead(cote));
  Serial.print("\n");
  delay (1000);*/
 float vitesseG = VITESSE;
  float vitesseD = VITESSE;
  int flag;
  int enc = 0; 
  int scantest = 0;
 // Serial.print(analogRead(mic));
 
scantest = scan();
//Serial.print(scantest);
while(sifflet() == 0 )
{
  suiveurDeLigne(&vitesseG, &vitesseD);
  
}

while(Find() == 0)
{
suiveurDeLigne(&vitesseG, &vitesseD);
delay(10);
}
MOTOR_SetSpeed(0,0);
MOTOR_SetSpeed(1,0);
delay(10);
frappe();
ENCODER_Reset(0);
while(enc<6000)
{
enc=ENCODER_Read(0);
  suiveurDeLigne(&vitesseG, &vitesseD);
}
while(analogRead(cote) > scantest){
suiveurDeLigne(&vitesseG, &vitesseD);
}
Tourner(-85, .20);
Avancer(5, .4);
while(1)
{
suiveurDeLigne(&vitesseG, &vitesseD);
if ((digitalRead(TEST2) == 0) && (digitalRead(TEST1) == 0) && (digitalRead(TEST3) == 0))
{
  break;
}
}
MOTOR_SetSpeed(0,0);
MOTOR_SetSpeed(1,0);
delay(100);
Avancer(15, 0.4);
delay(100);
flag = couleur();
delay(250);
Avancer(20, 0.4);
CatchNRelease(0);
DragNDropBall(flag);
RetourStart(flag);
suiveurDeLigne(&vitesseG, &vitesseD);
/*Serial.print(analogRead(cote));
Serial.print("\n");
delay(500);*/
}