.DEFAULT_GOAL := all
.PHONY : clean test all

CCFLAGS := -Wall -Wextra -Werror -pedantic-errors

novdso : novdso.c
	cc $(CFLAGS) -o $@ $<

test : novdso
	cd ./test/ && make

clean :
	-rm novdso
	-cd ./test/ && make clean

all : novdso test
