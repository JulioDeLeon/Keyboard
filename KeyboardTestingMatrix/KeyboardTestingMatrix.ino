int inputs[] = {2,3,4,5,6,7,8,14,15,16,17,18,19,20,21,22,23};

const int ROWS[] = {2,3,4,5,6};
const int NUM_ROWS = 5;

const int COLS[] = {7,8,14,15,16,18,17,19,20,21,22,23};
const int NUM_COLS = 12;
int OUTPUTS[NUM_ROWS][NUM_COLS] = {{0}};

//
//void printOutputs(int* outputs) {
//  char* outputStr = (char*) malloc(sizeof(char) * 64);
//  sprintf(outputStr, "2: %d 3: %d 4: %d 5: %d 6: %d 7: %d 8: %d 14: %d 15: %d 16: %d 17: %d 18: %d 19: %d 20: %d 21: %d 22: %d 23: %d",
//    outputs[0], outputs[1], outputs[2], outputs[3], outputs[4], outputs[5], outputs[6], outputs[7], outputs[8], outputs[9], 
//    outputs[10], outputs[11], outputs[12], outputs[13], outputs[14], outputs[15], outputs[16]);
//  Serial.println(outputStr);
//  free(outputStr);
//}

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

void scanColumn(int col) {
  for(int x = 0; x < NUM_COLS; x++) {
    if (x != col) {
      digitalWrite(COLS[x], LOW);
    } else {
      digitalWrite(COLS[x], HIGH);
    }
  }

  for(int x = 0; x < NUM_ROWS; x++) {
    OUTPUTS[x][col] = digitalRead(ROWS[x]);
  }
}

void scanRow(int row) {
  for(int x = 0; x < NUM_ROWS; x++) {
    if (x != row) {
      digitalWrite(ROWS[x], LOW);
    } else {
      digitalWrite(ROWS[x], HIGH);
    }
  }

  for(int x = 0; x < NUM_COLS; x++) {
    OUTPUTS[row][x] = digitalRead(COLS[x]);
  }
}

void readMatrix() {
  for(int x = 0; x < NUM_ROWS; x++) {
  //  scanColumn(x);
    scanRow(x);
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

void loop() {
  resetOutputs();
  readMatrix();
  printMatrix();
}



