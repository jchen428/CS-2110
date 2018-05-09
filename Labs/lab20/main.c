//Jesse Chen

#include "myLib.h"

int main()
{
	REG_DISPCTL = MODE3 | BG2_ENABLE;

	while(1) // game loop
	{
		waitForVblank();
	
		//TODO: SET PIXEL (10,10) TO THE RIGHT COLOR
		if (KEY_DOWN_NOW(BUTTON_UP))
		{
			setPixel(10, 10, WHITE);
		}
		else if (KEY_DOWN_NOW(BUTTON_DOWN))
		{
			setPixel(10, 10, RED);
		}
		else if (KEY_DOWN_NOW(BUTTON_LEFT))
		{
			setPixel(10, 10, BLUE);
		}
		else if (KEY_DOWN_NOW(BUTTON_RIGHT))
		{
			setPixel(10, 10, GREEN);
		}
		
		//TODO: DRAW THE STRING IF A OR B is pressed
		if (KEY_DOWN_NOW(BUTTON_A) || KEY_DOWN_NOW(BUTTON_B))
		{
			drawString(10, 10, "this is an example!", RED);
		}
	}
}
