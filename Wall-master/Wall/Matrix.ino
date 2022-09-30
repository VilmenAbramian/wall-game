/*===========================================================================================================*/
/*Выводит счёт по игрокам
 * left - первый игрок
 * rigjt - второй игрок
*/
void matrixWriteScore(int left, int right) {
  matrix.fillScreen(LOW);
  String left_str = String(left);
  String right_str = String(right);

  char left_ch[2];
  char right_ch[2];
  left_str.toCharArray(left_ch, 2);
  right_str.toCharArray(right_ch, 2);


  if (left < 10) {
    matrix.drawChar(2, 0, '0', HIGH, LOW, 1);
    matrix.drawChar(8, 0, left_ch[0], HIGH, LOW, 1);
  }
  else {
    String(left / 10).toCharArray(left_ch, 2);
    matrix.drawChar(2, 0, left_ch[0], HIGH, LOW, 1);
    String(left % 10).toCharArray(left_ch, 2);
    matrix.drawChar(8, 0, left_ch[0], HIGH, LOW, 1);
  }

  if (right < 10) {
    matrix.drawChar(19, 0, '0', HIGH, LOW, 1);
    matrix.drawChar(25, 0, right_ch[0], HIGH, LOW, 1);
  }
  else {
    String(right / 10).toCharArray(left_ch, 2);
    matrix.drawChar(19, 0, right_ch[0], HIGH, LOW, 1);
    String(right / 10).toCharArray(right_ch, 2);
    matrix.drawChar(25, 0, right_ch[0], HIGH, LOW, 1);
  }
  matrix.drawPixel(15, 1, HIGH);
  matrix.drawPixel(16, 1, HIGH);
  matrix.drawPixel(15, 2, HIGH);
  matrix.drawPixel(16, 2, HIGH);

  matrix.drawPixel(15, 5, HIGH);
  matrix.drawPixel(16, 5, HIGH);
  matrix.drawPixel(15, 6, HIGH);
  matrix.drawPixel(16, 6, HIGH);
  matrix.write();
}
/*===========================================================================================================*/
