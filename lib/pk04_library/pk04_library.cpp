#include <pk04_library.h>


// -> defines...
// L'ensemble des fonctions y ont acces
//#define circonference 24.2496; // en cm (un tour-->3200)
#define circonference 24.24; //A Croquette
const float roue = 18.9; //A Croquette
const float variation  = 0.001;
float speed = 0.45;
//Fix temporaire, émile sais quoi faire.
int LedVerte = 44;


//Implémentez vos fonction ici

//Fonctions de mouvement pour le robot avec  PID
void Avancer(float distance_voulue)
{
  float nb_pulse = (3200 * distance_voulue)/circonference;
  float distance = distance_voulue;
  float variationvit = 0.18;
  float vitesse = speed - variationvit;
  float vitesse_nouvelle = 0;
  int acc=0;
  int dec=0;

  MOTOR_SetSpeed(1, speed-variationvit);
  MOTOR_SetSpeed(0, speed-variationvit);

  while(nb_pulse >= ENCODER_Read(0))
  {
    /*Serial.print(ENCODER_Read(0));
    Serial.print("\t");
    Serial.print(ENCODER_Read(1));
    Serial.print("\n");
    Serial.print(vitesse ,35);
    Serial.print("\n");*/
        if(ENCODER_Read(0) > 0.08 * nb_pulse && acc==0){
      MOTOR_SetSpeed(0, speed);
      MOTOR_SetSpeed(1, vitesse+variationvit);
      vitesse = vitesse+variationvit;
      acc=1;
    } 
    if(ENCODER_Read(0) > 0.9 * nb_pulse && dec==0){
      MOTOR_SetSpeed(0, speed-variationvit);
      MOTOR_SetSpeed(1, vitesse-variationvit);
      vitesse = vitesse-variationvit;
      dec=1;
    }

    vitesse_nouvelle = pid(ENCODER_Read(1), ENCODER_Read(0), vitesse);
    vitesse = vitesse_nouvelle;
    delay(100); 
  }

  MOTOR_SetSpeed(1, 0);
  MOTOR_SetSpeed(0, 0);
  ENCODER_Reset(0);
  ENCODER_Reset(1);

}

void Tourner(int angle)
{
  float arc = angle*(3.1416*roue)/360;
  float nb_pulse = 3200 * arc/circonference;
    if(angle>0)
    {
      MOTOR_SetSpeed(0, 0.15);
      MOTOR_SetSpeed(1, -0.15);

      while(nb_pulse >= ENCODER_Read(0) && nb_pulse >= -ENCODER_Read(1)) {
        delay(10); 
      }
      if(nb_pulse<=ENCODER_Read(0)){
          MOTOR_SetSpeed(0, 0);
          MOTOR_SetSpeed(1,-0.1);
          while(nb_pulse>=-ENCODER_Read(1));
          MOTOR_SetSpeed(1,0);
      } 
      else if(nb_pulse<=-ENCODER_Read(1)){
          MOTOR_SetSpeed(1, 0);
          MOTOR_SetSpeed(0,0.1);
          while(nb_pulse>=ENCODER_Read(0));
          MOTOR_SetSpeed(0,0);
      } 
    }
    else if(angle<0)
    {
      MOTOR_SetSpeed(0, -0.15);
      MOTOR_SetSpeed(1, 0.15);

      nb_pulse = -1*nb_pulse;

      while(nb_pulse >= -ENCODER_Read(0) && nb_pulse >= ENCODER_Read(1)) {
        delay(10); 
      }
        

      if(nb_pulse<=-ENCODER_Read(0)){
          MOTOR_SetSpeed(0, 0);
          MOTOR_SetSpeed(1, 0.1);
          while(nb_pulse>=-ENCODER_Read(1));
          MOTOR_SetSpeed(1,0);
      } 
      else if(nb_pulse<=ENCODER_Read(1)){
          MOTOR_SetSpeed(1, 0);
          MOTOR_SetSpeed(0, -0.1);
          while(nb_pulse>=-ENCODER_Read(0));
          MOTOR_SetSpeed(0,0);
      } 
    }
    delay(100);
    ENCODER_Reset(0);
    ENCODER_Reset(1);
}

