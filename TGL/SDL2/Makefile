.ONESHELL:
CC=gcc
CFLAGS=-std=c99
ALL_T= gl_helpers.o immtri camera model
OPTIMIZATION_LEVEL=-O3
#-fsanitize=undefined
SDL2_LIBS= -lSDL2 -lSDL2_mixer -lSDL2main
OTHER_LIBS= -L../ -lTinyGL
all: $(ALL_T)
clean:
	rm -f $(ALL_T)
	rm -f *.exe
gl_helpers.o:
	gcc -c gl_helpers.c $(CFLAGS) $(INCLUDES) $(OTHER_LIBS) $(OPTIMIZATION_LEVEL) -std=c99
immtri: gl_helpers.o
	gcc immtri.c gl_helpers.o -o immtri -lm $(CFLAGS) $(INCLUDES) $(OTHER_LIBS) $(SDL2_LIBS) $(OPTIMIZATION_LEVEL)
camera: gl_helpers.o
	gcc camera.c gl_helpers.o -o camera -lm $(CFLAGS) $(INCLUDES) $(OTHER_LIBS) $(SDL2_LIBS) $(OPTIMIZATION_LEVEL)
model: gl_helpers.o
	gcc model.c gl_helpers.o -o model -lm $(CFLAGS) $(INCLUDES) $(OTHER_LIBS) $(SDL2_LIBS) $(OPTIMIZATION_LEVEL)