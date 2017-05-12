#include "types.h"
Coord *buff;

void setup() {
  Serial.begin(9600);
  
  for(int x = 0; x < NUM_ROWS; x++) {
    pinMode(ROWS[x], OUTPUT);
    digitalWrite(ROWS[x], LOW);
  }

  for(int x = 0; x < NUM_COLS; x++) {
    pinMode(COLS[x], INPUT_PULLUP);
  }
  
  buff = (Coord*) malloc(sizeof(Coord) * NUM_OF_FINGERS);
  for(int x = 0; x < NUM_OF_FINGERS; x++) {
    buff[x].row = buff[x].col = -1;
  }
}

void loop() {
  scanMatrix(buff);
  flushBuff(buff);
  delay(75);
}

void scanMatrix(Coord *buffp) {
  Coord *itr = buffp;
  int keysPressed = 0;
  Coord point;
  for(point.row = 0; point.row < NUM_ROWS; point.row++) {
    setRow(point.row);
    for(point.col = 0; point.col < NUM_COLS; point.col++) {
      if(digitalRead(COLS[point.col]) == LOW) {
        //pressed
        (*itr).row = point.row;
        (*itr).col = point.col;
        itr++;
        keysPressed++;
        if(keysPressed == NUM_OF_FINGERS) {
          return;
        } 
      } else {
        Keyboard.release(DEFAULT_FACE[point.row][point.col]);
        Keyboard.release(FUNCTION_FACE[point.row][point.col]);
        Keyboard.release(LOWER_FACE[point.row][point.col]);
        Keyboard.release(RAISE_FACE[point.row][point.col]);
      }
    }
  }
}

void flushBuff(Coord *buffp) {
  //check buffer for modifiers
  int modifiers = 0;
  bool r = false;
  bool l = false;
  bool f = false;
  layer face = BASE;
  
  for(int x = 0; x < NUM_OF_FINGERS; x++) {
    modifiers |= checkModifiers(buffp[x]);
    //perform layer check if not trigger yet
    if(r == false) {
      r = isRaise(buffp[x]);
    }
    if(l == false) {
      l = isLower(buffp[x]);
    }
    if(f == false) {
      f  = isFunction(buffp[x]);
    }
  }
  face = checkLayer(l,r,f);
  Keyboard.set_modifier(modifiers);
  for(int x = 0; x < NUM_OF_FINGERS; x++) {
    if( (0 <= buffp[x].row && buffp[x].row < NUM_ROWS) && (0 <= buffp[x].col && buffp[x].col < NUM_COLS) ) {
      Serial.printf("%dx%d ", buffp[x].row, buffp[x].col);
      switch(face) {
        case FUNCTION :
          Serial.printf("FUNCTION\n");
          Keyboard.press(FUNCTION_FACE[(buffp[x].row)][(buffp[x].col)]);
          break;
        case LOWER :
          Serial.printf("LOWER\n");
          Keyboard.press(LOWER_FACE[(buffp[x].row)][(buffp[x].col)]);
          break;
        case RAISE :
          Serial.printf("RAISE\n");
          Keyboard.press(RAISE_FACE[(buffp[x].row)][(buffp[x].col)]);
          break;
        default:
          Serial.printf("DEFAULT\n");
          Keyboard.press(DEFAULT_FACE[(buffp[x].row)][(buffp[x].col)]);
      }
      buffp[x].row = -1;
      buffp[x].col = -1;
      //Keyboard.press(KEY_A);
    }
  }
}


layer checkLayer(bool l, bool r, bool f) {
  //function layer trumps other layers
  if (f) {
    return FUNCTION;
  } else {
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
}

int checkModifiers(Coord coord) {
  int ret = 0;
  ret |= (isCtrl(coord))? MODIFIERKEY_CTRL : 0;
  ret |= (isAlt(coord))? MODIFIERKEY_ALT : 0;
  ret |= (isShift(coord))? MODIFIERKEY_SHIFT : 0;
  ret |= (isGUI(coord))? MODIFIERKEY_GUI : 0;
  return ret;
}

bool isShift(Coord coord) { return ((coord.row == LEFT_SHIFT_ROW) && (coord.col == LEFT_SHIFT_COL)) || ((coord.row == RIGHT_SHIFT_ROW) && (coord.col == RIGHT_SHIFT_COL)); }
bool isCtrl(Coord coord) { return (coord.row == LEFT_CTRL_ROW) && (coord.col == LEFT_CTRL_COL); }
bool isAlt(Coord coord) { return (coord.row == LEFT_ALT_ROW) && (coord.col == LEFT_ALT_COL); }
bool isGUI(Coord coord) { return (coord.row == LEFT_GUI_ROW) && (coord.col == LEFT_GUI_COL); }
bool isRaise(Coord coord) { return (coord.row == RAISE_MOD_ROW) && (coord.col == RAISE_MOD_COL); }
bool isLower(Coord coord) { return (coord.row == LOWER_MOD_ROW) && (coord.col == LOWER_MOD_COL); }
bool isFunction(Coord coord) { return ((coord.row == LEFT_FUNCTION_ROW_1) && (coord.col == LEFT_FUNCTION_COL_1)) || ((coord.row == LEFT_FUNCTION_ROW_2) && (coord.col == LEFT_FUNCTION_COL_2)); }

bool checkKey(Coord coord) {
  setRow(coord.row);
  int output = digitalRead(COLS[coord.col]);
  return output == LOW;
}


void setRow(int rowp) {
   for(int x = 0; x < NUM_ROWS; x++) {
    if (x != rowp) {
      pinMode(ROWS[x], INPUT_PULLUP);
    } else {
      pinMode(ROWS[x], OUTPUT);
    }
  }
}
