# How to run tests.

## Cmake

```
$ mkdir build
$ cd build
$ cmake ..
$ make
```

Please run binary in this folder(not in `build`) since asset file path are set relative to this folder.

```
$ cd ..
$ ./build/tinyobjloader_tests
```

## Makefile

Edit `Makefile`, then

```
$ make
$ ./tinyobj_tests
```
