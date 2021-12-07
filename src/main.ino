/*
  Projet: Le nom du script
  Equipe: Votre numero d'equipe
  Auteurs: Les membres auteurs du script
  Description: Breve description du script
  Date: Derniere date de modification
*/

#include <LibRobus.h>
#include <pk04_library.h>

Adafruit_SH1106 oled(-1);

void setup() {
  BoardInit();
  Serial.begin(9600);
  oled.begin(SH1106_SWITCHCAPVCC, 0x3C);
  oled.clearDisplay();
  oled.display();
  pinMode(pos1, INPUT);
  pinMode(pos2, INPUT);
  pinMode(pos3, INPUT);
  pinMode(TEST1, INPUT);//Droite
  pinMode(TEST2, INPUT);//Milieu
  pinMode(TEST3, INPUT);//Gauche
  pinMode(cote, INPUT);//cote
  pinMode(butvert, INPUT);
  pinMode(PinAlcool, INPUT);
  SERVO_Enable(0);
  SERVO_SetAngle(0, 75);
  delay(200);
  Serial.println("Test du ID-12 sur UART2 (RX2/Digital 17)");
  SERVO_Disable(0);
  oled.begin(SH1106_SWITCHCAPVCC, 0x3C);
  oled.clearDisplay();
}
void screen(int texte) {

  switch (texte) {
    case 1:
      oled.clearDisplay();
      oled.setTextSize(1.5);
      oled.setTextColor(WHITE);
      oled.setCursor(0, 29);
      oled.print("Pete toi une can");
      oled.display();
      delay(1000);
      break;
    case 2:
      oled.clearDisplay();
      oled.setTextSize(2);
      oled.setTextColor(WHITE);
      oled.setCursor(0, 29);
      oled.print("Chaudasse");
      oled.display();
      delay(1000);
      break;
    case 3:
      oled.clearDisplay();
      oled.setTextSize(2);
      oled.setTextColor(WHITE);
      oled.setCursor(0, 29);
      oled.print("Chaud bateau");
      oled.display();
      delay(1000);
      break;
    case 4:
      oled.clearDisplay();
      oled.setTextSize(2);
      oled.setTextColor(WHITE);
      oled.setCursor(0, 29);
      oled.print("Pas gerable");
      oled.display();
      delay(1000);
      break;
    case 5:
      oled.clearDisplay();
      oled.setTextSize(2);
      oled.setTextColor(WHITE);
      oled.setCursor(0, 29);
      oled.print("Traitement");
      oled.display();
      delay(1000);
      break;
  }
  delay(5000);
  oled.clearDisplay();
  oled.display();
}

void loop() {

  float vitesseG = VITESSE;
  float vitesseD = VITESSE;
  int flag = 0;
  int action = 0;

  oled.drawBitmap(0, 0, bitmap_mcdo , 128 , 64 , WHITE);
  oled.display();

  while (flag == 0) {
    suiveurDeLigne(&vitesseG, &vitesseD);
    /* Serial.print(digitalRead(TEST1));
      Serial.print(digitalRead(TEST2));
       Serial.print(digitalRead(TEST3));*/
    flag = digitalRead(pos1);
    Serial.print(flag);
  }
  MOTOR_SetSpeed(0, 0);
  MOTOR_SetSpeed(1, 0);

  delay(100);

  while (action == 0) {
    action = button();
    delay(100);

    if (action == 1) {
      swince();
      delay(1000);
    }

    if (action == 2) {
      oled.clearDisplay();
      screen(AlcooTest());
      delay(2000);
    }
  }
  delay(100);
}