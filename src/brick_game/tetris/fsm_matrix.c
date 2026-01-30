#include "../../common/fsm.h"
#include "defines.h"
#include "tetramino.h"
#include "tetris_backend.h"

void (*fsm_table[STATE_COUNT][ACTION_COUNT])(GameState_t *) = {
    [STATE_MOVING] =
        {
            [Pause] = handle_pause_game,
            [Left] = handle_move_left,
            [Right] = handle_move_right,
            [Down] = handle_soft_drop,
            [Action] = handle_rotate,
        },
    [STATE_PAUSE] = {[Pause] = handle_unpause_game}};

void handle_pause_game(GameState_t *p) {
  if (p->paused == 0) {
    p->paused = 1;
  }
  p->state = STATE_PAUSE;
}
void handle_unpause_game(GameState_t *p) {
  if (p->paused) {
    p->paused = 0;
  }
  p->state = STATE_MOVING;
}
void handle_exit_game(GameState_t *p) {
  if (p->state == STATE_GAMEOVER) {
    handle_highscore(p);
    process_termination();
    p->state = STATE_EXIT;
  } else {
    p->state = STATE_EXIT;
    process_termination();
    p->last_drop_time = get_time_ms();
  }
}
void handle_move_left(GameState_t *p) {
  p->piece_x--;
  if (check_collision(p)) {
    p->piece_x++;
  }
}
void handle_move_right(GameState_t *p) {
  p->piece_x++;
  if (check_collision(p)) {
    p->piece_x--;
  }
}
void handle_soft_drop(GameState_t *p) {
  p->piece_y++;
  if (check_collision(p)) {
    p->piece_y--;
  }
}
void handle_rotate(GameState_t *p) {
  int old_rotation = p->rotation;
  int old_current[4][4];
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      old_current[i][j] = p->current[i][j];
    }
  }

  p->rotation = (p->rotation + 1) % 4;
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      p->current[i][j] = TETRIMINOS[p->piece_type][p->rotation][i][j];
    }
  }

  if (check_collision(p)) {
    p->rotation = old_rotation;
    for (int i = 0; i < 4; i++) {
      for (int j = 0; j < 4; j++) {
        p->current[i][j] = old_current[i][j];
      }
    }
  }
}
