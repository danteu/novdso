novdso:
	cc -Wall -Wextra -Werror -pedantic-errors -o novdso novdso.c

clean:
	-rm novdso

all: novdso
