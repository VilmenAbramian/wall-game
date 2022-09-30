#include "Arduino.h"
#include "DFRobotDFPlayerMini.h"

DFRobotDFPlayerMini myDFPlayer;  //инициализируем плеер
void printDetail(uint8_t type, int value);
byte volume = 15; //sound level
static unsigned long timer = 0;
int musicTime = 3000;


void setup()
{
  dfp_init();
}
void loop()
{
  win();
  lose();
  gong();
  gun();
  gong2();
  scuby();
  levelEnd();
  meow();
  mario();
}

void dfp_init(){
Serial1.begin(9600); //скорость com-port, которую поддерживает плеер
  if (!myDFPlayer.begin(Serial1)) {}  //Use softwareSerial to communicate with mp3.
  myDFPlayer.volume(volume);  
}
  
//=======================================================
void win()
{
  if (millis()-timer>musicTime)
  {
    timer = millis();
    myDFPlayer.play(1);
  }
}
//=======================================================
void lose()
{
  if (millis()-timer>musicTime)
  {
    timer = millis();
    myDFPlayer.play(2);
  }
}
//=======================================================
void gong()
{
  if (millis()-timer>musicTime)
  {
    timer = millis();
    myDFPlayer.play(3);
  }
}
//=======================================================
void gun()
{
  if (millis()-timer>musicTime)
  {
    timer = millis();
    myDFPlayer.play(4);
  }
}
void gong2()
{
  if (millis()-timer>musicTime)
  {
    timer = millis();
    myDFPlayer.play(5);
  }
}
void scuby()
{
  if (millis()-timer>musicTime)
  {
    timer = millis();
    myDFPlayer.play(6);
  }
}
void levelEnd()
{
  if (millis()-timer>musicTime)
  {
    timer = millis();
    myDFPlayer.play(7);
  }
}
void meow()
{
  if (millis()-timer>musicTime)
  {
    timer = millis();
    myDFPlayer.play(8);
  }
}
void mario()
{
  if (millis()-timer>musicTime)
  {
    timer = millis();
    myDFPlayer.play(9);
  }
}
