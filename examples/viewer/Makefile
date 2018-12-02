.PHONY: clean all

# OS specific linker flags for GLFW
UNAME_S := $(shell uname -s)
ifeq ($(UNAME_S),Darwin)
LDFLAGS=-framework OpenGL -lglfw3 -lglew -framework Cocoa -framework IOKit -framework CoreVideo
endif
ifeq ($(UNAME_S),Linux)
LDFLAGS=-lGL -lGLU -lglfw3 -lGLEW -lX11 -lXrandr -lXinerama -lXxf86vm -lXcursor -lm -pthread -ldl
endif
CFLAGS=-Weverything -pedantic -ansi -O2 -g -Wno-disabled-macro-expansion -Wno-deprecated-declarations

all: viewer

viewer: viewer.o trackball.o

viewer.o: viewer.c ../../tinyobj_loader_c.h

trackball.o: trackball.c trackball.h

clean:
	@rm -rf viewer viewer.o trackball.o
