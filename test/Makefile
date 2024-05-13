.PHONY: clean all test

CFLAGS=-I.. -g -O2 -std=c99

tinyobj_%_tests: tinyobj_%_tests.c ../tinyobj_loader_c.h acutest.h
	$(CC) $(CFLAGS) -o $@ $^

run_%_tests: tinyobj_%_tests
	@echo Running $* tests:
	@./$^

all: test

tinyobj_tests: tinyobj_internal_tests tinyobj_api_tests tinyobj_regression_tests

test: run_internal_tests run_api_tests run_regression_tests

clean:
	@rm -rf *.o
	@rm -rf tinyobj_internal_tests tinyobj_api_tests tinyobj_regression_tests
