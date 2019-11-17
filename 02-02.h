#ifndef _02_02_H
#define _02_02_H

#include <unistd.h>

typedef struct _spinner_t {
	const char *name;
	useconds_t interval;
	const char * const * frames;
	int iframe;
} spinner_t;

extern spinner_t spinners[];
extern unsigned int nspinner;

#endif
