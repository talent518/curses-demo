CC := gcc
CFLAGS := $(CFLAGS)
LFLAGS_NW := $(LFLAGS) -lncursesw
LFLAGS := $(LFLAGS) -lcurses
LFLAGS_FORM := $(LFLAGS) -lform

NODE := node

all: 01-01
01-01: 01-01.o
	@echo LD $@
	@$(CC) -o $@ $^ $(LFLAGS)

all: 01-02
01-02: 01-02.o
	@echo LD $@
	@$(CC) -o $@ $^ $(LFLAGS)

all: 01-03
01-03: 01-03.o
	@echo LD $@
	@$(CC) -o $@ $^ $(LFLAGS)

all: 01-04
01-04: 01-04.o
	@echo LD $@
	@$(CC) -o $@ $^ $(LFLAGS_FORM)

all: 01-05
01-05: 01-05.o
	@echo LD $@
	@$(CC) -o $@ $^ $(LFLAGS_FORM)

all: 02
02: 02-01.o 02-02.o
	@echo LD $@
	@$(CC) -o $@ $^ $(LFLAGS_NW)

02-02.o: 02-02.h
02-02.c:
	@echo MK $@
	@$(NODE) 02-02.js

%.o: %.c
	@echo CC $<
	@$(CC) $(CFLAGS) -o $(@:.o=.s) -S $<
	@$(CC) $(CFLAGS) -o $(@:.o=.e) -E $<
	@$(CC) $(CFLAGS) -c $< -o $@

%.O: %.c
	@echo CC $<
	@$(CC) $(CFLAGS) -fpic -fPIC -o $(@:.O=.S) -S $<
	@$(CC) $(CFLAGS) -fpic -fPIC -o $(@:.O=.E) -E $<
	@$(CC) $(CFLAGS) -fpic -fPIC -c $< -o $@

clean: .gitignore
	@echo $@
	@LANG=en rm -vf $(shell cat .gitignore)

.gitignore:
	@$(MAKE) ignored

ignored:
	@echo $@
	@echo '*.o' > .gitignore
	@echo '*.O' >> .gitignore
	@echo '*.s' >> .gitignore
	@echo '*.S' >> .gitignore
	@echo '*.e' >> .gitignore
	@echo '*.E' >> .gitignore
	@cat Makefile | awk '{if($$1=="all:") print $$2;}' >> .gitignore
	@echo '02-02.c' >> .gitignore
	@-svn propset svn:ignore -F .gitignore .
