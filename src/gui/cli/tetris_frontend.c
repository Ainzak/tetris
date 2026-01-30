#include "tetris_frontend.h"

void render_game(GameInfo_t *info) {
  clear();

  mvprintw(0, 0, "+----------+");
  for (int i = 1; i <= FIELD_HEIGHT; ++i) {
    mvprintw(i, 0, "|");
    mvprintw(i, FIELD_WIDTH + 1, "|");
  }
  mvprintw(FIELD_HEIGHT + 1, 0, "+----------+");

  for (int i = 0; i < FIELD_HEIGHT; ++i) {
    for (int j = 0; j < FIELD_WIDTH; ++j) {
      char cell = (info->field[i][j] != 0) ? '#' : '.';
      mvprintw(i + 1, j + 1, "%c", cell);
    }
  }

  int sidebar_x = FIELD_WIDTH + 4;
  mvprintw(1, sidebar_x, "SCORE: %d    ", info->score);
  mvprintw(2, sidebar_x, "HIGH:  %d    ", info->high_score);
  mvprintw(3, sidebar_x, "LEVEL: %d    ", info->level);

  mvprintw(5, sidebar_x, "NEXT:");
  for (int i = 0; i < 4; ++i) {
    for (int j = 0; j < 4; ++j) {
      char cell = (info->next[i][j] != 0) ? '#' : ' ';
      mvprintw(6 + i, sidebar_x + j, "%c", cell);
    }
  }

  mvprintw(12, sidebar_x, "CONTROLS:");
  mvprintw(13, sidebar_x, "A/D - Move");
  mvprintw(14, sidebar_x, "S - Drop");
  mvprintw(15, sidebar_x, "W/Space - Rotate");
  mvprintw(16, sidebar_x, "P - Pause");
  mvprintw(17, sidebar_x, "Q - Quit");

  if (info->pause) {
    mvprintw(9, 2, " PAUSED ");
    mvprintw(10, 2, " P=Resume");
  }
  refresh();
}