void wallie() {
  /*Игра Wallie*/
  isGameOver = false;
  chooseTheButton();
  waitForPressed();
  if (!isGameOver) {
    waitWhilePressed();
  }

}

/*===========================================================================================================*/
/*Выбор случайной кнопки
   Использует аналоговый пин А5 для снятия наводок
   Выбирает случайное число от 0 до BTN_COUNT
   Изменяет выбранную ячейку маски с 0 на 1
   Включает диод на пине под выбранным номером
   Выводит выбранное число в Порт

*/
void chooseTheButton() {
  Serial.println("choosing random button");
  while (true) {
    randomSeed(analogRead(A5));
    int a = random(0, BTN_COUNT - 1);
    if (mask[a] == false) {
      mask[a] = true;
      changeLedState(a, HIGH);
      Serial.print("I've chosen: ");
      Serial.println(a);
      delay(1000);
      break;
    }
  }
}
/*===========================================================================================================*/
/* Ожидает 5 секунд нажатия всех кнопок маски
   Если кнопка маски нажата, то выключает её диод
   Если нажата неверная кнопка вызывает лоха
*/
void waitForPressed() {
  Serial.println("Жду, когда все будет нажато");
  for (currentTime = millis(); millis() - currentTime < 10000;) {
    if (isAllPressed()) {
      Serial.println("Все нужные кнопки нажаты");
      return;
    }
  }
  Serial.println("Btns hadn't been pressed");
  loser();
}
/*===========================================================================================================*/
/* Проверяет все ли кнопки из маски были нажаты
    Возвращает true если все кнопки нажаты
    Возвращает false если не все кнопки были нажаты

*/

boolean isAllPressed() {
  for (int i = 0 ; i < BTN_COUNT ; i++ ) {
    if (mask[i] == true) {
      if (digitalRead(btn[i]) == false) {
        delay(30);
        if (digitalRead(btn[i]) == false) {
          return false;
        }
      }
    }
    else if ((mask[i] == false) and (digitalRead(btn[i]) == true )) {
      Serial.print("Нажата неверная кнопка " );
      Serial.println(i);
      loser();
      return;

    }
  }
  return true;
}

/*===========================================================================================================*/
/* 3 секунды проверяет нажаты ли кнопки.
    Если не нажаты, то вызывает функцию лоха
*/
void waitWhilePressed() {
  Serial.println("Ищу отжатия");
  for (currentTime = millis(); millis() - currentTime < 5000;) {
    if (!isAllPressed()) {
      Serial.println("Отжатие найдено");
      loser();
      return;
    }
  }
  Serial.println("Winner");
}
