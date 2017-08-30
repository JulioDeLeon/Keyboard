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
  flushBuff2(buff);
  delay(CYCLE_DELAY_TIME);
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
        delay(DEBOUNCE_TIME);
        if(digitalRead(COLS[point.col]) == LOW) {
          (*itr).row = point.row;
          (*itr).col = point.col;
          itr++;
          keysPressed++;
          if(keysPressed == NUM_OF_FINGERS) {
            return;
          } 
        }
      } else {
        Keyboard.release(DEFAULT_FACE[point.row][point.col]);
        Keyboard.release(FUNCTION_FACE[point.row][point.col]);
        Keyboard.release(LOWER_FACE[point.row][point.col]);
        Keyboard.release(RAISE_FACE[point.row][point.col]);
        if(isMouseLeft(point)) {
          Mouse.release(MOUSE_LEFT);
        }
        if(isMouseRight(point)) {
          Mouse.release(MOUSE_RIGHT);
        }
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

void flushBuff2(Coord *buffp) {
  //check buffer for modifiers
  int modifiers = 0;
  
  // media actions that cant be handle by mirco manager
  int mediaCount = 0;
  int mediaCodes[MAX_BASE_KEYS];

  // mouse actions that cant be handle by keyboard actions
  int mouseActions = 0;
  mouseAction mouseCodes[MAX_MOUSE_ACTIONS];
  
  bool r = false;
  bool l = false;
  bool f = false;
  layer face = BASE;

  // So OSes wil support NKey however those keys need to be 6 non modifier keys
  // and 4 modifier keys. So the output buffer will contain non modifier keys. 
  int baseKeyCount = 0;
  
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
    
    if(isModifier(buffp[x])){
      buffp[x].col = -1;
      buffp[x].row = -1;
    }
  }

  //set layer and modifiers
  face = checkLayer(l,r,f);
  Keyboard.set_modifier(modifiers);

  //set output keys
  for(int x = 0; x < NUM_OF_FINGERS; x++) {
    if( (0 <= buffp[x].row && buffp[x].row < NUM_ROWS) && (0 <= buffp[x].col && buffp[x].col < NUM_COLS) ) {
      Serial.printf("%dx%d ", buffp[x].row, buffp[x].col);
      interpretPress(buffp[x], face, &baseKeyCount, &(mediaCodes[mediaCount]), &mediaCount, &(mouseCodes[mouseActions]), &mouseActions);
      buffp[x].row = -1;
      buffp[x].col = -1;
    }
    //Serial.printf("bk: %d mc: %d ma: %d\n", baseKeyCount, mediaCount, mouseActions);
  }
  //zero out unused keypresses
  zeroRestOutputHelper(baseKeyCount);

  //send keyboard output
  Keyboard.send_now();

  //handle media keys
  for(int x = 0; x < mediaCount; x++) {
    Keyboard.press(mediaCodes[x]);
  }

  //hanlde mouse actions
  for(int x = 0; x < mouseActions; x++) {
    //Serial.printf("mouse action: %d\n", x);
    handleMouseAction(mouseCodes[x]);
  }
}

void interpretPress(Coord coord, layer face, int *baseKeyCount, int *mediaBuff, int *mediaCount, mouseAction *mouseBuff, int *mouseActionCount){
  mouseAction act = getMouseAction(coord, face);
  int code = 0;
  if (act == NON) {
    switch(face) {
      case FUNCTION :
      Serial.printf("FUNCTION");
      code = FUNCTION_FACE[(coord.row)][(coord.col)];
      if(isMediaCode(code)){
        *mediaBuff = code;
        (*mediaCount)++;
      } else {
        setOutputHelper(*baseKeyCount, code);    
      }
      (*baseKeyCount)++;
      break;
    case LOWER :
      Serial.printf("LOWER");
      //Keyboard.press(LOWER_FACE[(coord.row)][(coord.col)]);
      code = LOWER_FACE[(coord.row)][(coord.col)];
      if(isMediaCode(code)){
        *mediaBuff = code;
        (*mediaCount)++;
      } else {
        setOutputHelper(*baseKeyCount, code);    
      }
      (*baseKeyCount)++;
      break;
    case RAISE :
      Serial.printf("RAISE");
      //Keyboard.press(RAISE_FACE[(coord.row)][(coord.col)]);
      code = RAISE_FACE[(coord.row)][(coord.col)];
      if(isMediaCode(code)){
        *mediaBuff = code;
        (*mediaCount)++;
      } else {
        setOutputHelper(*baseKeyCount, code);    
      }
      (*baseKeyCount)++;
      break;
    default:
      Serial.printf("DEFAULT");
      //Keyboard.press(DEFAULT_FACE[(coord.row)][(coord.col)]);
      code = DEFAULT_FACE[(coord.row)][(coord.col)];
      if(isMediaCode(code)){
        *mediaBuff = code;
        (*mediaCount)++;
      } else {
        setOutputHelper(*baseKeyCount, code);    
      }
      (*baseKeyCount)++;
    }
  } else {
    Serial.printf("MOUSE");
    *mouseBuff = act;
    (*mouseActionCount)++;
  }
  Serial.printf("\n");
}

