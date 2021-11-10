#include <pk04_library.h>


// -> defines...
// L'ensemble des fonctions y ont acces
//#define circonference 24.2496; // en cm (un tour-->3200)
#define circonference 24.24; //A Croquette
const float roue = 18.9; //A Croquette
const float variation  = 0.001;
float speed ;
float vroom = .45;
float turn = .15;
int color = 0 ;
bool maxDistance;
double maxSpeed;
float pCorrection;
int cycle, pulseCounterMaster, pulseCounterSlave, distanceMotor0, distanceMotor1, preError;



int dernierCas[3] = {0}; 
int nbCasUn = 0;

//Implémentez vos fonction ici
Adafruit_TCS34725 tcs = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_50MS, TCS34725_GAIN_4X);
//Fonctions de mouvement pour le robot avec  PID
void Avancer(float distance_voulue, float speed)
{
  float nb_pulse = (3200 * distance_voulue)/circonference;
  float variationvit = 0.18;
  float vitesse = speed - variationvit;
  float vitesse_nouvelle = 0;
  int acc=0;
  int dec=0;
  ENCODER_Reset(0);
  ENCODER_Reset(1);

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

void Tourner(int angle, float vit)
{
  float arc = angle*(3.1416*roue)/360;
  float nb_pulse = 3200 * arc/circonference;
  ENCODER_Reset(0);
  ENCODER_Reset(1);
    if(angle>0)
    {
      MOTOR_SetSpeed(0, vit);
      MOTOR_SetSpeed(1, -vit);

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
      MOTOR_SetSpeed(0, -vit);
      MOTOR_SetSpeed(1, vit);

      nb_pulse = -1*nb_pulse;

      while(nb_pulse >= -ENCODER_Read(0) && nb_pulse >= ENCODER_Read(1)) {
        delay(10); 
      }
        

      if(nb_pulse<=-ENCODER_Read(0)){
          MOTOR_SetSpeed(0, 0);
          MOTOR_SetSpeed(1, 0.1);
          while(nb_pulse>=ENCODER_Read(1));
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
  int angle = 105;
  if(release)
  angle = 165;

  SERVO_SetAngle(0,angle);
  delay(250);

}

int Find()
{ 
    
  float distance; 
  distance= SONAR_GetRange(0); 
Serial.print(distance);
if  (distance > 40 || distance < 5 ) 
{ 
  return 0;
}
else{
  return 1;
}
} 
void frappe()
{
  
digitalWrite(ledr, LOW);
digitalWrite(ledb, LOW);
digitalWrite(ledj, LOW);
  digitalWrite(LedVerte, HIGH);
  float distance = SONAR_GetRange(0);
  Tourner(90, turn);
  //delay(100);
  SERVO_SetAngle(0,55); 
  Avancer(distance-4, vroom + .2); 
  yeet();
  digitalWrite(LedVerte, LOW);

  delay(100); 
  
  Avancer(distance-4, vroom + .2); 

    Tourner(85, turn);

}

void DragNDropBall(int color)//rouge
{  

  if (color==1)
  {
    Tourner(2, turn);
    Avancer(50, vroom + .2);
    Avancer(50, vroom + .2);
    Avancer(50, vroom + .2);
    Avancer(50, vroom + .2);
    Avancer(17, vroom + .2);
    

  }
  if (color==2)//bleu
{
  Tourner(-90, turn);
  delay(100);
  Avancer(47, vroom);
  Tourner(96, turn);
  delay(100); 
    Avancer(50, vroom);
    Avancer(50, vroom);
    Avancer(50, vroom);
    Avancer(50, vroom);
    Avancer(17, vroom);
   

}
  if (color==3)//jaune
{
  Tourner(90, turn);
  delay(100); 
  Avancer(45, vroom);
  Tourner(-93, turn); 
  delay(100);
    Avancer(50, vroom);
    Avancer(50, vroom);
    Avancer(50, vroom);
    Avancer(50, vroom);
    Avancer(19, vroom);
 
}
  delay(1500);
  CatchNRelease(1); 
}

void RetourStart(int color)
{
     
  if (color==1) //rouge
  // retourne vers le circuit directement

  {
      Tourner(184.5, turn + .1);

    Avancer(305, vroom + .2);
    Tourner(-90,turn + .1);

  }
  if (color==2) //bleu
  // retourne vers le circuit directement(se dirige legerement vers le centre)

{
    Tourner(184.5, turn);

  Avancer(280, vroom + .2);
  
  Tourner(-90, turn);
  
  Avancer(10, vroom);

  Tourner(90, turn);
  
  Avancer(20, vroom);

  Tourner(-90,turn);

}
  if (color ==3 )//jaune
// retourne vers le circuit directement(se dirige legerement vers le centre)
{
    Tourner(184.5, turn);

  Avancer(280, vroom + .2);
 
  Tourner(90, turn);

  Avancer(10, vroom);

  Tourner(-90, turn);

  Avancer(20, vroom);

  Tourner(-90,turn);

}
}

int couleur() {
    uint16_t clear, red, green, blue;
int color = 0;

while (color == 0){
  tcs.setInterrupt(false);      // turn on LED

    delay(60);  // takes 50ms to read

    tcs.getRawData(&red, &green, &blue, &clear);

    tcs.setInterrupt(true);  // turn off LED
   /* Serial.print(red);
    Serial.print("\t");
    Serial.print(green);
    Serial.print("\t");
    Serial.print(blue);
    Serial.print("\n");*/

if(600 <= red && red <= 750 && 500 <= green && green <= 649 && 600 <= blue && blue <= 750 )//rose
{
color = 1;
}
if(400 <= red && red <= 600 && 600 <= green && green <= 850 && 700 <= blue && blue <= 900 )//bleu
{
  color = 2;
}
if(600 <= red && red <= 750 && 650 <= green && green <= 850 && 500 <= blue && blue <= 750 )//jaune
{
  color = 3;
}
    //delay(250);

}
if(color == 1)
{

  digitalWrite(ledr, HIGH);
}
if(color == 2)
{
  digitalWrite(ledb, HIGH);
}if(color == 3)
{
  digitalWrite(ledj, HIGH);
}
return color;
}

int sifflet()
{
  
if (analogRead(mic) > 575)
{
  
  digitalWrite(ledr, HIGH);
  digitalWrite(ledb, HIGH);
  digitalWrite(ledj, HIGH);

  return 1;
}
else{
return 0;
}
}

int capteurLigneCase(bool c, bool d, bool g){
  
  if (c == 0 && d == 0 && g == 0) return 8;
  else if (c == 0 && d == 0) return 7;
  else if(c == 0 && g == 0 ) return 6;
  else if(c == 0) return 5;
  else if(d == 0 && g == 0) return 4;
  else if(d == 0) return 3;
  else if(g == 0) return 2;
  else return 1;

    return ERREUR;
    
}

void reglerVitesse(int cas, float* vGauche, float* vDroite){
  if(cas == 1){
    if(dernierCas[2] == 1 && dernierCas[1] == 1){
      int signeG = 1, signeD = 1;
      if(dernierCas[0] == 2) signeG = -1;
      else if(dernierCas[0] == 3) signeD = -1;
      else signeD = -1;

     MOTOR_SetSpeed(0, VITESSE * signeG);
     MOTOR_SetSpeed(1, VITESSE * signeD);

      //tourne sur lui même tant que pas retrouvé une ligne blanche
      do{
          delay(5);
        }while ((capteurLigneCase((digitalRead(TEST2)), 
                  (digitalRead(TEST1)), 
                  (digitalRead(TEST3))))  == 1);

        *vGauche = VITESSE;
        *vDroite = VITESSE;
      }    
  }
  else if(cas == 2){
         if(*vDroite < 0.5){
          *vGauche -= MODIF_VITESSE;
          *vDroite += MODIF_VITESSE;
        }
  }
  else if(cas == 3){
      if(*vGauche < 0.5){
          *vGauche += MODIF_VITESSE;
          *vDroite -= MODIF_VITESSE; 
        }
  }
  else if(cas == 4 || cas == 5 || cas == 6 || cas == 7 || cas == 8)
  {
      *vDroite = VITESSE;
      *vGauche = VITESSE;
  }
    dernierCas[0] = dernierCas[1];
    dernierCas[1] = dernierCas[2];
    dernierCas[2] = cas;
}

void suiveurDeLigne(float *vitesseG, float *vitesseD){

    
    int z = capteurLigneCase((digitalRead(TEST2)), (digitalRead(TEST1)), (digitalRead(TEST3)));
    reglerVitesse(z, vitesseG, vitesseD);

   MOTOR_SetSpeed(0, *vitesseG);
   MOTOR_SetSpeed(1, *vitesseD);

    /*Serial.print("\n\n");
    Serial.print("   cas: ");
    Serial.print(z);
    Serial.print("   VGauche:");
    Serial.print(*vitesseG);
    Serial.print("   VDroite:");
    Serial.print(*vitesseD);*/

};
void yeet()
{
  
  Tourner(90, .65);
  Tourner(-270, .65);
  //Tourner(90, .5);
  SERVO_SetAngle(0, 165);
  delay(50);


}
int scan()
{
int val = 0;
int i;
for ( i = 0 ; i < 100 ; i++)
{
val = analogRead(cote) + val;
}
val = val / i;
val = val + 50 ;
delay(500);
return val;


}





