#include <curses.h>
#include <form.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

// 通过关闭域的O_ACTIVE属性创建标签
FIELD* make_label(int frow, int fcol, char *label) {
	FIELD *f = new_field(1, strlen(label), frow, fcol, 0, 0);

	if (f) {
		set_field_buffer(f, 0, label);
		set_field_opts(f, field_opts(f) & ~O_ACTIVE);
	}

	return f;
}

// 通过打开域的O_UNDERLINE属性创建输入域
FIELD *make_field(int frow, int fcol, int cols) {
	FIELD *f = new_field(1, cols, frow, fcol, 0, 0);

	if (f)
		set_field_back(f, A_UNDERLINE);

	return f;
}

int main(int argc, char *argv[]) {
	FORM *form;
	FIELD *f[6];
	int i = 0;

	initscr();
	nonl();
	raw();
	noecho();
	wclear(stdscr);

	f[0] = make_label(0, 7, "Sample form demo");
	f[1] = make_label(2, 0, "Field1:");
	f[2] = make_field(2, 9, 16);
	f[3] = make_label(3, 0, "Field2:");
	f[4] = make_field(3, 9, 16);
	f[5] = (FIELD*) 0;

	form = new_form(f);

	post_form(form);

	wrefresh(stdscr);

	sleep(3);

	unpost_form(form);

	wrefresh(stdscr);

	free_form(form);

	while (f[i])
		free_field(f[i++]);

	endwin();
	exit(0);
}
