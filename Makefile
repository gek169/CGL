.ONESHELL:
CC=gcc
CFLAGS=-I.
ALL_T= water demo sprite tilemap wavy lines linemodel triangles c3d palh trippy binbrowse imgdumper dhmap phystest citygen
SUB_T= OGL S3L lua
OPTIMIZATION_LEVEL=-O3 -g
SDL2_LIBS= -lSDL2 -lSDL2_mixer -lSDL2main
#SDL2_LIBS= -lSDL -lSDL_mixer -lSDLmain
OTHER_LIBS=
all: $(ALL_T) $(SUB_T)
clean:
	rm -f $(ALL_T)
	rm -f *.exe
	cd OGL && $(MAKE) clean && cd ..
	cd S3L && $(MAKE) clean && cd ..
	cd lua && $(MAKE) clean && cd ..
water:
	$(CC) water.c -o water -lm $(SDL2_LIBS) -std=c99 $(OPTIMIZATION_LEVEL)
dhmap:
	$(CC) dhmap.c -o dhmap -lm $(SDL2_LIBS) -std=c99 $(OPTIMIZATION_LEVEL)
demo:
	$(CC) demo.c -o demo -lm $(SDL2_LIBS) -std=c99 $(OPTIMIZATION_LEVEL)
imgdumper:
	$(CC) imgdumper.c -o imgdumper -lm $(SDL2_LIBS) -std=c99 $(OPTIMIZATION_LEVEL)
binbrowse:
	$(CC) binbrowse.c -o binbrowse -lm $(SDL2_LIBS) -std=c99 $(OPTIMIZATION_LEVEL)
sprite:
	$(CC) spriterendering.c -o sprite -lm $(SDL2_LIBS) -std=c99 $(OPTIMIZATION_LEVEL)
trippy:
	$(CC) trippy.c -o trippy -lm $(SDL2_LIBS) -std=c99 $(OPTIMIZATION_LEVEL)
tilemap::
	$(CC) tilemap.c -o tilemap -lm $(SDL2_LIBS) -std=c99 $(OPTIMIZATION_LEVEL)
lines:
	$(CC) lines.c -o lines -lm $(SDL2_LIBS) -std=c99 $(OPTIMIZATION_LEVEL)
citygen:
	$(CC) citygen.c -o citygen -lm $(SDL2_LIBS) -std=c99 $(OPTIMIZATION_LEVEL)
triangles:
	$(CC) triangles.c -o triangles -lm $(SDL2_LIBS) -std=c99 $(OPTIMIZATION_LEVEL)
phystest:
	$(CC) phystest.c -o phystest -lm $(SDL2_LIBS) -std=c99 $(OPTIMIZATION_LEVEL)
c3d:
	$(CC) c3d.c -o c3d -lm $(SDL2_LIBS) -std=c99 $(OPTIMIZATION_LEVEL)
wavy:
	$(CC) wavy.c -o wavy -lm $(SDL2_LIBS) -std=c99 $(OPTIMIZATION_LEVEL)
linemodel:
	$(CC) linemodel.c -o linemodel -lm $(SDL2_LIBS) -std=c99 $(OPTIMIZATION_LEVEL)
palh:
	$(CC) palh.c -o palh 
OGL:
	cd OGL && $(MAKE) && cd ..
S3L:
	cd S3L && $(MAKE) && cd ..
lua:
	cd lua && $(MAKE) && cd ..
TGL:
	cd TGL && $(MAKE) && cd ..
	
