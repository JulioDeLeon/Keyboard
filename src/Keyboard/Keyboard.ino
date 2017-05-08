#include "types.h"

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

int row = 0;
int col = 0;

void loop() {
  //readMatrix();
  //printMatrix();
   setRow(row); 
  int output = digitalRead(COLS[col]);
  if (output == LOW) {
    Serial.printf("%dx%d has been pressed: %c\n", row,col,DEFAULT_FACE[row][col]);
    Keyboard.print(DEFAULT_FACE[row][col]);
    delay(150);
  }
  row = (row + 1) % NUM_ROWS;
  if (row == NUM_ROWS -1) {
    col = (col + 1) % NUM_COLS; 
  }
}

