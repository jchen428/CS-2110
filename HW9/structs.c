/**
 * Jesse Chen
 */

#include "structs.h"
#include "myLib.h"
#include <stdlib.h>

#include "player.h"
#include "food.h"
#include "broccoli.h"

/**
 * Adds a random Food to the foods[] array at a random location.
 *
 * @param *e 			The game Engine
 * @param *p 			The Player
 * @param foods[] 		Food array containing all the game's Food
 * @param i 			Index of foods[] at which to add new Food
 * @param broccolis[] 	Broccoli array containing all the game's Broccoli
 */
void addFood(Engine *e, Player *p, Food foods[], int i, Broccoli broccolis[])
{
	Food_t foodTypes[5] = {BURGER, PIZZA, CHICKEN, CAKE, DONUT};

	foods[i].type = foodTypes[rand() % 5];
	foods[i].isEaten = 0;

	const u16 *image;

	switch (foods[i].type)
	{
		case BURGER:
			foods[i].height = BURGER_HEIGHT;
			foods[i].width = BURGER_WIDTH;
			image = burger;
		break;
		case PIZZA:
			foods[i].height = PIZZA_HEIGHT;
			foods[i].width = PIZZA_WIDTH;
			image = pizza;
		break;
		case CHICKEN:
			foods[i].height = CHICKEN_HEIGHT;
			foods[i].width = CHICKEN_WIDTH;
			image = chicken;
		break;
		case CAKE:
			foods[i].height = CAKE_HEIGHT;
			foods[i].width = CAKE_WIDTH;
			image = cake;
		break;
		case DONUT:
			foods[i].height = DONUT_HEIGHT;
			foods[i].width = DONUT_WIDTH;
			image = donut;
		break;
		default:
			foods[i].height = BURGER_HEIGHT;
			foods[i].width = BURGER_WIDTH;
			image = burger;
		break;
	}

	int flag;
	do
	{
		foods[i].row = rand() % (SCREEN_HEIGHT - foods[i].height);
		foods[i].col = rand() % (SCREEN_WIDTH - foods[i].width);

		flag = 0;
		for (int j = 0; j < e->level; j++)
		{
			if (checkCollideFB(&foods[i], &broccolis[j]))
			{
				flag = 1;
				setPixel(235, 155, RED);
			}
		}
	} while (checkCollidePF(p, &foods[i]) || checkCollideFF(foods, i) || flag);
		//|| checkCollideFB2(e, &foods[i], broccolis));

	drawImage3(foods[i].row, foods[i].col, foods[i].height, foods[i].width, image);

	e->curFood++;
	e->totalFood++;
}

/**
 * Add up to 4 Broccoli depending on current level. One per level up to level 4.
 *
 * @param *e 			The game Engine
 * @param *p 			The Player
 * @param broccolis[] 	Broccoli array containing all the game's Broccoli
 */
void addBroccoli(Engine *e, Player *p, Broccoli broccolis[])
{
	if (e->level >= 4)
	{
		broccolis[3].curRow = SCREEN_HEIGHT - BROCCOLI_HEIGHT;
		broccolis[3].curCol = 0;
		broccolis[3].deltaRow = -1;
		broccolis[3].deltaCol = 1;
		drawImage3(broccolis[3].curRow, broccolis[3].curCol, 
			BROCCOLI_HEIGHT, BROCCOLI_WIDTH, broccoli);
	}
	if (e->level >= 3)
	{
		broccolis[2].curRow = SCREEN_HEIGHT - BROCCOLI_HEIGHT;
		broccolis[2].curCol = SCREEN_WIDTH - BROCCOLI_WIDTH;
		broccolis[2].deltaRow = -1;
		broccolis[2].deltaCol = -1;
		drawImage3(broccolis[2].curRow, broccolis[2].curCol, 
			BROCCOLI_HEIGHT, BROCCOLI_WIDTH, broccoli);
	}
	if (e->level >= 2)
	{
		broccolis[1].curRow = 0;
		broccolis[1].curCol = SCREEN_WIDTH - BROCCOLI_WIDTH;
		broccolis[1].deltaRow = 1;
		broccolis[1].deltaCol = -1;
		drawImage3(broccolis[1].curRow, broccolis[1].curCol, 
			BROCCOLI_HEIGHT, BROCCOLI_WIDTH, broccoli);
	}
	if (e->level >= 1)
	{
		broccolis[0].curRow = 0;
		broccolis[0].curCol = 0;
		broccolis[0].deltaRow = 1;
		broccolis[0].deltaCol = 1;
		drawImage3(broccolis[0].curRow, broccolis[0].curCol, 
			BROCCOLI_HEIGHT, BROCCOLI_WIDTH, broccoli);
	}
}

/**
 * Checks boundaries and moves player.
 *
 * @param dir Direction the player is facing
 *				0: up
 *				1: down
 *				2: left
 *				3: right
 */
