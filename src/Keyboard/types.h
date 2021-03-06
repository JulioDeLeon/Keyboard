#ifndef TYPES_H
#define TYPES_H
#define u_long unsigned long
#define NUM_OF_FINGERS 10
#define DEBOUNCE_TIME 1
#define MAX_BASE_KEYS 6
#define MAX_MOUSE_ACTIONS 6
#define CYCLE_DELAY_TIME 45

enum layer { BASE, LOWER, RAISE, FUNCTION };
enum mouseAction { UP, DWN, LFT, RGHT, LCLK, RCLK, NON };
enum outputType { KEYBOARD, MOUSE };
// Uncomment original ROW[] and COLS[] when flashing gateron board

//const int ROWS[] = {2,3,4,5,6};
const int ROWS[] = {0,1,2,3,4};
const int NUM_ROWS = 5;
//const int COLS[] = {7,8,14,15,16,18,17,19,20,21,22,23};
const int NUM_COLS = 12;
const int COLS[] = {13,14,15,16,5,6,7,8,9,10,11,12};

const int LEFT_CTRL_COL = 0;
const int LEFT_CTRL_ROW = 4;

const int LEFT_ALT_COL = 2;
const int LEFT_ALT_ROW = 4;

const int LEFT_GUI_COL = 1;
const int LEFT_GUI_ROW = 4;

const int LEFT_FUNCTION_COL_1 = 3;
const int LEFT_FUNCTION_ROW_1 = 4;

const int LEFT_FUNCTION_COL_2 = 0;
const int LEFT_FUNCTION_ROW_2 = 2;


const int LOWER_MOD_COL = 4;
const int LOWER_MOD_ROW = 4;

const int RAISE_MOD_COL = 7;
const int RAISE_MOD_ROW = 4;

const int LEFT_SHIFT_COL = 0;
const int LEFT_SHIFT_ROW = 3;

const int RIGHT_SHIFT_COL = 11;
const int RIGHT_SHIFT_ROW = 3;

const int MOVE_MOUSE_LEFT_COL = 7;
const int MOVE_MOUSE_LEFT_ROW = 2;

const int MOVE_MOUSE_RIGHT_COL = 9;
const int MOVE_MOUSE_RIGHT_ROW = 2;

const int MOVE_MOUSE_DOWN_COL = 8;
const int MOVE_MOUSE_DOWN_ROW = 2;

const int MOVE_MOUSE_UP_COL = 8;
const int MOVE_MOUSE_UP_ROW = 1;

const int MOUSE_LEFT_CLICK_COL = 7;
const int MOUSE_LEFT_CLICK_ROW = 1;

const int MOUSE_RIGHT_CLICK_COL = 9;
const int MOUSE_RIGHT_CLICK_ROW = 1;

const layer MOUSE_LAYER = RAISE;
const int DEFAULT_MOUSE_DISTANCE = 20;

const int DEFAULT_FACE[NUM_ROWS][NUM_COLS] = {
  {KEY_ESC,KEY_1,KEY_2,KEY_3,KEY_4,KEY_5,KEY_6,KEY_7,KEY_8,KEY_9,KEY_0,KEY_BACKSPACE},
  {KEY_TAB,KEY_Q,KEY_W,KEY_E,KEY_R,KEY_T,KEY_Y,KEY_U,KEY_I,KEY_O,KEY_P,KEY_BACKSLASH},
  {0,KEY_A,KEY_S,KEY_D,KEY_F,KEY_G,KEY_H,KEY_J,KEY_K,KEY_L,KEY_SEMICOLON,KEY_QUOTE},
  {KEY_LEFT_SHIFT,KEY_Z,KEY_X,KEY_C,KEY_V,KEY_B,KEY_N,KEY_M,KEY_COMMA,KEY_PERIOD,KEY_SLASH,KEY_RIGHT_SHIFT},
  {KEY_LEFT_CTRL,KEY_LEFT_GUI,KEY_LEFT_ALT,0,0,KEY_SPACE,KEY_ENTER,0,KEY_LEFT,KEY_UP,KEY_DOWN,KEY_RIGHT}
};

const int FUNCTION_FACE[NUM_ROWS][NUM_COLS] = {
  {KEY_TILDE,KEY_F1,KEY_F2,KEY_F3,KEY_F4,KEY_F5,KEY_F6,KEY_F7,KEY_F8,KEY_F9,KEY_F10,KEY_DELETE,},
  {0,0,KEY_MEDIA_PREV_TRACK,KEY_MEDIA_PLAY_PAUSE,KEY_MEDIA_NEXT_TRACK,0,0,KEY_PAGE_UP,KEY_UP,KEY_PAGE_DOWN,KEY_PRINTSCREEN,0,},
  {0,0,KEY_MEDIA_VOLUME_DEC,KEY_MEDIA_VOLUME_INC,KEY_MEDIA_MUTE,0,KEY_HOME,KEY_LEFT,KEY_DOWN,KEY_RIGHT,0,0,},
  {0,0,0,0,0,0,KEY_END,0,0,0,0,0,},
  {KEY_LEFT_CTRL,KEY_LEFT_GUI,KEY_LEFT_ALT,0,0,KEY_SPACE,KEY_ENTER,0,KEY_LEFT,KEY_UP,KEY_DOWN,KEY_RIGHT}
};

const int LOWER_FACE[NUM_ROWS][NUM_COLS] = {
  {0,0,0,0,0,0,0,0,0,KEY_MINUS,KEY_EQUAL,KEY_BACKSPACE,},
  {0,0,0,0,0,0,0,0,0,KEY_LEFT_BRACE,KEY_RIGHT_BRACE,0,},
  {0,0,0,0,0,0,0,0,0,0,0,0,},
  {0,0,0,0,0,0,0,0,0,0,0,0,},
  {0,0,0,0,0,0,0,0,0,0,0,0,}
};

const int RAISE_FACE[NUM_ROWS][NUM_COLS] = {
  {KEY_NUM_LOCK,KEYPAD_7,KEYPAD_8,KEYPAD_9,KEYPAD_MINUS,0,0,0,0,0,0,KEY_BACKSPACE,},
  {0,KEYPAD_4,KEYPAD_5,KEYPAD_6,KEYPAD_PLUS,0,0,0,0,0,0,0,},
  {0,KEYPAD_1,KEYPAD_2,KEYPAD_3,KEYPAD_SLASH,0,0,0,0,0,0,0,},
  {0,KEYPAD_0,0,KEYPAD_PERIOD,KEYPAD_ASTERIX,0,0,0,0,0,0,0,},
  {0,0,0,0,0,0,0,0,0,0,0,0,}
};

struct Coord {
  int row;
  int col;
};

// util functions
layer checkLayer(bool l, bool r);
bool checkKey(Coord coord);
int checkModifiers(Coord coord);
bool isShift(Coord corrd);
bool isCtrl(Coord coord);
bool isAlt(Coord coord);
bool isGUI(Coord coord);
bool isRaise(Coord coord);
bool isLower(Coord coord);
bool isModifier(Coord coord);
bool isMediaCode(int code);
void setRow(int row);
void scanMatrix(Coord *buff);
void flushBuff(Coord *buff);
//Mouse functions are considered mouse functions in this case
mouseAction getMouseAction(Coord coord, layer l);
void handleMouseAction(mouseAction action);
bool isMouseLeft(Coord coord);
bool iaMouseRight(Coord coord);

void interpretPress(Coord coord, layer face, int *baseKeyCount, int *mediaBuff, int *mediaCount, mouseAction * mouseBuff, int *mouseActionCount);

#endif
