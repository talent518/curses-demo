#include <curses.h>
#include <signal.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

static void finish(int sig);

static void showmenu(int index);

#define SLT_LEN 5
static int selected_num = 0;
static int selected_menus[SLT_LEN] = {-1,-1,-1,-1,-1};
static char *menus[] = {"Java", "C", "Python", "C++", "(Visual) Basic.NET", "JavaScript", "C#", "PHP", "SQL", "Objective-C", "MATLAB", "R", "Perl", "Assembly language", "Swift", "GO", "Delphi/Object Pascal", "Ruby", "PL/SQL", "Visual Basic", NULL};
#define N (sizeof(menus)/sizeof(char*))
static int LL;

int main(int argc, char* *argv) {
	(void) signal(SIGINT, finish);

	initscr(); // 初始化 curses包

	clear();

	keypad(stdscr, TRUE); // 允许键盘映射

	(void) nonl();

	(void) cbreak();

	(void) noecho();

	// 判断是否支持彩色

	if (has_colors()) {
		start_color();

		use_default_colors();

		// 初始化颜色配对表
		init_pair(0, COLOR_BLACK, -1);
		init_pair(1, COLOR_RED, -1);
		init_pair(2, COLOR_GREEN, -1);
		init_pair(3, COLOR_YELLOW, -1);
		init_pair(4, COLOR_BLUE, -1);
		init_pair(5, COLOR_MAGENTA, -1);
		init_pair(6, COLOR_CYAN, -1);
		init_pair(7, COLOR_WHITE, -1);
	}

	char *colors[] = {"Black", "Red", "Green", "Yellow", "Blue", "Magenta", "Cyan", "White", NULL};
	int i;
	for(i=0; colors[i]; i++) {
		attron(A_BLINK|COLOR_PAIR(i));
		move(i, 0);
		addstr(colors[i]);
	}
	refresh();

	attron(A_BLINK|COLOR_PAIR(1));
	move(0, COLS-9);
	addstr("Right Top");
	refresh();

	attron(A_BLINK|COLOR_PAIR(3));
	move(LINES-1, 0);
	addstr("Left Bottom");
	refresh();

	attron(A_BLINK|COLOR_PAIR(2));
	move(LINES-1, COLS-12);
	addstr("Right Bottom");
	refresh();

	attron(A_BLINK|COLOR_PAIR(2));
	move(LINES / 2 + 1, COLS - 4);
	addstr("Eye");
	refresh();

	sleep(2);

	attron(A_BLINK|COLOR_PAIR(3));
	move(LINES / 2 - 3, COLS / 2 - 3);
	addstr("Bulls");
	refresh();

	LL = LINES - 1 - N;
	showmenu(-1);
	i = 0;
	while(selected_num < SLT_LEN)
		switch(getch()) {
		case KEY_UP:
			if(i > 0)
				i--;
			else
				i = N - 1;
			move(LL + i, 1);
			refresh();
			break;
		case KEY_DOWN:
			if(i+1 < N)
				i++;
			else
				i = 0;
			move(LL + i, 1);
			refresh();
			break;
		case ' ':
			showmenu(i);
			break;
		}

	attron(A_BLINK|COLOR_PAIR(1));
	move(LINES-1, COLS / 2 - 17);
	addstr("Press any key to exit the program.");
	refresh();

	getch();

	finish(0);
	return 0;
}

static void finish(int sig) {
	endwin();

	exit(0);
}

static void showmenu(int index) {
	int j, L, s;
	L = LL;

	if(index > -1) {
		for(s=0; s<SLT_LEN; s++)
			if(selected_menus[s] == index)
				break;

		if(s == SLT_LEN) {
			for(s=0; s<SLT_LEN && selected_menus[s] != -1; s++);
			if(s < SLT_LEN) {
				selected_menus[s] = index;
				selected_num++;
			}
		} else {
			selected_menus[s] = -1;
			selected_num--;
		}
	}

	attron(A_BLINK|COLOR_PAIR(1));
	move(L-1, 0);
	addstr("Please select 5 programming languages you like:");

	attron(A_BLINK|COLOR_PAIR(4));
	for(j=0; menus[j]; j++) {
		for(s=0; s<SLT_LEN; s++)
			if(selected_menus[s] == j)
				break;
		move(L++, 0);
		printw("[%c] %s", s == SLT_LEN ? ' ' : '+', menus[j]);
	}
	if(selected_num) {
		attron(A_BLINK|COLOR_PAIR(2));
		move(L, 0);
		printw("%d selected, including: ", selected_num);
		int first = 1;
		for(s=0; s < SLT_LEN; s++) {
			if(selected_menus[s] != -1) {
				if(first)
					first = 0;
				else
					addstr(", ");
				addstr(menus[selected_menus[s]]);
			}
		}
		addstr("                    ");
	}

	if(index == -1)
		move(LL, 1);
	else
		move(LL+index, 1);
	refresh();
}
