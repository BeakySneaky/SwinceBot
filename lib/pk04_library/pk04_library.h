#ifndef PK04_LIBRARY_H
#define PK04_LIBRARY_H

#include <Arduino.h>
#include <LibRobus.h>

//Définir vos fonctions ici.


//Avancer
void Avancer(float distance_voulue);

//Tourner
void Tourner(int angle);

//Tour180
void Tour180();

//PID
float pid(int nb_pulse_droite, int nb_pulse_gauche, float vitesse_droite);

//Fonction pour le servo
void CatchNRelease(bool release);

void FindNPushPin();

void DragNDropBall();

// retourner au start
void RetourStart();


int couleur();

#endif