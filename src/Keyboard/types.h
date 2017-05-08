#ifndef TYPES_H
#define TYPES_H

const int ROWS[] = {2,3,4,5,6};
const int NUM_ROWS = 5;
const int COLS[] = {7,8,14,15,16,18,17,19,20,21,22,23};
const int NUM_COLS = 12;

const char DEFAULT_FACE[NUM_ROWS][NUM_COLS] = {
  {0,'1','2','3','4','5','6','7','8','9','0',0},
  {'\t','q','w','e','r','t','y','u','i','o','p',0},
  {0,'a','s','d','f','g','h','j','k','l',';',0},
  {0,'z','x','c','v','b','n','m',',',',','/',0},
  {0,0,0,0,0,' ',0,0,0,0,0,0,}
};


#endif
