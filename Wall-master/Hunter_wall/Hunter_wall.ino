#include <Adafruit_NeoPixel.h>
#define LED_PIN 5
#define NUM_LEDS 60
Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUM_LEDS, LED_PIN, NEO_GRB + NEO_KHZ800);

//задаем номера пинов для кнопок и их значения по умолчанию
byte score1 = 0; //кол-во правильных нажатий первой стороной
byte score2 = 0; //кол-во правильных нажатий второй стороной
byte btnPins[BTN_COUNT] = {49, 47, 45, 43, 41, 39, 37, 35, 33, 31, 48, 46, 44, 42, 40, 38, 36, 34, 32, 30}; //номера пинов
byte btnState[BTN_COUNT] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }; //начальные состояния кнопок
byte int1 = 2; //прерывание 1 группы кнопок
byte int2 = 3; //прерывание 2 группы кнопок

byte maxScore=50;//максимальный счет

//прерывание 1
void BUTTON1() {
  //деактивируем прерывания
  detachInterrupt(digitalPinToInterrupt(int1));
  detachInterrupt(digitalPinToInterrupt(int2));
  delay(20);

  //записываем в массив состояние каждой кнопки группы 1
  for (byte i = 0; i < 10; i++) {
    btnState [i] = digitalRead(btnPins[i]);
  }

  //в цикле суммируем все элементы массива состояний
  //Если хотя бы один элемент из первых 10 равен 1, то state1 равен 1
  for (byte i = 0; i < 10; i++) {
    state1 = state1 + ButtonState[i];
  }

  //активируем прерывания
  attachInterrupt(digitalPinToInterrupt (int1), BUTTON1, RISING);
  attachInterrupt(digitalPinToInterrupt (int2), BUTTON2, RISING);
}

//прерывание 2
void BUTTON2() {
  //деактивируем прерывания
  detachInterrupt(digitalPinToInterrupt(int1));
  detachInterrupt(digitalPinToInterrupt(int2));
  delay(20);

  //записываем в массив состояние каждой кнопки группы 2
  for (byte i = 10; i < 20; i++) {
    btnState [i] = digitalRead(btnPins[i]);
  }

  //в цикле суммируем все элементы массива состояний
  //Если хотя бы один элемент из вторых 10 равен 1, то state2 равен 1
  for (byte i = 10; i < 20; i++) {
    state2 = state2 + ButtonState[i];
  }

  //активируем прерывания
  attachInterrupt(digitalPinToInterrupt (int1), BUTTON1, RISING);
  attachInterrupt(digitalPinToInterrupt (int2), BUTTON2, RISING);
}

void setup() {
  Serial.begin(9600);

  //настраиваем все пины кнопок на вход
  for (byte i = 0; i < 20; i++) {
    pinMode(btnPins[i], INPUT);
  }

  //настраиваем пины прерываний на вход
  pinMode(int1, INPUT);
  pinMode(int2, INPUT);
}

void loop() {
  ohotnik(400, 2, 3, 50, 1);
}

//==================================================================================
void ohotnik(int deltaTime, byte int1, byte int2, byte brightness, byte diviation) {
  strip.begin();

  //разрешить прерывания
  attachInterrupt(digitalPinToInterrupt (int1), BUTTON1, RISING);
  attachInterrupt(digitalPinToInterrupt (int2), BUTTON2, RISING);

  //установить яркость
  strip.setBrightness(brightness);

  //пределы генерации светящегося пикселя
  byte pixel = random(diviation, 60 - diviation);

  //рандомом задается вероятность появления нецелевых цветов
  if (random(0, 2)) {
    for (byte i = pixel - diviation; i <= pixel + diviation; i++)
    {
      strip.setPixelColor(i, strip.Color(0, 0, 255));
    }
  }
  else {
    for (byte i = pixel - diviation; i <= pixel + diviation; i++)
    {
      strip.setPixelColor(i, strip.Color(0, 255, 0));
    }
  }
  strip.show();

  //задержка горения цвета
  delay(deltaTime);

  //обнулить цвета
  for (byte i = pixel - diviation; i <= pixel + diviation; i++)
  {
    strip.setPixelColor(i, strip.Color(0, 0, 0));
  }
  strip.show();

  //рандомом задается вероятность появления целевого цвета
  if (random(0, 10) == 5) {

    //пределы генерации светящегося пикселя
    pixel = random(diviation, 60 - diviation);

    for (byte i = pixel - diviation; i <= pixel + diviation; i++)
    {
      strip.setPixelColor(i, strip.Color(255, 0, 0));
    }
    strip.show();

    //задержка горения цвета
    //прибавление счета игрокам
      timee = millis();
      while (millis()-timee < deltaTime) {
        if (state1) {
          score1 = score1 + 1;
        }
        if (state2) {
          score2 = score2 + 1;
        }
        if (score1 = maxScore) {
          winOhotnik(1, 255, 0, 0);
        }
        if (score2 = maxScore) {
          winOhotnik(2, 0, 0, 255);
        }
      }

    //обнулить цвета
    for (byte i = pixel - diviation; i <= pixel + diviation; i++)
    {
      strip.setPixelColor(i, strip.Color(0, 0, 0));
    }
    strip.show();
  }
//=======================================================================================

//=======================================================================================
//функция цвета светодиодов (R, G, B 0...255) для победы игрока под номером N.
void winOhotnik(N, R, G, B) {

  //winPlay(); //воспроизвести звук выигрыша
  //matrixWriteText(String WIN, int N);  //выиграл игрок N

  Serial.println("win");
  Serial.println(N);
  //лента горит красным на время задержки
  for (byte i = 0; i <= 59; i++)
  {
    strip.setPixelColor(i, strip.Color(R, G, B));
  }
  strip.show();

  delay(2000);  //задержка после выигрыша

  score1 = 0; //обнуление нажатий, затем, новая игра
  score2 = 0;
}
//=======================================================================================
