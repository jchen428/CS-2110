typedef unsigned short u16;
typedef unsigned int u32;
typedef unsigned char u8;

// Jesse Chen

#define REG_DISPCTL *(u16 *)0x4000000
#define MODE3 3
#define BG2_ENABLE (1<<10)

#define COLOR(r,g,b) ((r) | (g) << 5 | (b) << 10)
#define RED COLOR(31,0,0)
#define WHITE COLOR(31,31,31)
#define GREEN COLOR(0, 0, 31)
#define BLACK 0

#define OFFSET(r,c) ((r)*240+(c))


unsigned short *videoBuffer = (u16 *)0x6000000;

void setPixel(int row, int col, u16 color);
void drawRectangle(int row, int col, int width, int height, u16 color);
void drawHollowRectangle(int row, int col, int width, int height, u16 color);

int main()
{
	REG_DISPCTL = MODE3 | BG2_ENABLE;
	
	//use the functions you wrote to draw a pretty picture here!
	
	setPixel(10, 10, RED);
	drawRectangle(20, 20, 20, 20, WHITE);
	drawHollowRectangle(40, 40, 20, 20, GREEN); 

	while(1);
}

void setPixel(int row, int col, u16 color)
{
	videoBuffer[240 * row + col] = color;
}

void drawRectangle(int row, int col, int width, int height, u16 color)
{
	for (int i = row; i < row + width; i++) {
		for (int j = col; j < col + height; j++) {
			videoBuffer[240 * i + j] = color;
		}
	}
}

void drawHollowRectangle(int row, int col, int width, int height, u16 color)
{
	for (int i = col; i < col + width; i++) {
		videoBuffer[240 * row + i] = color;
		videoBuffer[240 * (row + height) + i] = color;
	}

	for (int i = row; i < row + height; i++) {
		videoBuffer[240 * i + col] = color;
		videoBuffer[240 * i + col + width] = color;
	}
}
