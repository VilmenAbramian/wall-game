int latchPin = 12;
int clockPin = 11;
int dataPin = 13;
byte ledState[20] = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}; //состояния светодиодов
void setup()
{
  Serial.begin(9600);
  pinMode(latchPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(dataPin, OUTPUT);

  for (int i = 0; i < 20; i++)
  {
    ledControll(i, 1); //данные, передаваемые функции
    delay(550);
    ledControll(i, 0); //данные, передаваемые функции
    delay(550);
  }



}

void loop()
{

}

void ledControll(byte number, bool state)
{
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
