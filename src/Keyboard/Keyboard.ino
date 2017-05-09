#include "types.h"

/*n7ujm
 * a
 */
void setup() {
  //start serial connection for printing
  Serial.begin(9600);
  
  for(int x = 0; x < NUM_ROWS; x++){
    pinMode(ROWS[x], OUTPUT);
    digitalWrite(ROWS[x], LOW);
  }

  for(int x = 0; x < NUM_COLS; x++) {
    pinMode(COLS[x], INPUT_PULLUP);
  }
}


int row = 0;
int col = 0;

void loop() {
  //readMatrix();
  //printMatrix();
  Keyboard.set_modifier(checkModifiers());
  setRow(row); 
  if (checkKey(row,col)) {
    Serial.printf("%dx%d has been pressed: %c\n", row,col,DEFAULT_FACE[row][col]);
    //Keyboard.print(DEFAULT_FACE[row][col]);
    Keyboard.press(DEFAULT_FACE[row][col]);
    delay(150);

  } else {
    Keyboard.release(DEFAULT_FACE[row][col]);
  }

  row = (row + 1) % NUM_ROWS;
  if (row == NUM_ROWS -1) {
    col = (col + 1) % NUM_COLS; 
  }
}



/*
 * 
 * 
 */

 layer checkLayer() {
  bool r = checkKey(RAISE_MOD_ROW, RAISE_MOD_COL);
  bool l = checkKey(LOWER_MOD_ROW, LOWER_MOD_COL);
  if( not (r xor l) ) {
    return BASE;
  } else if ( r and not l) {
    return RAISE;
  } else if ( l and not r) {
    return LOWER;
  } else {
    return BASE;
  }
}

int checkModifiers() {
  int ret = 0;
  ret |= (checkKey(LEFT_CTRL_ROW,LEFT_CTRL_COL))? MODIFIERKEY_CTRL : 0;
  ret |= (checkKey(LEFT_ALT_ROW,LEFT_ALT_COL))? MODIFIERKEY_ALT : 0;
  ret |= (checkKey(LEFT_SHIFT_ROW,LEFT_SHIFT_COL))? MODIFIERKEY_SHIFT : 0;
  ret |= ret | (checkKey(LEFT_GUI_ROW,LEFT_GUI_COL))? MODIFIERKEY_GUI : 0;
  return ret;
}

bool checkKey(int rowP, int colP) {
  setRow(rowP);
  int output = digitalRead(COLS[colP]);
  return output == LOW;
}


void setRow(int rowp) {
   for(int x = 0; x < NUM_ROWS; x++) {
    if (x != rowp) {
      //digitalWrite(ROWS[x], LOW);
      pinMode(ROWS[x], INPUT_PULLUP);
    } else {
      pinMode(ROWS[x], OUTPUT);
      //digitalWrite(ROWS[x], LOW);
    }
  }
}
