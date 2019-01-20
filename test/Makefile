.PHONY: clean all test ansicompile

CFLAGS=-I.. -g -O0 -std=c99

all: test ansicompile

tinyobj_tests: tinyobj_tests.o tinyobj_internal_tests.o tinyobj_api_tests.o tinyobj_regression_tests.o

tinyobj_internal_tests.o: tinyobj_internal_tests.c tinyobj_internal_tests.h ../tinyobj_loader_c.h acutest.h

tinyobj_api_tests.o: tinyobj_api_tests.c tinyobj_api_tests.h ../tinyobj_loader_c.h acutest.h

tinyobj_regression_tests.o: tinyobj_regression_tests.c tinyobj_regression_tests.h ../tinyobj_loader_c.h acutest.h

test: tinyobj_tests
	@./tinyobj_tests

ansicompile: ../tinyobj_loader_c.h
	$(CC) -I.. -g -O0 -ansi -Wall -Wextra -Wshadow -Wconversion -Wformat=2 -pedantic -DTINYOBJ_LOADER_C_IMPLEMENTATION -fsyntax-only $<

clean:
	@rm -rf tinyobj_tests tinyobj_tests.o tinyobj_internal_tests.o tinyobj_api_tests.o tinyobj_regression_tests.o
