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

Adafruit_SH1106 oled(-1);

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
  Serial.begin(9600);
    oled.begin(SH1106_SWITCHCAPVCC, 0x3C);
    oled.clearDisplay();
    oled.display();
    pinMode(redpin, OUTPUT);
    pinMode(greenpin, OUTPUT);
    pinMode(bluepin, OUTPUT);
    pinMode(pos1, INPUT);
    pinMode(pos2, INPUT);
    pinMode(pos3, INPUT);
    pinMode(TEST1, INPUT);//Droite
    pinMode(TEST2, INPUT);//Milieu
    pinMode(TEST3, INPUT);//Gauche
    pinMode(cote, INPUT);//cote
    pinMode(butvert, INPUT);
    pinMode(poids, INPUT);
    pinMode(PinAlcool, INPUT);
    pinMode(FonctionAlcool, OUTPUT);
    digitalWrite(FonctionAlcool, LOW);
    SERVO_Enable(0);
    SERVO_SetAngle(0,180);
    delay(200);
//  Serial2.begin(9600);
  Serial.println("Test du ID-12 sur UART2 (RX2/Digital 17)");
SERVO_Disable(0);
  oled.begin(SH1106_SWITCHCAPVCC, 0x3C);

  oled.clearDisplay();
}
void screen(int texte) {
 
 switch(texte) {
    case 1:
      oled.clearDisplay();
      oled.setTextSize(2);
      oled.setTextColor(WHITE);
      oled.setCursor(0,29);
      oled.print("Sobre");
      oled.display();
      delay(1000);
      //oled.clearDisplay();
      //oled.display();
      break;
    case 2:
      oled.clearDisplay();
      oled.setTextSize(2);
      oled.setTextColor(WHITE);
      oled.setCursor(0,29);
      oled.print("Bu");
      oled.display();
      delay(1000);
      //oled.clearDisplay();
      //oled.display();
      break;
    case 3:
      oled.clearDisplay();
      oled.setTextSize(2);
      oled.setTextColor(WHITE);
      oled.setCursor(0,29);
      oled.print("Chaudasse");
      oled.display();
      delay(1000);
      //oled.clearDisplay();
      //oled.display();
      break;
    case 4:
      oled.clearDisplay();
      oled.setTextSize(2);
      oled.setTextColor(WHITE);
      oled.setCursor(0,29);
      oled.print("Bateau");
      oled.display();
      delay(1000);
      //oled.clearDisplay();
      //oled.display();
      break;
    case 5:
      oled.clearDisplay();
      oled.setTextSize(2);
      oled.setTextColor(WHITE);
      oled.setCursor(0,29);
      oled.print("Traitement");
      oled.display();
      delay(1000);
      //oled.clearDisplay();
      //oled.display();
      break;
  }
  delay(5000);
     oled.clearDisplay();
    oled.display();
}

/* ****************************************************************************
Fonctions de boucle infini (loop())
**************************************************************************** */
// -> Se fait appeler perpetuellement suite au "setup"

void loop() {

float vitesseG = VITESSE;
  float vitesseD = VITESSE;
 int flag = 0; 
 int action = 0;
oled.drawBitmap(0,0, bitmap_mcdo , 128 , 64 , WHITE);
oled.display();
 
while(flag == 0){
  suiveurDeLigne(&vitesseG, &vitesseD);
  flag = digitalRead(pos1);
  Serial.print(flag);
}
MOTOR_SetSpeed(0,0);
MOTOR_SetSpeed(1,0);
delay(100);

action = button();
delay(100);
if(action == 1){
  swince();
  delay(1000);
  }
if(action == 2){
  oled.clearDisplay();
  screen(AlcooTest());
  delay(4000);
}
delay(100);
}