#ifndef FSM_H
#define FSM_H

#include <stdbool.h>

#include "../brick_game/tetris/tetris_states.h"

typedef enum {
  Start,
  Pause,
  Terminate,
  Left,
  Right,
  Up,
  Down,
  Action
} UserAction_t;

typedef struct {
  int **field;
  int **next;
  int score;
  int high_score;
  int level;
  int speed;
  int pause;
} GameInfo_t;

void handle_start_game(GameState_t *p);
void handle_pause_game(GameState_t *p);
void handle_unpause_game(GameState_t *p);
void handle_exit_game(GameState_t *p);
void handle_restart_game(GameState_t *p);
void handle_move_left(GameState_t *p);
void handle_move_right(GameState_t *p);
void handle_soft_drop(GameState_t *p);
void handle_rotate(GameState_t *p);

void userInput(UserAction_t action, bool hold);
GameInfo_t updateCurrentState();

#endif