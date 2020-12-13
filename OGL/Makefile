.ONESHELL:
CC=gcc
CFLAGS=-I.
ALL_T= gl3w.o shader.o gl_helpers.o immtri camera model
OPTIMIZATION_LEVEL=-O3
SDL2_LIBS= -lSDL2 -lSDL2_mixer -lSDL2main
ifeq ($(OS),Windows_NT)
OTHER_LIBS= -lopengl32
else
OTHER_LIBS= -lGL
endif
all: $(ALL_T)
clean:
	rm -f $(ALL_T)
gl3w.o:
	gcc -c gl3w.c $(OTHER_LIBS) $(OPTIMIZATION_LEVEL) -std=c99
gl_helpers.o:
	gcc -c gl_helpers.c $(OTHER_LIBS) $(OPTIMIZATION_LEVEL) -std=c99
shader.o:
	gcc -c shader.c $(OTHER_LIBS) $(OPTIMIZATION_LEVEL) -std=c99
immtri: gl_helpers.o
	gcc immtri.c gl_helpers.o -o immtri -lm $(OTHER_LIBS) $(SDL2_LIBS) $(OPTIMIZATION_LEVEL)
camera: gl_helpers.o
	gcc camera.c gl_helpers.o -o camera -lm $(OTHER_LIBS) $(SDL2_LIBS) $(OPTIMIZATION_LEVEL)
model: gl_helpers.o
	gcc model.c gl_helpers.o -o model -lm $(OTHER_LIBS) $(SDL2_LIBS) $(OPTIMIZATION_LEVEL)