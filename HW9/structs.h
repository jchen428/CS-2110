/**
 * Jesse Chen
 */

#ifndef STRUCTS_H
#define STRUCTS_H

typedef enum { BURGER, PIZZA, CHICKEN, CAKE, DONUT } Food_t;

typedef struct
{
	int state;
	int level;
	int score;
	int timer;
	int totalFood;
	int curFood;
} Engine;

typedef struct
{
 	int curRow;
 	int curCol;
 	int oldRow;
 	int oldCol;
 	int facing;
} Player;

typedef struct
{
	Food_t type;
	int isEaten;
	int row;
 	int col;
 	int height;
 	int width;
} Food;

typedef struct
{
	int curRow;
 	int curCol;
 	int oldRow;
 	int oldCol;
 	int deltaRow;
 	int deltaCol;
} Broccoli;

typedef struct
{
	int row;
	int col;
	int height;
	int width;
} Collideable;

void addFood(Engine *e, Player *p, Food foods[], int i, Broccoli broccolis[]);
void addBroccoli(Engine *e, Player *p, Broccoli broccolis[]);
void movePlayer(Player *p, int dir);
void moveBroccoli(Broccoli *b);
int checkCollision(Collideable c1, Collideable c2);
int checkCollidePF(Player *p, Food *f);
int checkCollidePB(Player *p, Broccoli *b);
int checkCollideFB(Food *f, Broccoli *b);
int checkCollideFB2(Engine *e, Food *f, Broccoli broccolis[]);
int checkCollideBB(Engine *e, Broccoli broccolis[]);
int checkCollideFF(Food foods[], int foodIndex);
void eatFood(Engine *e, Food *f);

#endif
