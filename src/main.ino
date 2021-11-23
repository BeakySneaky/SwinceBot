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
    SERVO_Enable(0);
    SERVO_SetAngle(0,75);
  Serial2.begin(9600);
  Serial.println("Test du ID-12 sur UART2 (RX2/Digital 17)");
SERVO_Disable(0);
}

/* ****************************************************************************
Fonctions de boucle infini (loop())
**************************************************************************** */
// -> Se fait appeler perpetuellement suite au "setup"

void loop() {
 
 // rfid();
  delay(15000);
  swince();
  delay(2000);
  SERVO_Disable(0);
  exit(0);
}