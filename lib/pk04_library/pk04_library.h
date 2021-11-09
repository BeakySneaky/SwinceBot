#ifndef PK04_LIBRARY_H
#define PK04_LIBRARY_H
//#include <Adafruit_TCS34725.h>

#include <Arduino.h>
#include <LibRobus.h>
#include <Wire.h>
// -> defines...
// L'ensemble des fonctions y ont acces
//#define circonference 24.2496; // en cm (un tour-->3200)
#define VITESSE 0.20
#define MODIF_VITESSE 0.01
#define VITESSE_MAX 0.8
#define ERREUR -1

#define TEST1 40
#define TEST2 38
#define TEST3 39
#define redpin 3
#define greenpin 5
#define bluepin 6

#define ledr 46 
#define ledb 48 
#define ledj 42 
#define LedVerte 44
#define cote 41
#define mic A13
#define commonAnode true

//Définir vos fonctions ici.
//Adafruit_TCS34725 tcs = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_50MS, TCS34725_GAIN_4X);

//Avancer
void Avancer(float distance_voulue, float speed);

//Tourner
void Tourner(int angle, float vit);

//Tour180
void Tour180();

//PID
float pid(int nb_pulse_droite, int nb_pulse_gauche, float vitesse_droite);

//Fonction pour le servo
void CatchNRelease(bool release);

int Find();

void frappe();

void DragNDropBall(int color);

void yeet();
// retourner au start
void RetourStart(int color);

int sifflet();

int couleur();

int capteurLigneCase(bool c, bool d, bool g);

void reglerVitesse(int cas, float *vGauche, float *vDroite);

void suiveurDeLigne(float *vitesseG, float *vitesseD);

#endif