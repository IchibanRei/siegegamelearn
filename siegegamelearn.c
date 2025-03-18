
#include <conio.h>
#include <stdio.h>
#include <stdlib.h>
#include <peekpoke.h>
#include <string.h>
#include <c64.h>
#include <cbm_petscii_charmap.h>

#include "common.h"
//#link "common.c"

void delay(byte count) {
  while(count--) {
    waitvsync();
  }
}

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

byte frames_per_move; // speed of the game

#define START_SPEED 12
#define MAX_SPEED 5
#define MAX_SCORE 7

typedef enum { D_RIGHT, D_DOWN, D_LEFT, D_UP } Direction;
const char BOX_CHARS[8] = { '+', '+', '+', '+',
 		 	    '-', '-', '|', '|' };
const sbyte DIR_X[4] = { 1, 0, -1, 0 };
const sbyte DIR_Y[4] = { 0, -1, 0, 1 };

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
  frames_per_move = START_SPEED;
}

void reset_players() {
  players[0].x = players[0].y = 5;
  players[0].dir = D_RIGHT;
  players[1].x = COLS - 6;
  players[1].y = ROWS - 6;
  players[1].dir = D_LEFT;
  players[0].collided = players[1].collided = 0;
}

byte readcharxy (byte x, byte y) {
  return PEEK(SCRNADR(0x400, x, y));
}

void draw_player (Player* p) {
  cputcxy(p->x, p->y, p->head_attr);
}

void flash_colliders() {
  byte i;
  // flash players that collided
  for(i = 0;i < 56; i++) {
    delay(2);
    revers(players[0].collided && (i&1));
    draw_player(&players[0]);
    revers(&players[1].collided && (i&1));
    draw_player(&players[1]);
  }
  revers(0);
}

void play_round() {
  reset_players();
  clrscr();
  textcolor(COLOR_WHITE);
  draw_playfield();
  while(1) {
    if (players[0].collided || players[1].collided) break;
  }
  flash_colliders();
}

void human_control(Player *p) {
  byte dir = 0xff;
  char joy;
  if(!p->human) return; // if not a human return
  if(!kbhit()) return; // return if no key hit
  joy = joy_read(0); // read first joystick
  if(JOY_UP(joy))dir = D_UP;
  if(JOY_LEFT(joy))dir = D_LEFT;
  if(JOY_RIGHT(joy))dir = D_RIGHT;
  if(JOY_DOWN(joy))dir = D_DOWN;
  // don't let the player reverse direction
  if(dir < 0x80 && dir != (p->dir^2)) {
    p->dir = dir;
  }
}

void move_player(Player* p) {
  cputcxy(p->x, p->y, p->tail_attr);
  p->x += DIR_X[p->dir];
  p->y += DIR_Y[p->dir];
  if((readcharxy(p->x, p->y) & 0x7f) != ' ') {
    p->collided = 1;
    draw_player(p);
  }
}

void make_move() {
  byte i;
  for (i = 0; i < frames_per_move; i++) {
    human_control(&players[0]);
  }
  //ai_control(&players[0]);
  //ai_control(&players[1]);
  // if players collide, 2nd player gets the point
  textcolor(COLOR_CYAN);
  move_player(&players[1]);
  textcolor(COLOR_YELLOW);
  move_player(&players[0]);
  textcolor(COLOR_WHITE);
}


void main(void) {
  clrscr();
  draw_playfield();
  getchar();
  make_move();
  
}
