#include <Adafruit_NeoPixel.h>
const byte LED_PIN = 5;
const byte NUM_LEDS =  60;
Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUM_LEDS, LED_PIN, NEO_GRB + NEO_KHZ800);
byte score1 = 0; //кол-во нажатий первой стороной
byte score2 = 0; //кол-во нажатий второй стороной
byte int1 = 2; //прерывание 1 группы кнопок
byte int2 = 3; //прерывание 2 группы кнопок

//задаем номера пинов для кнопок и их значения по умолчанию
byte btnPins[20] = {49, 47, 45, 43, 41, 39, 37, 35, 33, 31, 48, 46, 44, 42, 40, 38, 36, 34, 32, 30}; //номера пинов
byte btnState[20] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }; //начальные состояния кнопок

bool state1 = 0;    //флаг нажатий первых 10 кнопок
bool state2 = 0;    //флаг нажатий вторых 10 кнопок

//=======================================================================================
//прерывание 1
void BUTTON1() {
   // Serial.println('1');
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
    state1 = state1 + btnState[i];
  }

  //активируем прерывания
  attachInterrupt(digitalPinToInterrupt (int1), BUTTON1, RISING);
  attachInterrupt(digitalPinToInterrupt (int2), BUTTON2, RISING);

    for (byte i = 0; i < 10; i++) {
      Serial.println(btnState [i]);
  }
}

void BUTTON2() {
  Serial.println('2');
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
    state2 = state2 + btnState[i];
  }

  //активируем прерывания
  attachInterrupt(digitalPinToInterrupt (int1), BUTTON1, RISING);
  attachInterrupt(digitalPinToInterrupt (int2), BUTTON2, RISING);

      for (byte i = 10; i < 20; i++) {
      Serial.print(btnState [i]);
           Serial.println();
  }
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
  //вызываем функцию канат с параметрами (цвет каната r, g, b, номера пинов прерывания, яркость)
  kanat(0, 255, 0, int1, int2, 250);
}


//=======================================================================================
void kanat(byte R, byte G, byte B, byte int1, byte int2, byte brightness) {

  strip.begin();

  //активируем прерывания
  attachInterrupt(digitalPinToInterrupt (int1), BUTTON1, RISING);
  attachInterrupt(digitalPinToInterrupt (int2), BUTTON2, RISING);

  strip.setBrightness(brightness);  //устанавливаем яркость ленты

  //обнуление цвета пикселей
  for (byte i = 0; i <= 59; i++)
  {
    strip.setPixelColor(i, strip.Color(0, 0, 0));
  }

  //заполнение центра (2 пикселя горят по умолчанию)
  strip.setPixelColor(29, strip.Color(R, G, B));
  strip.setPixelColor(30, strip.Color(R, G, B));

  //включение слева в соответствии с текущим значением score1
  for (byte i = 29 - score1; i < 29; i++)
  {
    strip.setPixelColor(i, strip.Color(R, G, B));
  }

  //включение справа в соответствии с текущим значением score2
  for (byte i = 31; i < 31 + score2; i++)
  {
    strip.setPixelColor(i, strip.Color(R, G, B));
  }
  strip.show();

  //если есть нажатие, то...
  if (state1 == 1) {
    //buttonPlay(); //включить звук нажатия
    if (score2 == 0) {
      score1 = score1 + 1;
    }
    if (score2 > 0) {
      score2 = score2 - 1;
    }

    //ограничиваем диапазон переменных
    score1 = constrain(score1, 0, 29);
    score2 = constrain(score2, 0, 29);

    //обнуляем состояние первых 10 кнопок
    for (byte i = 0; i < 10; i++) {
      btnState[i] = 0;
    }

    //если количество нажатий 1 игрока достигло 29, то...
    if (score1 == 29) {
      winKanat(1, 255, 0, 0); //игрок 1 выиграл
    }
    //обнулить флаг нажатий
    state1 = 0;
        state2 = 0;
  }

  //если есть нажатие, то...
  if (state2 == 1) {
    //buttonPlay(); //включить звук нажатия
    if (score1 > 0) {
      score1 = score1 - 1;
    }
    if (score1 == 0) {
      score2 = score2 + 1;
    }

    //ограничиваем диапазон переменных
    score1 = constrain(score1, 0, 29);
    score2 = constrain(score2, 0, 29);

    //обнуляем состояние вторых 10 кнопок
    for (byte i = 10; i < 20; i++) {
      btnState[i] = 0;
    }

    //если количество нажатий 2 игрока достигло 29, то...
    if (score2 == 29) {
      winKanat(2, 0, 0, 255); //игрок 2 выиграл
    }
    //обнулить флаг нажатий
        state1 = 0;
    state2 = 0;
  }
}
//=======================================================================================



//=======================================================================================
//функция цвета светодиодов (R, G, B 0...255) для победы игрока под номером N.
void winKanat(byte N,byte R,byte G, byte B) {

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
