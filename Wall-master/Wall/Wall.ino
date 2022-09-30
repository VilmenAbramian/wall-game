#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Max72xxPanel.h>


//количество кнопок и диодов
const int BTN_COUNT = 20;
//Пины с группами прерываний
const int ISR_1 = 2;
const int ISR_2 = 3;

//пины кнопок
byte btn[BTN_COUNT] = {49, 47, 45, 43, 41, 39, 37, 35, 33, 31, 48, 46, 44, 42, 40, 38, 36, 34, 32, 30}; //номера пинов
//массив с состояниями диодов
int ledState[BTN_COUNT] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}; // Присвоить номера!!!!
//массив с масками (какие должы быть состояния кнопок во время игры)
boolean mask[BTN_COUNT] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
//счёт первого и второго игрока
int timers[BTN_COUNT] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0} ;
int timer = 0;
boolean flagInterrupt = false;
boolean flag = false;

boolean isGameOver = false;

int score1 = 0;
int score2 = 0;

byte gameMode = 1;


int latchPin = 12;
int clockPin = 11;
int dataPin = 13;

const int pinCS = 10; // ПИН CS для матрицы
const int numberOfHorizontalDisplays = 4; //количество горизонтальных матриц
const int numberOfVerticalDisplays = 1; //количество вертикальных матриц
Max72xxPanel matrix = Max72xxPanel(pinCS, numberOfHorizontalDisplays, numberOfVerticalDisplays); // объект матрица


//счётчик
unsigned long currentTime = 0;

/*===========================================================================================================*/
void setup() {
  //выбираем режим работы для пинов диодов и кнопок
  for (int i = 0; i < BTN_COUNT; i++) {
    pinMode(btn[i], INPUT);
    //pinMode(led[i], OUTPUT);
  }
  //последоательный порт для отладки
  Serial.begin(9600);

  pinMode(latchPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(dataPin, OUTPUT);

  
  //выбор яркости матрицы
  matrix.setIntensity(7);
  //поворот матриц
  matrix.setRotation(0, 1);
  matrix.setRotation(1, 1);
  matrix.setRotation(2, 1);
  matrix.setRotation(3, 1);

}

/*===========================================================================================================*/
void loop() {
 wallie();
}

/*===========================================================================================================*/
/* Функция лоха
*/
void loser() {
  reset();
  isGameOver=true;
  offAllLedState();
  delay(5000);
  Serial.println("Losser");
}
/*===========================================================================================================*/
/*
   Функция изменения состояния светодиода
   number - номер светодиода
   state - состояние HIGH или LOW
   Функция должна изменять массив состояний светодиодов и загружать этот массив в регистр
*/
void changeLedState(byte number, boolean state) {
  ledState[number] = state;
  byte x[8] = {0, 0, 0, 0, 0, 0, 0, 0};
  byte y[8] = {0, 0, 0, 0, 0, 0, 0, 0};
  byte z[8] = {0, 0, 0, 0, 0, 0, 0, 0};
  byte a = B00000000;
  byte b = B00000000;
  byte c = B00000000;
 for (int i=1; i<8; i++)
  {
    x[i] = ledState[i-1] << i;
  }
  x[0] = B00000000;
  a = x[0]|x[1]|x[2]|x[3]|x[4]|x[5]|x[6]|x[7];

  for (int i=8; i<15; i++)
  {
    y[i-7] = ledState[i-1] << i-7;
  }
  y[0] = B00000000;
  b = y[0]|y[1]|y[2]|y[3]|y[4]|y[5]|y[6]|y[7];

    for (int i=15; i<21; i++)
  {
    z[i-14] = ledState[i-1] << i-14;
  }
  z[0] = B00000000;
  z[7] = B00000000;
  c = z[0]|z[1]|z[2]|z[3]|z[4]|z[5]|z[6]|z[7];
  
  //Serial.println(a, BIN);
  Serial.println(b, BIN);
  digitalWrite(latchPin, LOW);
  shiftOut(dataPin, clockPin, MSBFIRST, c);
  shiftOut(dataPin, clockPin, MSBFIRST, b);
  shiftOut(dataPin, clockPin, MSBFIRST, a);
  digitalWrite(latchPin, HIGH);

}
/*===========================================================================================================*/
void offAllLedState() {
  digitalWrite(latchPin, LOW);   
  shiftOut(dataPin, clockPin, LSBFIRST, B00000000);   
  shiftOut(dataPin, clockPin, LSBFIRST, B00000000); 
  shiftOut(dataPin, clockPin, LSBFIRST, B00000000);
  digitalWrite(latchPin, HIGH);
}
/*===========================================================================================================*/
/* Функция победы в раунде
*/
void winRound() {

}
/*===========================================================================================================*/
int Random(int a, int b) {
  randomSeed(analogRead(A5));
  return random(a, b);
}
/*===========================================================================================================*/
/*
   Обнуляет все переменные используемые в играх
   НЕ ОБНУЛЯЕТ индекс игры
   Записывает в currentTime текущее время

*/
void reset() {
  Serial.println("Reset available");
  for (int i = 0; i < BTN_COUNT; i++) {
    mask[i] = 0;
    timers[i] = 0;
    ledState[i]=0;
  }
  timer = 0;
  flagInterrupt = false;
  flag = false;
  score1 = 0;
  score2 = 0;
  currentTime = millis();
}
