
#include <conio.h>
#include <stdio.h>
#include <stdlib.h>
#include <peekpoke.h>
#include <string.h>
#include <c64.h>
#include <cbm_petscii_charmap.h>

#include "common.h"
//#link "common.c"

typedef struct{
  byte x; 	  // x coordinate
  byte y; 	  // y coordinate
  byte dir; 	  // direction 0-3
  byte score;	  // current score
  char head_attr; // char to draw player
  char tail_attr; // char to draw trail
  bool collided;  // did this player collide
  bool human;	  // is this player a human
} Player;

Player players[2]; // two players

const char BOX_CHARS[8] = { '+', '+', '+', '+',
 		 	    '-', '-', '-', '-' };

void draw_box(byte x, byte y, byte x2, byte y2, const char* chars){
  byte x1 = x;
  cputcxy(x, y, chars[2]);
  cputcxy(x2, y, chars[3]);
  cputcxy(x, y2, chars[0]);
  cputcxy(x2, y2, chars[1]);
  while (++x < x2){
    cputcxy(x, y, chars[5]);
    cputcxy(x, y2, chars[4]);
  }
  while (++y < y2){
    cputcxy(x1, y, chars[6]);
    cputcxy(x2, y, chars[7]);
  }
}

void draw_playfield() {
  draw_box(0, 1, COLS-1, ROWS-1, BOX_CHARS);
  cputsxy( 0, 0, "plyr1:");
  cputsxy(20, 0, "plyr2:");
  cputcxy( 7, 0, players[0].score+'0');
  cputcxy(27, 0, players[1].score+'0');
}

void init_game(){
  memset(players, 0, sizeof(players));
  players[0].head_attr = '1';
  players[0].tail_attr = '#';
  players[1].head_attr = '2';
  players[1].tail_attr = '*';
  //frames_per_move = START_SPEED;
}

void main(void) {
  clrscr();
  draw_playfield();
  getchar();
  
}