void Tour180(void)
{

  float quartC = (3.1416/2)*roue;
  float nb_pulse = 3200 * quartC/circonference;
    delay(500);
    MOTOR_SetSpeed(0, -0.15);
    MOTOR_SetSpeed(1, 0.15);

   while(nb_pulse >= -ENCODER_Read(0) && nb_pulse>= ENCODER_Read(1))
  {
  
    delay(10); 
  }
  MOTOR_SetSpeed(0, 0);
  MOTOR_SetSpeed(1, 0);

  if(nb_pulse<=ENCODER_Read(1)){
    MOTOR_SetSpeed(1, 0);
    while(nb_pulse >= -ENCODER_Read(0)) ;
    MOTOR_SetSpeed(0, 0);
  }
   else if(nb_pulse<=-ENCODER_Read(0)){
    MOTOR_SetSpeed(0, 0);
    while(nb_pulse >= ENCODER_Read(1)) ;
    MOTOR_SetSpeed(1, 0);
  }
  delay(100);
  ENCODER_Reset(0);
  ENCODER_Reset(1);
}

float pid(int nb_pulse_droite, int nb_pulse_gauche, float vitesse_droite)
{
  if (nb_pulse_droite > nb_pulse_gauche)
  {
    MOTOR_SetSpeed(1, vitesse_droite - variation);
    return vitesse_droite - variation;
  }
  else if (nb_pulse_droite < nb_pulse_gauche)
  {
    MOTOR_SetSpeed(1, vitesse_droite + variation);
    return vitesse_droite + variation;
  }
  return vitesse_droite;
}

//Fonction pour actionner le servo
//Le servo a deux positions possible, 0 ou 90 degrés.
//le paramètre release définit l'état du servo.
//Pour "catch" le servo va à un angle de 90 degrés.
//Pour "release" (release = true) le servo va à un angle de 0 degrés.
//Vous pouvez tweak l'angle en fonction de comment le servo est monté.
void CatchNRelease(bool release){
  int angle = 90;
  if(release)
  angle = 0;

  SERVO_SetAngle(0,angle);
  delay(1750);

}

void FindNPushPin()
{ 
    
  float distance= 100; 
  distance= SONAR_GetRange(0); 

while  (distance >40 ) 
{
  distance= SONAR_GetRange(0); 
  delay(100); 
}

  LedVerte= HIGH; 
  Tourner(90);
  delay(100); 
  Avancer(distance+10); 
  LedVerte=LOW;
  Tour180(); 
  delay(100); 
  Avancer(distance+10); 
}

void DragNDropBall()//rouge
{  

  Avancer(35);
  CatchNRelease(1); 

  if (couleur()==1)
  {
    Avancer(250);
    

  }
  if (couleur()==2)//bleu
{
  Tourner(90);
  delay(100);
  Avancer(60);
  Tourner(-90);
  delay(100); 
  Avancer(250);
   

}
  if (couleur()==3)//jaune
{
  Tourner(-90);
  delay(100); 
  Avancer(60);
  Tourner(90); 
  delay(100);
  Avancer(250);
 
}
  delay(1000);
  CatchNRelease(0); 
}

void RetourStart()
{

  Tourner(180);
  delay (100);
     
  if (couleur()==1)
  {
    Avancer(310);

  }
  if (couleur()==2)
{
  Avancer(250);
  
  Tourner(90);
  
  Avancer(30);

  Tourner(-90);
  
  Avancer(60);

}
  if (couleur()==3)
{
  
  Avancer(250);
 
  Tourner(-90);

  Avancer(30);

  Tourner(90);

  Avancer(60);

}
}

int couleur() { return 1;}

