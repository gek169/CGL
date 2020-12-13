#include<stdio.h>
#include<stdlib.h>
/*
PALETTE HELPER
This program assists in the usage of the standard palette
as loaded by lSPal in api.h to create sprites.
the space character is blank. Alpha zero.
the next character is R * 0 + G * 0 + B * 1
the next character is R0 G1 B0
next is R0 G1 B1...
LSPAL loads a 64 color palette (+transparent, 65th) which maps to ascii characters.
333 would represent 255,255,255 and 000 would represent all black.
The space character + 1 is the first "real" color
(Space character is transparent)

The R,G, and B base-4 "digits" are each multiplied by 85 to get the "actual"
RGB values to be displayed on the screen.
*/
int main(int argc, char** argv){
	int pal[3];
	for(int i = 1; i < argc; i++)
		pal[i-1]=atoi(argv[i]);
	//for(int i = 0; i < 4; i++)
	//                         BASE     R        G          B
		printf("\n%d,%d,%d = %c\n",pal[0],pal[1],pal[2],' '+1 +	pal[0]*16	+pal[1]*4	+	pal[2]); //Edit the marked numbers for RGB values.
}