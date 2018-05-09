/**
 * Jesse Chen
 */

#include "myLib.h"
#include "text.h"
#include "structs.h"
#include <stdio.h>
#include <stdlib.h>

//Include images
#include "title.h"
#include "clear.h"
#include "win.h"
#include "lose.h"
#include "player.h"
#include "food.h"
#include "broccoli.h"

#define MAX_FOOD 50
#define MAX_BROCCOLI 4

Food foods[MAX_FOOD];
Broccoli broccolis[MAX_BROCCOLI];
int curFoodIndex;
int broccoliCount;

/**
 * Initializes/reinitializes the game to title screen
 */
void init(Engine *e)
{
	drawImage3(0, 0, TITLE_HEIGHT, TITLE_WIDTH, title);
	e->state = 0;
	e->level = 0;
	e->score = 0;
}

void startLevel(Engine *e, Player *p)
{
	fillScreen(BGCOLOR);

	e->state = 1;
	e->level++;
	e->timer = 0;
	e->totalFood = 0;
	e->curFood = 0;

	p->curRow = (SCREEN_HEIGHT / 2) - (RIGHT_HEIGHT / 2);
	p->curCol = (SCREEN_WIDTH / 2) - (RIGHT_WIDTH / 2);
	p->facing = 1;

	for (curFoodIndex = 0; curFoodIndex < 5; curFoodIndex++)
		addFood(e, p, foods, curFoodIndex, broccolis);

	addBroccoli(e, p, broccolis);

	drawImage3(p->curRow, p->curCol, RIGHT_HEIGHT, RIGHT_WIDTH, right);

	while (e->timer < 300) //Wait 300 VBlanks before starting
	{
		WaitForVblank();
		e->timer++;
	}
	e->timer = 0;
}

int main()
{
	REG_DISPCTL = MODE3 | BG2_ENABLE;

	Engine engine;
	Player player;
	char scoreBuffer[11];
	init(&engine);

	while (1) //Game loop
	{
		WaitForVblank();

		if (KEY_DOWN_NOW(BUTTON_SELECT))
		{
			init(&engine);
		}

		switch (engine.state) //Determine game state
		{
			case 0: //Title screen
				engine.timer++;

				if (KEY_DOWN_NOW(BUTTON_START))
				{
					srand(engine.timer); //Seed RNG
					startLevel(&engine, &player);
				}
			break;
			case 1: //Main gameplay
				engine.timer++;
				int isEmpty = 1;

				//Save old position
				player.oldRow = player.curRow;
				player.oldCol = player.curCol;

				//Player movement
				if (KEY_DOWN_NOW(BUTTON_UP))
					movePlayer(&player, 0);
				if (KEY_DOWN_NOW(BUTTON_DOWN))
					movePlayer(&player, 1);
				if (KEY_DOWN_NOW(BUTTON_LEFT))
					movePlayer(&player, 2);
				if (KEY_DOWN_NOW(BUTTON_RIGHT))
					movePlayer(&player, 3);
				else
					movePlayer(&player, -1);

				//Check for Food collisions and if Food is still on the screen
				for (int i = 0; i < 5 * engine.level; i++)
				{
					if (foods[i].isEaten == 0)
					{
						if (checkCollidePF(&player, &foods[i]))
							eatFood(&engine, &foods[i]);

						isEmpty = 0;
					}
				}

				//Broccoli movement and collisions
				for (int i = 0; i < engine.level; i++)
				{
					broccolis[i].oldRow = broccolis[i].curRow;
					broccolis[i].oldCol = broccolis[i].curCol;
					moveBroccoli(&broccolis[i]);

					if (checkCollidePB(&player, &broccolis[i]))
					{
						engine.state = 4;
						engine.timer = 0;
					}

					for (int j = 0; j < engine.totalFood; j++)
						checkCollideFB(&foods[j], &broccolis[i]);
				}

				//Check for Broccoli-Broccoli collisions
				if (engine.level > 1)
					checkCollideBB(&engine, broccolis);
					

				//Periodically add more food if there are less than 5 on
				//screen and less than 5 * level have appeared in total
				if (engine.timer > 50 && engine.curFood < 5 && engine.totalFood < 5 * engine.level)
				{
					addFood(&engine, &player, foods, curFoodIndex, broccolis);
					curFoodIndex++;
					engine.timer = 0;
				}

				//Print score on screen
				sprintf(scoreBuffer, "Score: %d", engine.score);
				drawRect(151, 1 + 6 * 7, 8, 6 * 4, BGCOLOR);
				drawString(151, 1, scoreBuffer, BLACK);

				//If no food is left, go to state 2
				if (isEmpty == 1)
				{
					engine.state = 2;
					engine.timer = 0;

					if (engine.level == 4)
						engine.state = 3;
				}

				//Debugging controls
				if (KEY_DOWN_NOW(BUTTON_A))
				{
					if (engine.level < 4)
					{
						engine.state = 2;
						engine.timer = 0;
					}		
					else
					{
						engine.state = 3;
						engine.timer = 0;
					}
				}
				else if (KEY_DOWN_NOW(BUTTON_B))
					engine.state = 4;
			break;
			case 2: //Stage clear screen
				if (engine.timer == 0)
				{
					drawImage3(0, 0, CLEAR_HEIGHT, CLEAR_WIDTH, clear);
					engine.timer++;
				}

				if (KEY_DOWN_NOW(BUTTON_START))
				{
					startLevel(&engine, &player);
				}
			break;
			case 3: //Win screen
				if (engine.timer == 0)
				{
					drawImage3(0, 0, WIN_HEIGHT, WIN_WIDTH, win);
					engine.timer++;
				}
			break;
			case 4: //Lose screen
				drawImage3(0, 0, LOSE_HEIGHT, LOSE_WIDTH, lose);
			break;
		}
	} // while gameloop
}
