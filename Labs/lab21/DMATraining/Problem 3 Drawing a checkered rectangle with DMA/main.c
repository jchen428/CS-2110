#include <stdlib.h>
#include "mylib.h"

void drawCheckeredRect(int x, int y, int width, int height, vu16 color1, int unused, vu16 color2);

int main(void)
{
    REG_DISPCNT = MODE3 | BG2_ENABLE;
			
    while (1)
    {
    	waitForVblank();
		  drawCheckeredRect(10, 20, 100, 3, RGB(0, 0, 31), 32767, RGB(31, 0, 0));
    }
}

/* Draws a checkered rectangle
   Each adjacent pixel will be in alternating colors (eg. pixel 0 is blue, pixel 1 is red, pixel 2 is blue, pixel 3 is red and so on

   x is guaranteed to be even, width is guaranteed to be even

   You may not allocate any memory (via malloc and friends) or declare an array in this function

   Hints 
     Use DMA_32 to copy 32 bits at a time.
     However you will need a 32 bit source to use, note that, color1 and color2 are only 16 bits.
     Write it with a for loop first

   Ignore the parameter named unused it is there for a specific reason, to make sure you code this function correctly.
*/
void drawCheckeredRect(int x, int y, int width, int height, vu16 color1, int unused, vu16 color2)
{
}

