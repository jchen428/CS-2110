#include "myLib.h"
#include "text.h"

void drawChar(int row, int col, char ch, u16 color)
{
	/*for (int i = 0; i < 8; i++)
	{
		DMA[3].src = &fontdata_6x8[OFFSET(i, 0, 6) + ch * 48];
		DMA[3].dst = videoBuffer + OFFSET(row + i, col, 6);
		DMA[3].cnt = 6 | DMA_ON;
	}*/

	int r,c;
	for(r=0; r<8; r++)
	{
		for(c=0; c<6; c++)
		{
			if(fontdata_6x8[OFFSET(r, c, 6) + ch * 48])
			{
				setPixel(r+row, c+col, color);
			}
		}
	}
}


void drawString(int row, int col, char *s, u16 color)
{
	while (*s)
	{
		drawChar(row, col, *s++, color);
		col += 6;

	}
}
