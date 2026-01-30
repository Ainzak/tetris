#ifndef TETRIS_STATES_H
#define TETRIS_STATES_H

#include "defines.h"

typedef enum {
  STATE_START,
  STATE_SPAWN,
  STATE_MOVING,
  STATE_ATTACH,
  STATE_PAUSE,
  STATE_GAMEOVER,
  STATE_EXIT,
  STATE_COUNT
} States_t;

typedef struct {
  int field[FIELD_HEIGHT][FIELD_WIDTH];
  int next[4][4];
  int current[4][4];
  int piece_x, piece_y;
  int piece_type;
  int rotation;
  int next_type;
  int score;
  int high_score;
  int level;
  int speed;
  States_t state;
  int paused;
  bool game_over;
  long long last_drop_time;
} GameState_t;

#endif