void movePlayer(Player *p, int dir)
{
	switch (dir) //Change row/col position
	{
		case 0:
			p->curRow -= 2;
		break;
		case 1:
			p->curRow += 2;
		break;
		case 2:
			p->curCol -= 2;
			p->facing = 0;
		break;
		case 3:
			p->curCol += 2;
			p->facing = 1;
		break;
	}

	boundsCheck(&p->curRow, SCREEN_HEIGHT - 1, 0, RIGHT_HEIGHT);
	boundsCheck(&p->curCol, SCREEN_WIDTH - 1, 0, RIGHT_WIDTH);

	drawRect(p->oldRow, p->oldCol, RIGHT_HEIGHT, RIGHT_WIDTH, BGCOLOR);

	if (p->facing == 0)
		drawImage3(p->curRow, p->curCol, LEFT_HEIGHT, LEFT_WIDTH, left);
	else if (p->facing == 1)
		drawImage3(p->curRow, p->curCol, RIGHT_HEIGHT, RIGHT_WIDTH, right);
}

/**
 * Moves a Broccoli.
 *
 * @param *b The Broccoli to be moved
 */
void moveBroccoli(Broccoli *b)
{
	b->curRow += b->deltaRow;
	b->curCol += b->deltaCol;

	boundsCheck(&b->curRow, SCREEN_HEIGHT - 1, &b->deltaRow, BROCCOLI_HEIGHT);
	boundsCheck(&b->curCol, SCREEN_WIDTH - 1, &b->deltaCol, BROCCOLI_WIDTH);

	drawRect(b->oldRow, b->oldCol, BROCCOLI_HEIGHT, BROCCOLI_WIDTH, BGCOLOR);
	drawImage3(b->curRow, b->curCol, BROCCOLI_HEIGHT, BROCCOLI_WIDTH, broccoli);
}

/**
 * Checks for collisions between two arbitrary Collideable game objects.
 *
 * @param c1 Object 1
 * @param c2 Object 2
 * @return 1 (true) if collision detected, or 0 (false) if no collision detected
 */
int checkCollision(Collideable c1, Collideable c2)
{
	if (c1.row < (c2.row + c2.height + 1) && (c1.col + c1.width + 1) > c2.col &&
		(c1.row + c1.height + 1) > c2.row && c1.col < (c2.col + c2.width + 1))
		return 1;
	else
		return 0;
}

/**
 * Checks collisions between the Player and a Food.
 *
 * @param *p The Player
 * @param *f The Food
 * @return 1 (true) if collision detected, or 0 (false) if no collision detected
 */
int checkCollidePF(Player *p, Food *f)
{
	Collideable player, food; //Make Food and Broccoli comparable

	player.row = p->curRow;
	player.col = p->curCol;
	player.height = RIGHT_HEIGHT;
	player.width = RIGHT_WIDTH;

	food.row = f->row;
	food.col = f->col;
	food.height = f->height;
	food.width = f->width;
	
	if(checkCollision(player, food)) //Check if current Food and Broccoli collide
	{
		return 1;
	}
	else
		return 0;
}

/**
 * Checks collisions between the Player and a Broccoli.
 *
 * @param *p The Player
 * @param *b The Broccoli
 * @return 1 (true) if collision detected, or 0 (false) if no collision detected
 */
int checkCollidePB(Player *p, Broccoli *b)
{
	Collideable player, broccoli; //Make Food and Broccoli comparable

	player.row = p->curRow;
	player.col = p->curCol;
	player.height = RIGHT_HEIGHT;
	player.width = RIGHT_WIDTH;

	broccoli.row = b->curRow;
	broccoli.col = b->curCol;
	broccoli.height = BROCCOLI_HEIGHT;
	broccoli.width = BROCCOLI_WIDTH;

	if(checkCollision(player, broccoli)) //Check if Player and Broccoli collide
		return 1;
	else
		return 0;
}

/**
 * Checks collisions between a Food and a Broccoli. 
 *
 * @param *f 	The Food
 * @param *b	The Broccoli
 * @return 		1 (true) if any collisions detected, or 0 (false) if no
 *				collisions detected
 */
int checkCollideFB(Food *f, Broccoli *b)
{
	if (f->isEaten == 0) //If Food is not eaten
	{
		Collideable food, broccoli; //Make Food and Broccoli comparable

		food.row = f->row;
		food.col = f->col;
		food.height = f->height;
		food.width = f->width;

		broccoli.row = b->curRow;
		broccoli.col = b->curCol;
		broccoli.height = BROCCOLI_HEIGHT;
		broccoli.width = BROCCOLI_WIDTH;

		if(checkCollision(food, broccoli)) //Check if Food and Broccoli collide
		{
			b->deltaRow *= -1;
			b->deltaCol *= -1;
			//boundsCheck2(&b->curRow, f->row, f->row + f->height - 1, &b->deltaRow, BROCCOLI_HEIGHT);
			//boundsCheck2(&b->curCol, f->col, f->col + f->width - 1, &b->deltaCol, BROCCOLI_WIDTH);

			return 1;
		}
	}

	return 0;
}

