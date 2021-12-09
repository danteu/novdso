.PHONY : test clean

CCFLAGS := -Wall -Wextra -Werror -pedantic-errors

test : time
	@echo "Writing test log to './test_out.txt'"
	@echo "Test vdso..."
	@./test_vdso > test_out.txt
	@echo "Test novdso..."
	@./test_novdso >> test_out.txt
	@echo "Success!"

time : time.c
	cc $(CCFLAGS) -o $@ $<

clean :
	-rm test_out.txt
	-rm time
