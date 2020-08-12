novdso:
	cc -Wall -Wextra -pedantic -g -o novdso novdso.c

clean:
	-rm novdso

all: novdso
