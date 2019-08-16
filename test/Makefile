.PHONY: clean all test

CFLAGS=-I.. -g -O0 -std=c99

all: test

tinyobj_tests: tinyobj_tests.o tinyobj_internal_tests.o tinyobj_api_tests.o tinyobj_regression_tests.o

tinyobj_internal_tests.o: tinyobj_internal_tests.c tinyobj_internal_tests.h ../tinyobj_loader_c.h acutest.h

tinyobj_api_tests.o: tinyobj_api_tests.c tinyobj_api_tests.h ../tinyobj_loader_c.h acutest.h

tinyobj_regression_tests.o: tinyobj_regression_tests.c tinyobj_regression_tests.h ../tinyobj_loader_c.h acutest.h

test: tinyobj_tests
	@./tinyobj_tests

# ansicomile does not work anymore since the code contains C99 syntax(`-ansi` cannot be used for C99 code)
#ansicompile: ../tinyobj_loader_c.h
#	$(CC) -I.. -g -O0 -ansi -Wall -Wextra -Wshadow -Wconversion -Wformat=2 -pedantic -DTINYOBJ_LOADER_C_IMPLEMENTATION -fsyntax-only $<

clean:
	@rm -rf tinyobj_tests tinyobj_tests.o tinyobj_internal_tests.o tinyobj_api_tests.o tinyobj_regression_tests.o
