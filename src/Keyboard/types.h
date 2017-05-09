#ifndef TYPES_H
#define TYPES_H
#include <Bounce2.h>
#define u_long unsigned long

const u_long BOUNCE_TIME = 5;
const int ROWS[] = {2,3,4,5,6};
const int NUM_ROWS = 5;
const int COLS[] = {7,8,14,15,16,18,17,19,20,21,22,23};
const int NUM_COLS = 12;

const int LEFT_CTRL_COL = 0;
const int LEFT_CTRL_ROW = 4;

const int LEFT_ALT_COL = 2;
const int LEFT_ALT_ROW = 4;

const int LEFT_GUI_COL = 1;
const int LEFT_GUI_ROW = 4;

const int LEFT_FUNCTION_COL = 3;
const int LEFT_FUNCTION_ROW = 4;

const int LOWER_MOD_COL = 4;
const int LOWER_MOD_ROW = 4;

const int RAISE_MOD_COL = 7;
const int RAISE_MOD_ROW = 4;

const int LEFT_SHIFT_COL = 0;
const int LEFT_SHIFT_ROW = 3;

enum layer { BASE, LOWER, RAISE };

const int DEFAULT_FACE[NUM_ROWS][NUM_COLS] = {
  {KEY_ESC,KEY_1,KEY_2,KEY_3,KEY_4,KEY_5,KEY_6,KEY_7,KEY_8,KEY_9,KEY_0,KEY_BACKSPACE},
  {KEY_TAB,KEY_Q,KEY_W,KEY_E,KEY_R,KEY_T,KEY_Y,KEY_U,KEY_I,KEY_O,KEY_P,KEY_BACKSLASH},
  {0,KEY_A,KEY_S,KEY_D,KEY_F,KEY_G,KEY_H,KEY_J,KEY_K,KEY_L,KEY_SEMICOLON,KEY_QUOTE},
  {KEY_LEFT_SHIFT,KEY_Z,KEY_X,KEY_C,KEY_V,KEY_B,KEY_N,KEY_M,KEY_COMMA,KEY_PERIOD,KEY_SLASH,0},
  {KEY_LEFT_CTRL,KEY_LEFT_GUI,KEY_LEFT_ALT,0,0,KEY_SPACE,KEY_ENTER,0,KEY_LEFT,KEY_DOWN,KEY_UP,KEY_RIGHT}
};

const int FUNCTION_FACE[NUM_ROWS][NUM_COLS] = {
  {0,0,0,0,0,0,0,0,0,0,0,0,},
  {0,0,0,0,0,0,0,0,0,0,0,0,},
  {0,0,0,0,0,0,0,0,0,0,0,0,},
  {0,0,0,0,0,0,0,0,0,0,0,0,},
  {0,0,0,0,0,0,0,0,0,0,0,0,}
};


// util functions
layer checkLayer();
int checkModifiers();
bool checkKey(int row, int col);
void setRow(int row);

#endif
