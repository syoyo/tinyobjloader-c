# tinyobjloader in C

Tiny but powerful header only wavefront obj loader written in C89.

If you are looking for C++ version, please see https://github.com/syoyo/tinyobjloader

## Current status

Experimental. Loading geometry data would be OK, More testing required for materials and shapes.

## Features

* Single file, header only 
* Dependency-free pure C89 implementation(except for `libc`)
* Provides mostly similar functionality as in C++ version. https://github.com/syoyo/tinyobjloader
* Moderate speed and moderate memory consumption
  * Rungholt 7M triangle scene(260 MB) can be loaded in 4.7 secs and consumes 3.6 GB memory at the maximum on MacBook12 Core m5 1.2 GHz(single core use)


## Usage

Copy `tinyobj_loader_c.h` to your project.

```
/* define TINYOBJ_LOADER_C_IMPLEMENTATION for only *one* .c */
#define TINYOBJ_LOADER_C_IMPLEMENTATION
#include "tinyobj_loader_c.h"

...

```

See `examples/viewer/` for more details.

## Tests

The single header test library [acutest](https://github.com/mity/acutest) is used to provide a test runner and assertion macros. There are two test suites: one for the API and one for the internal functions. Ultimately the internal tests should be removed, but are useful while the project is volatile.

The tests can be run from the project root using:

```
$ make test
```

This builds and executes a binary called `tinyobj_tests` in the test folder. There are some options to run specific tests that can be passed to the executable directly that are described on the [acutest readme](https://github.com/mity/acutest#running-unit-tests).

By default acutest forks for each test. To disable this for debugging purposes, you can pass the switch `--no-exec` to `tinyobj_tests`.

## License

MIT license.

## TODO

* [ ] Testing.
* [ ] Windows build
