/*
 *  Created on: Feb 6, 2014
 *      Author: User
 */

#ifndef FAST_BASKET_H_
#define FAST_BASKET_H_


/* Includes */
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include <io.h>

//Library for writing to LCD
#include "altera_up_avalon_character_lcd.h"
//Library for VGA
#include "altera_up_avalon_video_pixel_buffer_dma.h"
//Library for Character buffer
#include "altera_up_avalon_video_character_buffer_with_dma.h"
//All the picture arrays are defined in this header file
#include "bmp_arrays.h"

#include <sys/alt_alarm.h>
//Library for timer

/* Macros */
//#define SWITCHES_BASE (volatile char *) 0x0004010
//#define LEDS_BASE (char *) 0x0004000
#define KEYS_BASE (volatile char * ) 0x0005020
#define WIDTH 320
#define HEIGHT_BOAT 222
#define HEIGHT_BASKET 207
#define HEIGHT_BOTTOM 240
#define MAX_FRUITS 10 // for now

#define BASKET_MOVEMENT 6

#define CHAR_BUF_WIDTH 80 //(each row is 4 pixels high)
#define CHAR_BUF_HEIGHT 60 // (each column is 4 pixels wide)

//Keyboard Related
#include <stdio.h>
#include "altera_up_avalon_ps2.h"
#include "altera_up_ps2_keyboard.h"

#define KEYBOARD_UP 0x75
#define KEYBOARD_DOWN 0x72
#define KEYBOARD_LEFT 0x6B
#define KEYBOARD_RIGHT 0x74
#define KEYBOARD_ENTER 0x5A
#define KEYBOARD_ESCAPE 0x76
#define KEYBOARD_ONE 0x16

alt_up_ps2_dev *ps2;
KB_CODE_TYPE decode;
char ascii;
int keyboard_init = 1;
alt_u8 getKeyboard();
void initKeyboard();
/* Enumeration for game state */

enum game_state {
	welcome, play
};

/* Structs */
typedef struct {
	int height; 	// tracks current height of fruit
	int speed; 		// random speed (only 0 to 9 for now)
	int sc; 		// speed count for controlling speed using Johnny's method
	int x_coord; 	// random x_coordinate where it will fall, ranges from 0 to 319
	int colour; 	// random colour generated with 3 random RGB parameters (range 0 to 255)
	int one; 		/* this parameter is only used in draw fruit to increment the fruit height. It is never changed.
			 	 	 	  when using a constant such as fruit_height[i] ++ or fruit_height[i]+=1 there
			 	 	 	  appears to be garbage values that cause the program to be buggy. */
} fruit ;


typedef struct {
	int type;		// type of fruit
					// 1 = apple, 2 = banana, 3 = cherry, 4 = lemon, 5 = orange, 6 = peach, 7 = pear, 8 = strawberry
	int height; 	// tracks current height of fruit
	int speed; 		// random speed (only 0 to 9 for now)
	int sc; 		// speed count for controlling speed using Johnny's method
	int x_coord; 	// random x_coordinate where it will fall, ranges from 0 to 319
	int one;
} bmp_fruit ;





/* Global Variables */
int num_fruits = 10; // will be decided on level
int max_speed = 10;  // will be decided on level

int basket_left = WIDTH/2 - 16;
int basket_right = WIDTH/2 + 15;
bmp_fruit bmp_fruits[MAX_FRUITS];
int availableXcoords[20] = {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19};
int catched;


/* Function Prototypes */
//For basket
void MoveLeft();
void MoveRight();
void DrawHollowBasket(alt_up_pixel_buffer_dma_dev*);
void DrawBmpBasket(int, int , alt_up_pixel_buffer_dma_dev*);
void DrawBlackRight(alt_up_pixel_buffer_dma_dev* );
void DrawBlackLeft(alt_up_pixel_buffer_dma_dev* );
void DrawBasket(alt_up_pixel_buffer_dma_dev* );
void MoveBasketLeft(alt_up_pixel_buffer_dma_dev* pixel_buffer);
void MoveBasketRight(alt_up_pixel_buffer_dma_dev* pixel_buffer);

//For background
void DrawBackground(alt_up_pixel_buffer_dma_dev* pixel_buffer);
void DrawGameOver(alt_up_pixel_buffer_dma_dev* pixel_buffer);
void RedrawBackground(int x0, int y0, int x1, int y1, alt_up_pixel_buffer_dma_dev* pixel_buffer);

//For Bmp generated fruits
void InitBmpFruit(int);
void MoveBmpFruit( int i, alt_up_pixel_buffer_dma_dev* pixel_buffer);
void swapXCoordWithAvailable(int fruit_index);
void shuffleAvailableXcoords(void);
void DrawBmpFruit(int[],int, int,alt_up_pixel_buffer_dma_dev*);
void GenBmpFruit(int);
void MoveVariousFruits(int i, alt_up_pixel_buffer_dma_dev* pixel_buffer);
#endif /* FAST_BASKET_H_ */
