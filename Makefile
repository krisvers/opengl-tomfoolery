CC = gcc
WINCC = i686-w64-mingw32-gcc
CCFLAGS = -std=c99 -Wall -Wextra -Wpedantic -Iinclude
WINFLAGS = -DGLFW_DLL -DGLEW_STATIC
CCLIBS = -lGL -lglfw -lGLEW
WINLIBS = -Llib -lglfw3 -lglew32s -lopengl32 -lgdi32 -lkernel32 -lmingw32

CFILES = $(shell find . -type f -name "*.c")
OUTFILE = ./exe

all:
	$(CC) $(CFILES) -o $(OUTFILE) $(CCFLAGS) $(CCLIBS)

win:
	$(WINCC) $(CFILES) -o $(OUTFILE).exe $(CCFLAGS) $(WINFLAGS) $(WINLIBS)
