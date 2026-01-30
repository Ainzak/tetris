#ifndef TETRIS_BACKEND_H
#define TETRIS_BACKEND_H
#define _GNU_SOURCE

#include <ncurses.h>
#include <stdlib.h>
#include <sys/time.h>
#include <time.h>

#include "../../common/fsm.h"
#include "defines.h"
#include "tetris_states.h"

extern GameState_t g_state;
extern void (*fsm_table[STATE_COUNT][ACTION_COUNT])(GameState_t *);
int spawn_figure(GameState_t *game_state);
void initialize_tetris();
void process_termination();
void process_automatic_transitions(GameState_t *state);
void handle_highscore(GameState_t *state);
int check_collision(GameState_t *state);
void lock_piece_to_field(GameState_t *);
int clear_full_rows(GameState_t *);
long long get_time_ms(void);
void update_score(GameState_t *state, int rows_cleared);
void update_level(GameState_t *state);

#endif