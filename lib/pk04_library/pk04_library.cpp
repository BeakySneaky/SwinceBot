#include <pk04_library.h>


//Implémentez vos fonction ici


void CatchNRelease(bool release){

  int angle = 90;
  if(release)
  angle = 0;

  SERVO_SetAngle(0,angle);
  delay(1750);

}