#include <curses.h>
#include <form.h>
#include <stdarg.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

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

int main(int argc, char *argv[]) {
	FIELD *field[4];
	FORM *my_form;
	char buffer[21];
	int ch;

	initscr();
	cbreak();
	noecho();
	keypad(stdscr, TRUE);

	field[0] = new_field(1, 20, 4, 18, 0, 20);
	field[1] = dup_field(field[0], 6, 18);
	field[2] = link_field(field[0], 8, 18);
	field[3] = NULL;

	set_field_back(field[0], A_UNDERLINE);
	set_field_back(field[1], A_UNDERLINE);
	field_opts_off(field[2], O_ACTIVE);

	my_form = new_form(field);

	post_form(my_form);

	refresh();

	mvprintw(4, 9, "Field1:");
	mvprintw(6, 9, "Field2:");
	mvprintw(8, 9, "Field3:");
	mvprintw(10, 9, " Value:");

	mvprintw(LINES - 30, 10, "Use the arrow keys to move!");

	refresh();

	// set_current_field(my_form, field[0]);

	form_driver(my_form, REQ_FIRST_FIELD);

	while ((ch = getch()) != KEY_F(1)) {
		switch (ch) {
			/*如果是↓键或者或者回车键,将移到下一个表单域*/
			case KEY_DOWN:
			case 10: //回车键
				form_driver(my_form, REQ_VALIDATION);
				{
					FIELD *f = current_field(my_form);
					char *str = field_buffer(f, 0);
					char *p;
					strcpy(buffer, str);
					p = buffer + strlen(buffer) - 1;
					while (*p == ' ')
						*p-- = '\0';
					putstr(10, 18, "%s,%s,%d,%d", str, buffer, field_index(f), field_pad(f));
				}
				form_driver(my_form, REQ_NEXT_FIELD);
				break;

				/*如果是↑,则移动到上一个表单域*/
			case KEY_UP:
				form_driver(my_form, REQ_PREV_FIELD);
				break;

			case KEY_LEFT:
				form_driver(my_form, REQ_PREV_CHAR);
				break;

			case KEY_RIGHT:
				form_driver(my_form, REQ_NEXT_CHAR);
				break;

			case KEY_HOME:
				while (!form_driver(my_form, REQ_PREV_CHAR))
					;
				break;

			case KEY_END:
				while (!form_driver(my_form, REQ_NEXT_CHAR))
					;
				break;

			case KEY_DC:
				form_driver(my_form, REQ_DEL_CHAR);
				break;

			case KEY_BACKSPACE:
#if 1
				if (!form_driver(my_form, REQ_PREV_CHAR))
					form_driver(my_form, REQ_DEL_CHAR);
#else
				form_driver(my_form, REQ_DEL_PREV); // 有Bug，到第一个字符后，再执行该操作光标会跳转到下一个字段
#endif
				break;

				/*如果是其余的字符,则打印*/
			default:
				form_driver(my_form, ch);
				break;
		}
	}

	unpost_form(my_form);

	free_form(my_form);

	free_field(field[0]);

	free_field(field[1]);

	free_field(field[2]);

	endwin();

	return 0;
}
