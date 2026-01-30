#include "tetris_backend.h"
#include "tetris_frontend.h"
#include "tetris_states.h"

int main() {
  initscr();
  cbreak();
  noecho();
  keypad(stdscr, TRUE);
  nodelay(stdscr, TRUE);
  curs_set(0);

  while (1) {
    int ch = getch();
    if (ch == '\n' || ch == KEY_ENTER || ch == 's' || ch == 'S') {
      userInput(Start, false);
      GameInfo_t info = updateCurrentState();
      render_game(&info);
      break;
    }
    if (ch == 'q' || ch == 'Q') {
      endwin();
      return 0;
    }
    napms(50);
  }

  bool down_pressed = false;
  while (g_state.state != STATE_EXIT) {
    int ch = getch();
    switch (ch) {
      case 'q':
      case 'Q':
        userInput(Terminate, false);
        break;
      case KEY_LEFT:
      case 'a':
      case 'A':
        userInput(Left, false);
        break;
      case KEY_RIGHT:
      case 'd':
      case 'D':
        userInput(Right, false);
        break;
      case KEY_DOWN:
      case 's':
      case 'S':
        userInput(Down, true);
        down_pressed = true;
        break;
      case KEY_UP:
      case 'w':
      case 'W':
      case ' ':
        userInput(Action, false);
        break;
      case 'p':
      case 'P':
        userInput(Pause, false);
        break;
      case '\n':
      case KEY_ENTER:
        userInput(Start, false);
        break;
      case ERR:

        if (down_pressed) {
          userInput(Down, false);
          down_pressed = false;
        }
        break;
    }
    GameInfo_t info = updateCurrentState();
    render_game(&info);
    if (g_state.state == STATE_GAMEOVER) {
      mvprintw(8, 1, "          ");
      mvprintw(9, 1, " GAME OVER");
      mvprintw(10, 1, "          ");
    }
    refresh();
    napms(16);
  }
  endwin();
  return 0;
}
