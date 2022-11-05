.PHONY : test clean

CCFLAGS := -Wall -Wextra -Werror -pedantic-errors

EXPECT_INSTALLED := $(shell command -v expect 2> /dev/null)
STRACE_INSTALLED := $(shell command -v strace 2> /dev/null)

test : time
ifeq ($(and $(EXPECT_INSTALLED),$(STRACE_INSTALLED)),)
	@echo "Missing 'expect' or 'strace', tests skipped!"
else
	@echo "Writing test log to '$(CURDIR)/test_out.txt'"
	@echo "Test vdso..."
	@./test_vdso > test_out.txt
	@echo "Test novdso..."
	@NOVDSO_PTRACER_ANY= ./test_novdso >> test_out.txt
	@echo "Success!"
endif

time : time.c
	cc $(CCFLAGS) -o $@ $<

clean :
	-rm test_out.txt
	-rm time