void setOutputHelper (int outputNumber, int keyCode) {
  switch(outputNumber) {
    case 0:
      Keyboard.set_key1(keyCode);
      break;
    case 1:
      Keyboard.set_key2(keyCode);
      break;
    case 2:
      Keyboard.set_key3(keyCode);
      break;
    case 3:
      Keyboard.set_key4(keyCode);
      break;
    case 4:
      Keyboard.set_key5(keyCode);
      break;
    case 5:
      Keyboard.set_key6(keyCode);
      break;
    default:
      return;     
  }  
}

void zeroRestOutputHelper (int startIndex) {
  for (int x = startIndex; x < MAX_BASE_KEYS; x++) {
    switch(x) {
      case 0:
        Keyboard.set_key1(0);
        break;
      case 1:
        Keyboard.set_key2(0);
        break;
      case 2:
        Keyboard.set_key3(0);
        break;
      case 3:
        Keyboard.set_key4(0);
        break;
      case 4:
        Keyboard.set_key5(0);
        break;
      case 5:
        Keyboard.set_key6(0);
        break;
      default:
        return;     
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

bool isModifier(Coord coord) {
  return isShift(coord) || isCtrl(coord) || isAlt(coord) || isGUI(coord) || isRaise(coord) || isLower(coord) || isFunction(coord);
}

bool isMediaCode(int code) {
  //  {0,0,KEY_MEDIA_PREV_TRACK,KEY_MEDIA_PLAY_PAUSE,KEY_MEDIA_NEXT_TRACK,0,0,KEY_PAGE_UP,KEY_UP,KEY_PAGE_DOWN,KEY_PRINTSCREEN,0,},
  //{0,0,KEY_MEDIA_VOLUME_DEC,KEY_MEDIA_VOLUME_INC,KEY_MEDIA_MUTE,0
  return code == KEY_MEDIA_PREV_TRACK 
      || code == KEY_MEDIA_NEXT_TRACK
      || code == KEY_MEDIA_PLAY_PAUSE
      || code == KEY_MEDIA_VOLUME_DEC
      || code == KEY_MEDIA_VOLUME_INC
      || code == KEY_MEDIA_MUTE
      || code == KEY_NUM_LOCK;
}

mouseAction getMouseAction(Coord coord, layer l) {
  if (l != MOUSE_LAYER) {
    return NON;
  } else {
    if (coord.row == MOVE_MOUSE_LEFT_ROW && coord.col == MOVE_MOUSE_LEFT_COL) {
      return LFT;
    } else if (coord.row == MOVE_MOUSE_RIGHT_ROW && coord.col == MOVE_MOUSE_RIGHT_COL) {
      return RGHT;
    } else if (coord.row == MOVE_MOUSE_UP_ROW && coord.col == MOVE_MOUSE_UP_COL) {
      return UP;
    } else if (coord.row == MOVE_MOUSE_DOWN_ROW && coord.col == MOVE_MOUSE_DOWN_COL) {
      return DWN;
    } else if (coord.row == MOUSE_LEFT_CLICK_ROW && coord.col == MOUSE_LEFT_CLICK_COL) {
      return LCLK;
    } else if (coord.row == MOUSE_RIGHT_CLICK_ROW && coord.col == MOUSE_RIGHT_CLICK_COL) {
      return RCLK;
    } else {
      return NON;
    }
  }
  Serial.printf("Within getMouseAction, should not be here\n");
  return NON;
}

void handleMouseAction(mouseAction action) {
 switch(action) {
  case DWN:
    Mouse.move(0, DEFAULT_MOUSE_DISTANCE);
    break;
  case UP:
    Mouse.move(0, (-1) * DEFAULT_MOUSE_DISTANCE);
    break;
  case LFT:
    Mouse.move((-1) * DEFAULT_MOUSE_DISTANCE, 0);
    break;
  case RGHT:
    Mouse.move(DEFAULT_MOUSE_DISTANCE, 0);
    break;
  case LCLK:
    Mouse.press(MOUSE_LEFT);
    break;
  case RCLK:
    Mouse.press(MOUSE_RIGHT);
    break;
  case NON:
  default:
    return;
 }
}

bool isMouseLeft(Coord coord) {
  return (coord.row == MOUSE_LEFT_CLICK_ROW) && (coord.col == MOUSE_LEFT_CLICK_COL);
}

bool isMouseRight(Coord coord) {
  return (coord.row == MOUSE_RIGHT_CLICK_ROW) && (coord.col == MOUSE_RIGHT_CLICK_COL);
}

bool checkKey(Coord coord) {
  setRow(coord.row);
  int output = digitalRead(COLS[coord.col]);
  return output == LOW;
}


void setRow(int rowp) {
   for(int x = 0; x < NUM_ROWS; x++) {
    if (x != rowp) {
      digitalWrite(ROWS[x], HIGH);
    } else {
      digitalWrite(ROWS[x], LOW);
    }
  }
}

void unsetAllRows() {
  for(int x = 0; x < NUM_ROWS; x++) {
    digitalWrite(ROWS[x], LOW);
  }
}

