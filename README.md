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

## License

MIT license.

## TODO

* [ ] Testing.
* [ ] Windows build
