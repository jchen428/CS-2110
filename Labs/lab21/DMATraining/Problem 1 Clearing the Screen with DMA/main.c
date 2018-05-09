#include <stdlib.h>
#include "mylib.h"
#include "mystery.h"

extern const u16 mystery[38400];

int main(void)
{
	//int i, j;
	REG_DISPCNT = MODE3 | BG2_ENABLE;

	/* 1. Hey fix this Draw the image with DMA instead ;D */
	for (int i = 0; i < 160; i++)
	{
		 DMA[3].src = mystery + (i * 240);
		 DMA[3].dst = videoBuffer + (i * 240);
		 DMA[3].cnt = 240 | DMA_ON;
	}
		
	while (1)
	{
		waitForVblank();
		/* 2. Clear the screen here using DMA */
		/* Note in a real game you would not do this for performance issues, but just as an exercise do it here */
		for (int i = 0; i < 160; i++)
		{
			 DMA[3].src = 0;
			 DMA[3].dst = videoBuffer + (i * 240);
			 DMA[3].cnt = 240 | DMA_ON;
		}
	}
}
