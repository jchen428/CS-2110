/**
 * Jesse Chen
 */

#include "myLib.h"

u16 *videoBuffer = (unsigned short *)0x6000000;

void drawImage3(int row, int col, int height, int width, const u16* image)
{
	for (int i = 0; i < height; i++)
	{
		 DMA[3].src = image + OFFSET(i, 0, width);
		 DMA[3].dst = videoBuffer + OFFSET(row + i, col, 240);
		 DMA[3].cnt = width | DMA_ON;
	}
}

void setPixel(int row, int col, u16 color)
{
	videoBuffer[OFFSET(row, col, 240)] = color;
}

void drawRect(int row, int col, int height, int width, u16 color)
{
	DMA[3].src = &color;
	for (int i = 0; i < height; i++)
	{
		DMA[3].dst = videoBuffer + OFFSET(row + i, col, 240);
		DMA[3].cnt = width | DMA_ON | DMA_SOURCE_FIXED;
	}
}

void fillScreen(volatile u16 color)
{
	DMA[3].src = &color;
	DMA[3].dst = videoBuffer;
	DMA[3].cnt = SCREEN_SIZE | DMA_ON | DMA_SOURCE_FIXED;
}

int boundsCheck(int *var, int bound, int *delta, int size)
{
	if (*var < 0)
	{
		*var = 0;
		*delta = -*delta;
		return 1;
	}
	if (*var > bound - size+1)
	{
		*var = bound - size+1;
		*delta = -*delta;
	}
	return 0;
}

int boundsCheck2(int *var, int minBound, int maxBound, int *delta, int size)
{
		if (*var < minBound)
		{
			if (minBound == 0)
				*var = 0;
			else
				*var = minBound - size;
			
			*delta = -*delta;
			return 1;
		}
		if (*var > maxBound - size + 1)
		{
			if (maxBound == SCREEN_WIDTH)
				*var = maxBound - size + 1;
			else
				*var = maxBound;

			*delta = -*delta;
		}
		return 0;

}

void WaitForVblank()
{
	while (SCANLINECOUNTER > 160);
	while (SCANLINECOUNTER < 160);
}
