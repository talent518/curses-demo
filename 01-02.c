#include <curses.h>
#include <stdio.h>
#include <stdlib.h>

#define StartX 1
#define StartY 1

void initial();

int main(int argc, char *argv[]) {
	int x = StartX;
	int y = StartY;
	int ch;
	initial();
	box(stdscr, '|', '-');
	attron(A_REVERSE);
	mvaddstr(0, 20, "curses program");
	attroff(A_REVERSE);
	move(x, y);
	do {
		ch = getch();
		switch (ch) {
		case KEY_UP:
			--y;
			break;
		case KEY_DOWN:
			++y;
			break;
		case KEY_RIGHT:
			++x;
			break;
		case KEY_LEFT:
			if(x>1) --x;
			break;
		case '\r':
			++y;
			x = 1;
			break;
		case '\t':
			x += 7;
			break;
		case KEY_BACKSPACE:
			if(x>1) mvaddch(y, --x, ' ');
			break;
		case 27:
			endwin();
			exit(1);
		default:
			addch(ch);
			x++;
			break;
		}
		move(y, x);
	} while (1);

	return 0;
}

void initial() {
	initscr();
	cbreak();
	nonl();
	noecho();
	intrflush(stdscr, FALSE);
	keypad(stdscr, TRUE);
	refresh();
}
