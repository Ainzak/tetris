#include "tetris_backend.h"

#include "../../common/fsm.h"
#include "tetramino.h"
#include "tetris_states.h"

GameState_t g_state = {0};
static int render_field[FIELD_HEIGHT][FIELD_WIDTH];
static int *render_ptr_array[FIELD_HEIGHT];

static int *field_ptr_array[FIELD_HEIGHT];
static int *next_ptr_array[FIELD_WIDTH];
static bool buffers_initialized = false;
static bool game_initialized = false;

static void ensure_buffers_ready(void) {
  if (!buffers_initialized) {
    for (int i = 0; i < FIELD_HEIGHT; i++) {
      field_ptr_array[i] = g_state.field[i];
      render_ptr_array[i] = render_field[i];
    }
    for (int i = 0; i < 4; i++) {
      next_ptr_array[i] = g_state.next[i];
    }
    buffers_initialized = true;
  }
}

int check_collision(GameState_t *state) {
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      if (state->current[i][j] == 0) {
        continue;
      }

      int field_x = state->piece_x + j;
      int field_y = state->piece_y + i;

      if (field_x < 0 || field_x >= 10) {
        return 1;
      }

      if (field_y >= 20) {
        return 1;
      }

      if (field_y < 0) {
        continue;
      }

      if (state->field[field_y][field_x] != 0) {
        return 1;
      }
    }
  }
  return 0;
}

static bool soft_drop_held = false;

void process_automatic_transitions(GameState_t *state) {
  long long now = get_time_ms();
  switch (state->state) {
    case STATE_SPAWN: {
      int have_collision = spawn_figure(state);
      if (!have_collision) {
        state->state = STATE_MOVING;
        state->last_drop_time = now;
      } else {
        state->state = STATE_GAMEOVER;
        state->game_over = true;
      }
      break;
    }
    case STATE_MOVING: {
      int drop_interval = soft_drop_held ? (state->speed / 10) : state->speed;
      if (now - state->last_drop_time >= drop_interval) {
        state->piece_y++;
        if (check_collision(state)) {
          state->piece_y--;
          state->state = STATE_ATTACH;
        }
        state->last_drop_time = now;
      }
      break;
    }
    case STATE_ATTACH:
      lock_piece_to_field(state);
      int rows_cleared = clear_full_rows(state);
      update_score(state, rows_cleared);
      update_level(state);
      state->state = STATE_SPAWN;
      break;
    default:
      break;
  }
}

void userInput(UserAction_t action, bool hold) {
  if (action == Start) {
    if (!game_initialized) {
      initialize_tetris();
    }
  } else if (action == Terminate) {
    handle_exit_game(&g_state);
    return;
  }
  if (action == Down) soft_drop_held = hold;
  void (*handler)(GameState_t *) = fsm_table[g_state.state][action];
  if (handler) {
    handler(&g_state);
  }
}

GameInfo_t updateCurrentState(void) {
  ensure_buffers_ready();

  if (!g_state.paused && g_state.state != STATE_EXIT) {
    process_automatic_transitions(&g_state);
  }

  for (int y = 0; y < FIELD_HEIGHT; y++) {
    for (int x = 0; x < FIELD_WIDTH; x++) {
      render_field[y][x] = g_state.field[y][x];
    }
  }

  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      if (g_state.current[i][j] == 0) continue;
      int fx = g_state.piece_x + j;
      int fy = g_state.piece_y + i;
      if (fy >= 0 && fy < FIELD_HEIGHT && fx >= 0 && fx < FIELD_WIDTH) {
        render_field[fy][fx] = g_state.current[i][j];
      }
    }
  }

  GameInfo_t info = {0};
  info.field = render_ptr_array;
  info.next = next_ptr_array;
  info.score = g_state.score;
  info.high_score = g_state.high_score;
  info.level = g_state.level;
  info.speed = g_state.speed;
  info.pause = g_state.paused;

  return info;
}

int spawn_figure(GameState_t *game_state) {
  game_state->piece_x = 3;
  game_state->piece_y = 0;
  game_state->rotation = 0;
  game_state->piece_type = game_state->next_type;
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      game_state->current[i][j] = game_state->next[i][j];
    }
  }
  int collision = check_collision(game_state);
  if (collision == 0) {
    game_state->next_type = rand() % TETRIMINO_COUNT;
    for (int i = 0; i < 4; i++) {
      for (int j = 0; j < 4; j++) {
        game_state->next[i][j] = TETRIMINOS[game_state->next_type][0][i][j];
      }
    }
  }
  return collision;
}

void initialize_tetris() {
  if (!game_initialized) {
    srand((unsigned int)time(NULL));
    game_initialized = true;
    g_state.level = 1;
    g_state.speed = 1000;
    g_state.paused = 0;
    g_state.state = STATE_SPAWN;
    g_state.game_over = 0;
    g_state.score = 0;
    g_state.high_score = 0;
    FILE *f = fopen("brick_game/tetris/data/highscore.txt", "r");
    if (f) {
      fscanf(f, "%d", &g_state.high_score);
      fclose(f);
    }
    for (int i = 0; i < 20; i++) {
      for (int j = 0; j < 10; j++) {
        g_state.field[i][j] = 0;
      }
    }
    g_state.next_type = rand() % TETRIMINO_COUNT;
    for (int i = 0; i < 4; i++) {
      for (int j = 0; j < 4; j++) {
        g_state.next[i][j] = TETRIMINOS[g_state.next_type][0][i][j];
      }
    }
  }
}

void process_termination() {}

void update_score(GameState_t *state, int rows_cleared) {
  state->score += SCORE_TABLE[rows_cleared];
}

void update_level(GameState_t *state) {
  if (state->score / 600 + 1 >= 10) {
    state->level = 10;
  } else
    state->level = state->score / 600 + 1;
  state->speed = 1000 - (state->level - 1) * 90;
}

void handle_highscore(GameState_t *state) {
  if (state->score > state->high_score) {
    state->high_score = state->score;
    FILE *f = fopen("brick_game/tetris/data/highscore.txt", "w");
    if (f) {
      fprintf(f, "%d\n", state->high_score);
      fclose(f);
    }
  }
}

void lock_piece_to_field(GameState_t *state) {
  for (int i = 0; i < 4; ++i) {
    for (int j = 0; j < 4; ++j) {
      if (state->current[i][j] != 0) {
        int fx = state->piece_x + j;
        int fy = state->piece_y + i;
        if (fx >= 0 && fx < FIELD_WIDTH && fy >= 0 && fy < FIELD_HEIGHT) {
          state->field[fy][fx] = state->current[i][j];
        }
      }
    }
  }
}

int clear_full_rows(GameState_t *state) {
  int rows_cleared = 0;
  for (int y = FIELD_HEIGHT - 1; y >= 0; y--) {
    int is_full = 1;
    for (int x = 0; x < FIELD_WIDTH; x++) {
      if (state->field[y][x] == 0) {
        is_full = 0;
        break;
      }
    }
    if (is_full) {
      rows_cleared++;
      for (int row = y; row > 0; row--) {
        for (int x = 0; x < FIELD_WIDTH; x++) {
          state->field[row][x] = state->field[row - 1][x];
        }
      }
      for (int x = 0; x < FIELD_WIDTH; x++) {
        state->field[0][x] = 0;
      }

      y++;
    }
  }
  return rows_cleared;
}

long long get_time_ms(void) {
  struct timespec ts;
  clock_gettime(CLOCK_MONOTONIC, &ts);
  return ts.tv_sec * 1000LL + ts.tv_nsec / 1000000;
}
