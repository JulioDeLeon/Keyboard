#include "types.h"
int row = 0;
int col = 0;

void resetOutputs() {
  for (int x = 0; x < NUM_ROWS; x++) {
    for (int y = 0; y < NUM_COLS; y++) {
      OUTPUTS[x][y] = 0;
    }
  }
}

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

  resetOutputs();
}



void scanRow(int row) {
  for(int x = 0; x < NUM_ROWS; x++) {
    if (x != row) {
      digitalWrite(ROWS[x], LOW);
    } else {
      digitalWrite(ROWS[x], HIGH);
    }
  }

  int output = 0;
  for(int x = 0; x < NUM_COLS; x++) {
    OUTPUTS[row][x] = digitalRead(COLS[x]);
    output = output + (OUTPUTS[row][x] << x);
  }
  
  if ( output != 4095) {
    for(int x = 0; x < NUM_COLS; x++) {
      if( 1 and (output >> x) ){
        Serial.printf("HERE is: %dx%d: %c at %d\n", row, x, DEFAULT_FACE[row][x], output);   
      }
    }
  }
  
}

void readMatrix() {
  for(int x = 0; x < NUM_ROWS; x++) {
  //  scanColumn(x);
    scanRow(x);
    resetOutputs();
  }
}

void printMatrix() {
  char* buff = (char*) malloc(sizeof(char) * 32);
  for(int x = 0; x < NUM_ROWS; x++){
    for(int y = 0; y < NUM_COLS; y++){
      sprintf(buff, "%dx%d: %d ",x,y, OUTPUTS[x][y]);
      Serial.print(buff);
    }
  }
  Serial.print("\n");
  free(buff);
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

void loop() {
  resetOutputs();
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

