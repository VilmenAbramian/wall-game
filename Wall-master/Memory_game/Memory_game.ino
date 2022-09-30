const byte BTN_COUNT = 20; // amount of button using in game
const byte LED_COUNT = 3; // amount the pins for controll buttons leds by remove register

byte latchPin = 33; //ST_CP
byte clockPin = 34; //SH_CP
byte dataPin = 35; //DS

byte btnPins[BTN_COUNT] = {2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 22, 23, 24, 25, 26, 27, 28, 29}; // arduino pins
byte ledPins[LED_COUNT] = {30, 31, 32}; //arduino pins
byte buttonBank1[BTN_COUNT / 2] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
byte buttonBank2[BTN_COUNT / 2] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
byte press1[BTN_COUNT / 2] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
byte press2[BTN_COUNT / 2] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
byte score1 = 0;
byte score2 = 0;

byte chain [10][3] = //массив, храниящий все предыдущие значения горящих светодиодов кнопок
{
  {0, 0, 0},
  {0, 0, 0},
  {0, 0, 0},
  {0, 0, 0},
  {0, 0, 0},
  {0, 0, 0},
  {0, 0, 0},
  {0, 0, 0},
  {0, 0, 0},
  {0, 0, 0},
};

int lightTime = 1200; //время горения одной пары кнопок
int boopTime = 900; //время, выделяемое на нажатие кнопоки

byte flag = 0; // number of preset buttons (количество проходов программы)

unsigned long currentTime = 0; // all work time

void setup()
{
  Serial.begin(9600);
  for (byte i = 0; i < BTN_COUNT; i++) //initialize arduino pins
  {
    pinMode(btnPins[i], INPUT); //with arduino`s resistor
    pinMode(ledPins[i], OUTPUT);
  }
  pinMode(latchPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(dataPin, OUTPUT);
}
//=====================================================================================
void loop()
{
  generateButton();  //generate true random values for both gamers
  indicate();  //включает подсветку кнопок обоих игроков
  waitForPressed(); //смотрим нажатия
  result(); //проверяе результаты
}
//======================================================================================
void generateButton()
{
  Serial.println("Start for generate");
  if (flag >= 9)
    flag = 0;
  randomSeed(analogRead(A0));
  byte a = random(0, ((BTN_COUNT / 2) - 1));
  buttonBank1[flag] = a;

  randomSeed(analogRead(A0));
  byte b = random(10, BTN_COUNT - 1);
  buttonBank2[flag] = b;

  flag++;
  Serial.print("Set of buttons:");
  Serial.println(flag);
}
//=========================================================================================
void indicate()
{
  Serial.println("Start indicate");
  generateCodRegister(); //генерирует три байта, которые непосредственно отправляются на регистры
  for (int i = 0; i <= flag; i++)
  {
    for (currentTime = millis(); millis() - currentTime < lightTime;)
    {
      digitalWrite(latchPin, LOW);
      shiftOut(dataPin, clockPin, LSBFIRST, chain[i][0]);
      shiftOut(dataPin, clockPin, LSBFIRST, chain[i][1]);
      shiftOut(dataPin, clockPin, LSBFIRST, chain[i][2]);
      digitalWrite(latchPin, HIGH);
    }
  }
  Serial.println("End indicate");
}
//=========================================================================================
void generateCodRegister() //генерирует три байта
{
  byte x1 = 128;
  byte y1 = 128;
  byte x2 = 128;
  byte y2 = 128;
  byte z = 0;
  //for (byte i = 0; i <= 9; i++)
  //{
  if (buttonBank1[flag] <= 6)
  {
    x1 = x1 >> buttonBank1[flag] + 1;
    y1 = 0;
  }
  if (buttonBank1[flag] > 6)
  {
    x1 = 0;
    y1 = y1 >> buttonBank1[flag] - 6;
  }
  //}
  //for (byte i = 10; i <= 19; i++)
  //{
  if (buttonBank1[flag] <= 13)
  {
    x2 = x2 >> buttonBank1[flag] - 6;
    y2 = 0;
  }
  if (buttonBank1[flag] > 13)
  {
    x2 = 0;
    y2 = y2 >> buttonBank1[flag] - 13;
  }
  //}
  z = y1 | x2;
  chain[flag][0] = y2;
  chain[flag][1] = z;
  chain[flag][2] = x1;
  //y2, z, x1
}

//=========================================================================================
void waitForPressed()
{
  Serial.println("Waiting for pressing");
  for (int i = 0; i <= (BTN_COUNT / 2 - 1); i++)
  {
    do
    {
      press1[i] = digitalRead(btnPins[i]);
      if (press1[i] == buttonBank1[i])
        score1++;
      currentTime = millis();
    }
    while (currentTime + boopTime > millis());
  }
  for (int i = 10; i <= BTN_COUNT; i++)
  {
    do
    {
      press2[i] = digitalRead(btnPins[i]);
      if (press2[i] == buttonBank2[i])
        score2++;
      currentTime = millis();
    }
    while (currentTime + boopTime > millis());
  }
}

//=========================================================================================
void result()
{
  Serial.print("Score_one:");
  Serial.println("score1");
  Serial.print("Score_two:");
  Serial.println("score2");
}
