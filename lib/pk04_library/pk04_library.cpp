#include <pk04_library.h>


// -> defines...
// L'ensemble des fonctions y ont acces
//#define circonference 24.2496; // en cm (un tour-->3200)
#define circonference 24.24; //A Croquette
const float roue = 18.9; //A Croquette
const float variation  = 0.001;
float speed ;
float vroom = .45;
float turn = .25;
int color = 0 ;
bool maxDistance;
double maxSpeed;
float pCorrection;
int cycle, pulseCounterMaster, pulseCounterSlave, distanceMotor0, distanceMotor1, preError;
/*char crecu;
//char i;
char  incoming=0;
char id_tag[11];*/
int dernierCas[3] = {0}; 
int nbCasUn = 0;

//Implémentez vos fonction ici
//Adafruit_TCS34725 tcs = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_50MS, TCS34725_GAIN_4X);
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
    delay(50); 
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



}
 

void swince(){
  SERVO_Enable(0);
  delay(50);
  SERVO_SetAngle(0, 75);
  Serial.print("yeet");
  delay(50);
  while(digitalRead(butred) == 0){
    delay (10);
  }
  
  SERVO_SetAngle(0, 178);
delay(300);
SERVO_SetAngle(0, 75);

  while(digitalRead(butred) == 0){
    delay (10);
  }
  SERVO_SetAngle(0, 178);
  delay(250);
SERVO_Disable(0);
}


int button(){
  while(1){
    if(digitalRead(butvert) == HIGH){
      return 2;
    }
        if(digitalRead(butred) == HIGH){
      return 1;
    }
  }
}

int AlcooTest() {
  //Vérifier que la pin est bien initialiser (A10)

  //Pour éteindre le capteur quand pas utilisé
  //digitalWrite(FonctionAlcool, HIGH);
  Serial.print("Calibration de l'appareil.\n");
  delay(1000);
  Serial.print("Début du test.\n");
  int ValeurAnalog[100]; //Tableau qui va permettre de trouver la valeur analog la plus haute
  int MaxValeur = 0;
  int i;
  int cas; 

  //Affiche "Traitement" à l'écran
  //Vérifier que les librairies et l'écran sont bien installés/déclarés
  screen(5);


  //Boucle qui lit les valeurs durant un certain temps (à mesurer combien) et enregistre la valeur max
  for(i = 0; i < 100; i++) {
    ValeurAnalog[i] = analogRead(PinAlcool);
    Serial.print(ValeurAnalog[i]);
    Serial.print("\n");

    if (ValeurAnalog[i] > MaxValeur) {
      MaxValeur = ValeurAnalog[i];
    }
    delay(20);
  }

  Serial.print("\n\n\n\n");
  Serial.print(MaxValeur);
  Serial.print("\n\n\n\n");

  

  if (MaxValeur <= 200) {
    cas = 1;
  }
  else if (MaxValeur > 200 && MaxValeur <= 300) {
    cas = 2;
  }
  else if (MaxValeur > 300 && MaxValeur <= 400) {
    cas = 3;
  }
  else if (MaxValeur > 400 && MaxValeur <= 1024) {
    cas = 4;
  }

  Serial.print("\n\n\n\n");
  Serial.print(cas);
  Serial.print("\n\n\n\n");

  Serial.print("Fin du test.\n");
  return cas;
}

