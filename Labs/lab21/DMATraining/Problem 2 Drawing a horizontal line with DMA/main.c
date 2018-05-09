#include <stdlib.h>
#include "mylib.h"

u16 junk[240];
void drawJunk(int y);
void fillJunk();

int main(void)
{
	int i;
	REG_DISPCNT = MODE3 | BG2_ENABLE;
	
	/* Initialization */	
	for (i = 0; i < 240; i++)
		junk[i] = i;
			
	while (1)
	{
		waitForVblank();
		//for (i = 0; i < 240; i++)
		//	drawJunk(i);
		fillJunk();
	}
}

/* Copy everything that is in the junk array on the videobuffer
at the y position that is after this finishes the row of the 
videoBuffer at the indicated y should contain whatever is in
junk and if this is correct you should see 240 vertical lines
kinda like a gradient going from black to red */
/* i.e. if y is zero then only the first row of the videoBuffer should be filled*/
/* i.e. if y is one then only the second row of the videoBuffer should be filled*/
void drawJunk(int y)
{
	DMA[3].src = &junk;
	DMA[3].dst = videoBuffer + (y * 240);
	DMA[3].cnt = 240 | DMA_ON;
}

/* Now I was nice enough to write the code that will call this for each row
But consider this!
instead of the u16 junk[240] array how about if I make it
u16 junk[120] and fill it with random values
now what this function will do is copy it into the entire screen
Now of course you will need a for loop!
but how many times will this loop need to be ran?
*/
void fillJunk()
{
	/* Declare some junk */
	u16 myJunk[7];
	int i;
	
	for (i = 0; i < sizeof(myJunk) / sizeof(u16); i++)
		myJunk[i] = rand() % 32767; /* Hey random color here */
		
	/* Your code here repeatedly DMA myJunk to fill the screen */
	for (int i = 0; i < 160; i++)
	{
		DMA[3].src = &myJunk;
		DMA[3].dst = videoBuffer + (i * 240);
		DMA[3].cnt = 240 | DMA_ON;
	}
}


/* Now for even more practice
What if the number of entries in junk was not a nice number
say 7 now rewrite fillJunk for this case */