/**
 * Checks collisions between a Food and all Broccolis. 
 *
 * @param *f 			The Food, guaranteed new/uneaten
 * @param broccolis[] 	Broccoli array containing all the game's Broccoli
 * @return 				1 (true) if any collisions detected, or 0 (false) if no
 *						collisions detected
 */
int checkCollideFB2(Engine *e, Food *f, Broccoli broccolis[])
{
	Collideable food, broccoli; //Make Food and Broccoli comparable

	food.row = f->row;
	food.col = f->col;
	food.height = f->height;
	food.width = f->width;

	for (int i = 0; i < e->level; i++)
	{
		broccoli.row = broccolis[i].curRow;
		broccoli.col = broccolis[i].curCol;
		broccoli.height = BROCCOLI_HEIGHT;
		broccoli.width = BROCCOLI_WIDTH;

		if(checkCollision(food, broccoli)) //Check if Food and Broccoli collide
		{
			setPixel(155, 150, RED);
			broccolis[i].deltaRow *= -1;
			broccolis[i].deltaCol *= -1;

			//boundsCheck2(&broccolis[i].curRow, f->row, f->row + f->height - 1, &broccolis[i].deltaRow, BROCCOLI_HEIGHT);
			//boundsCheck2(&broccolis[i].curCol, f->col, f->col + f->width - 1, &broccolis[i].deltaCol, BROCCOLI_WIDTH);

			return 1;
		}
	}

	return 0;
}


/**
 * Checks collisions between all indistinct pairs of Broccoli.
 *
 * @param *e 			The game Engine
 * @param broccolis[] 	Broccoli array containing all the game's Broccoli
 * @return 				1 (true) if any collisions detected, or 0 (false) if no
 *						collisions detected
 */
int checkCollideBB(Engine *e, Broccoli broccolis[])
{
	Collideable b1, b2; //Make Broccolis comparable
	int flag = 0; //Return flag for result
	
	if (e->level > 1)
	{
		for (int i = 0; i < e->level; i++)
		{
			b1.row = broccolis[i].curRow;
			b1.col = broccolis[i].curCol;
			b1.height = BROCCOLI_HEIGHT;
			b1.width = BROCCOLI_WIDTH;

			for (int j = 0; j < e->level; j++)
			{
				if (i != j)
				{
					b2.row = broccolis[j].curRow;
					b2.col = broccolis[j].curCol;
					b2.height = BROCCOLI_HEIGHT;
					b2.width = BROCCOLI_WIDTH;

					if (checkCollision(b1, b2))
					{
						broccolis[i].deltaRow *= -1;
						broccolis[i].deltaCol *= -1;

						flag = 1; //Collision if any one pair collide
					}
				}
			}
		}
	}

	return flag;
}

/**
 * Checks collisions between all pairs of Food.
 *
 * @param foods[] 	Food array containing all the game's Food
 * @param foodIndex Index of foods[] at which Food-Food collisions are being checked
 * @return 			1 (true) if any collisions detected, or 0 (false) if no
 *						collisions detected
 */
int checkCollideFF(Food foods[], int foodIndex)
{
	Collideable f1, f2;

	f1.row = foods[foodIndex].row;
	f1.col = foods[foodIndex].col;
	f1.height = foods[foodIndex].height;
	f1.width = foods[foodIndex].width;

	for (int i = 0; i < foodIndex; i++)
	{
		if (!foods[i].isEaten)
		{
			f2.row = foods[i].row;
			f2.col = foods[i].col;
			f2.height = foods[i].height;
			f2.width = foods[i].width;

			if (checkCollision(f1, f2))
			{
				return 1;
			}
		}
	}

	return 0;
}

/**
 * Eats a Food.
 *
 * @param *e 		The game Engine
 * @param *f 		The Food
 * @param height 	The image height of *f
 * @param width 	The image width of *f
 */
void eatFood(Engine *e, Food *f)
{
	Food_t type = f->type;
	int height, width;

	switch (type) //Determine Food type
	{
		case BURGER:
			height = BURGER_HEIGHT;
			width = BURGER_WIDTH;
		break;
		case PIZZA:
			height = PIZZA_HEIGHT;
			width = PIZZA_WIDTH;
		break;
		case CHICKEN:
			height = CHICKEN_HEIGHT;
			width = CHICKEN_WIDTH;
		break;
		case CAKE:
			height = CAKE_HEIGHT;
			width = CAKE_WIDTH;
		break;
		case DONUT:
			height = DONUT_HEIGHT;
			width = DONUT_WIDTH;
		break;
		default:
			height = 25;
			width = 25;
		break;
	}

	e->curFood--;
	e->score++;
	f->isEaten = 1;
	drawRect(f->row, f->col, height, width, BGCOLOR);
}
