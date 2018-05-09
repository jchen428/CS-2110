#include "myLib.h"
#include "font.h"

unsigned short *videoBuffer = (unsigned short *)0x6000000;

void setPixel(int row, int col, u16 color)
{
	videoBuffer[OFFSET(row, col, 240)] = color;
}

void drawChar(int row, int col, char ch, u16 color){
	int r,c;
	for(r=0; r<8; r++){
		for(c=0; c<6; c++){
			if(fontdata_6x8[OFFSET(r,c,6) + 48*ch]){
				setPixel(r + row, c + col, color);
			}	
		}
	}
}

void drawString(int row, int col, char *str, u16 color){
	int chars_drawn = 0;
	while(*str){
		drawChar(row, col + 6*chars_drawn++, *str++, color);
	}
}

void waitForVblank(){
	while(SCANLINE_COUNTER > 160);
	while(SCANLINE_COUNTER < 160);
}
