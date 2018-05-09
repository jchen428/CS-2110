#include <stdlib.h>
#include "mylib.h"
#include "spritesheet.h"
#include "farnsworth.h"

void draw_image_portion(int destination_row, int destination_col, int source_row, int source_col, int source_width, int source_height, const u16* image, int image_width);

void hard_tester(int destination_row, int destination_col);

int main(void)
{
    
    REG_DISPCNT = MODE3 | BG2_ENABLE; 



    /* Test your function here. To test it we have included 2 images.
       1) farnsworth
       2) A spritesheet of size 480x480. Each graphic in this image is 60x60 and there are 8 of these graphics in a row.*/

   
    waitForVblank();

    draw_image_portion(90, 70, 33, 29, 60, 62, farnsworth, FARNSWORTH_WIDTH); /* This should produce the same result screen 
                                                                                 as the one from the pdf. */
    



    /* Once you are done throughly testing your function, try calling the hard_tester() function. The function is already
       implemented for you at the end of this file. It draws each of the 60x60 graphics from spritesheet in order. One
       graphic per frame. This makes it appear as though it is animated. */

    hard_tester(30,90);  


    while (1); 
}


/* TODO: COMPLETE THIS FUNCTION
 *
 * This function draws a portion of an arbitrary sized image on the screen at a certain point using DMA.
 *
 * The destination_row and desination_col define the beginning row and column on the screen that the portion of the 
 * image should be drawn at.
 *
 * Together the source_row, source_col, source_width and source_height define the location and dimensions of the 
 * smaller portion of the image to be drawn.
 *
 * You cannot assume that the source image given is the size of the screen. The inputted source image can be of any 
 * width or height.
 *
 * We will test your function with valid parameters. We will NOT ask you to draw a portion of an image that is outside the
 * bounds of the source image.
 *
 * @ destination_row: the row on the screen that the portion of the image should be drawn at
 * @ destination_col: the column on the screen that the portion of the image should be drawn at
 * @ source_row: the row of the source image that begins the portion of the smaller image to be drawn
 * @ source_col: the column of the source image that begins the portion of the smaller image to be drawn
 * @ source_width: the width of the portion of the image to be drawn 
 * @ source_height: the height of the portion of the image to be drawn
 * @ image: the full source image
 * @ image_width: the width of the full source image
 */

void draw_image_portion(int destination_row, int destination_col, int source_row, int source_col, int source_width, int source_height, const u16* image, int image_width)
{
    for (int i = 0; i < source_height; i++)
    {
         DMA[3].src = image + OFFSET(source_row + i, source_col, image_width);
         DMA[3].dst = videoBuffer + OFFSET(destination_row + i, destination_col, 240);
         DMA[3].cnt = source_width | DMA_ON;
    }
}


/* DO NOT MODIFY
 *
 * Draws each of the 60x60 graphics from spritesheet in order. One graphic per frame. 
 * This makes it appear as though it is animated.
 *
 * @ destination_row: the row on the screen that the animation will begin at
 * @ destination_col: the column on the screen that the animation will begin at
 */
void hard_tester(int destination_row, int destination_col)
{
    int animation_length = 20; /* number of times to cycle through drawing all 60 images */


    for(int a = 0; a < animation_length; a++){ /* loops through the animation sequence animation_length number of times */

        for(int i = 0; i < 7; i++) { /* loop through the first 7 rows of the graphics */
            for(int j = 0; j < 8; j++) { /* loop through each column in the first 7 rows */
                waitForVblank();
                draw_image_portion(destination_row, destination_col, i*60, j*60, 60, 60, spritesheet, SPRITESHEET_HEIGHT);
            }
        }
 
        for(int j = 0; j < 4; j++) { /* loop through the last 4 graphics in the last row */
            waitForVblank();
            draw_image_portion(destination_row, destination_col, 7*60, j*60, 60, 60, spritesheet, SPRITESHEET_HEIGHT);
        }
    }
}
