#include <stdarg.h>
#include <string.h>
#include <stdlib.h>

#include <signal.h>
#include <sys/time.h>
#include <time.h>
#include <unistd.h>

#include <ncurses.h>
#include <locale.h>

#include "02-02.h"

void putstr(int row, int col, const char *str, ...) {
	int x = getcurx(curscr);
	int y = getcury(curscr);

	move(row, col);
	va_list va;
	va_start(va, str);
	vwprintw(stdscr, str, va);
	va_end(va);

	move(y, x);
	refresh();
}

static spinner_t *pspinner = &spinners[0];
static unsigned int ispinner = 0;
static unsigned int offset = 0;
static char fmt[32];
static unsigned int spcol = 0;

#define MOVE(row) { \
	move((row)-offset, spcol+2); \
	refresh(); \
}

void alarm_handler(int sig) {
	if (pspinner->frames[pspinner->iframe + 1]) {
		pspinner->iframe++;
	} else {
		pspinner->iframe = 0;
	}

	putstr(ispinner-offset, 0, fmt, pspinner->name, pspinner->frames[pspinner->iframe]);
	putstr(ispinner-offset, 60, "%-3d %-2d %-2d", ispinner, pspinner->iframe, strlen(pspinner->frames[pspinner->iframe]));
	putstr(5, 80, "%-5d %-6d %-5d %-5d", ispinner, offset, LINES, nspinner);

	refresh();
}

int main(int argc, char *argv[]) {
	int ch, i, n, j, N = 0;
	spinner_t *p;
	struct itimerval itv;

	signal(SIGALRM, alarm_handler);

	setlocale(LC_ALL, "");
	initscr();
	cbreak();
	noecho();
	keypad(stdscr, TRUE);

	for (i = 0; i < nspinner; i++) {
		p = &spinners[i];
		n = strlen(p->name);
		if(n > spcol)
			spcol = n;
		for (j = 0; p->frames[j]; j++) {
			n = strlen(p->frames[j]);
			if(n > N)
				N = n;
		}
	}

	sprintf(fmt, "%%%ds [ ] %%-%ds", spcol, N);
	putstr(0, 80, "fmt: %s", fmt);
	putstr(3, 80, "Index Offset LINES TOTAL");
	putstr(4, 80, "------------------------");
	putstr(6, 80, "------------------------");
	putstr(7, 80, "编号  偏移量 行数  总数");

refresh:
	for (i = offset, n = 0; i < nspinner && n < LINES; i++,n++) {
		p = &spinners[i];
		putstr(n, 0, fmt, p->name, p->frames[p->iframe]);
		putstr(n, 60, "%-3d %-2d %-2d", i, p->iframe, strlen(p->frames[p->iframe]));
	}
	MOVE(ispinner);

timer:
	itv.it_interval.tv_sec = 0;
	itv.it_interval.tv_usec = pspinner->interval;

	itv.it_value.tv_sec = 0;
	itv.it_value.tv_usec = pspinner->interval;

	setitimer(ITIMER_REAL, &itv, NULL);

	while ((ch = getch()) != KEY_F(1)) {
		j = ispinner;
		switch (ch) {
			case KEY_DOWN:
				if (j + 1 < nspinner)
					j++;
				else
					j = 0;
				break;
			case KEY_UP:
				if (j > 0)
					j--;
				else
					j = nspinner - 1;
				break;
			default:
				goto end;
				break;
		}

		i = j-offset;

		if(i >= LINES) {
			offset += i-LINES+1;
		} else if(i < 0) {
			offset += i;
		} else {
			ispinner = j;
			pspinner = &spinners[ispinner];
			MOVE(ispinner);
			goto timer;
		}

		ispinner = j;
		pspinner = &spinners[ispinner];
		goto refresh;
	}

end:
	endwin();

	return 0;
}
