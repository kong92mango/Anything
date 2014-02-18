
/***************************************** Project: Falling fruits *****************************************/

// All includes, macros, function prototypes, struct typedefs, global variables are declared
// in this header file
#include "fast_basket.h"
#define GAME_LENGTH 30
int main(void) {

	catched =0;
	int key_count=0; //for welcome screen
	unsigned int counter =0;
	int timeleft = GAME_LENGTH;
	int ticks_per_s;
	int ticks_start;
	int ticks_now;
	int elapsed_time;
	//unsigned int sm_counter = 0;

	srand ((int)time(NULL)); // Seed the random function

	/************************************** Initialize Character Buffer *****************************************/

	alt_up_char_buffer_dev *char_buffer;
	char_buffer = alt_up_char_buffer_open_dev("/dev/video_character_buffer_with_dma_0");
	if ( char_buffer == NULL)
		alt_printf("Error: could not open char buffer\n");
	else
		alt_printf ("Opened char buffer\n");
	alt_up_char_buffer_init(char_buffer);
	alt_up_char_buffer_clear(char_buffer);

	/**************************************** Initialize LCD Display *********************************************/

	alt_up_character_lcd_dev * char_lcd_dev;
	char_lcd_dev = alt_up_character_lcd_open_dev ("/dev/character_lcd_0");
	if ( char_lcd_dev == NULL)
		alt_printf ("Error: could not open character LCD device\n");
	else
		alt_printf ("Opened character LCD device\n");
	alt_up_character_lcd_init (char_lcd_dev);

	/************************************** Initialize Pixel Buffer for VGA ***************************************/

	alt_up_pixel_buffer_dma_dev* pixel_buffer;
	// Use the name of your pixel buffer DMA core
	pixel_buffer = alt_up_pixel_buffer_dma_open_dev("/dev/video_pixel_buffer_dma_0");

	if ( pixel_buffer == NULL)
		alt_printf ("Error: could not open pixel buffer\n");
	else
		alt_printf ("Opened pixel buffer\n");
	// Set the background buffer address ?Although we don't use the background, they only provide a function
	// to change the background buffer address, so we must set that, and then swap it to the foreground.
	alt_up_pixel_buffer_dma_change_back_buffer_address(pixel_buffer,	PIXEL_BUFFER_BASE);
	// Swap background and foreground buffers
	alt_up_pixel_buffer_dma_swap_buffers(pixel_buffer);
	// Wait for the swap to complete
	while(alt_up_pixel_buffer_dma_check_swap_buffers_status(pixel_buffer));
	// Clear the screen
	alt_up_pixel_buffer_dma_clear_screen(pixel_buffer, 0);

	/************************************** Initialize Game State to Welcome Screen****************************/

	initKeyboard();
	enum game_state gState = welcome; // Initialize game state to "Welcome"
	unsigned char keys;
	ticks_per_s = alt_ticks_per_second();
	printf("initial timer tick record\n");
	ticks_start = alt_nticks();
	//printf("frequency = %d\n", ticks_per_s);
	//printf("started time: %d\n",ticks_start);

	while(1) {
		//break;
		DrawBackground( pixel_buffer );
		if(gState == welcome ) {
			catched = 0;
			key_count = 0;
			while(1) {
//				if(sm_counter > 400000000){
//					sm_counter = 0;
//					counter++;
//				}

				break;
				alt_u8 data = getKeyboard();
				keys = IORD(KEYS_BASE, 0); // Read push keys from DE2 - IORD prevents processor reading from cache

				//Write "Welcome to fruit catching" on centre of screen
				alt_up_char_buffer_string(char_buffer, "Welcome to", CHAR_BUF_WIDTH/2 - 5, CHAR_BUF_HEIGHT/2 - 13);
				alt_up_char_buffer_string(char_buffer, "Fruit Catching", CHAR_BUF_WIDTH/2 - 7, CHAR_BUF_HEIGHT/2 - 11);

				alt_up_char_buffer_string(char_buffer, "Right key to chose level", CHAR_BUF_WIDTH/2 -3, CHAR_BUF_HEIGHT/2 +17);
				alt_up_char_buffer_string(char_buffer, "Left key to select", CHAR_BUF_WIDTH/2 -3, CHAR_BUF_HEIGHT/2 +19);
				alt_up_char_buffer_string(char_buffer, "Key3 to reset", CHAR_BUF_WIDTH/2 - 3, CHAR_BUF_HEIGHT/2 +21);

				alt_up_char_buffer_string(char_buffer, "Level Select:", CHAR_BUF_WIDTH/2 - 26, CHAR_BUF_HEIGHT/2 + 14);
				alt_up_char_buffer_string(char_buffer, "Easy", CHAR_BUF_WIDTH/2 - 24, CHAR_BUF_HEIGHT/2 + 17);
				alt_up_char_buffer_string(char_buffer, "Medium", CHAR_BUF_WIDTH/2 - 24, CHAR_BUF_HEIGHT/2 + 19);
				alt_up_char_buffer_string(char_buffer, "Hard", CHAR_BUF_WIDTH/2 - 24, CHAR_BUF_HEIGHT/2 + 21);
				alt_up_char_buffer_string(char_buffer, "Blizzard", CHAR_BUF_WIDTH/2 - 24, CHAR_BUF_HEIGHT/2 + 23);

				switch(key_count%4)	{
					case 0: //Easy
						num_fruits = 1;
						max_speed = 1;
					//	alt_up_pixel_buffer_dma_draw_rectangle(pixel_buffer, 58, 211, 62, 215, 0, 0);
						RedrawBackground(58, 211, 62, 215, pixel_buffer);
						alt_up_pixel_buffer_dma_draw_rectangle(pixel_buffer, 58, 187, 62, 191, 0x05FF, 0);
						break;
					case 1: //Medium
						num_fruits = 3;
						max_speed = 3;
					//	alt_up_pixel_buffer_dma_draw_rectangle(pixel_buffer, 58, 187, 62, 191, 0, 0);
						RedrawBackground(58, 187, 62, 191, pixel_buffer);
						alt_up_pixel_buffer_dma_draw_rectangle(pixel_buffer, 58, 195, 62, 199, 0x05FF, 0);
						break;
					case 2: //Hard
						num_fruits = 5;
						max_speed = 5;
					//	alt_up_pixel_buffer_dma_draw_rectangle(pixel_buffer, 58, 195, 62, 199, 0, 0);
						RedrawBackground(58, 195, 62, 199, pixel_buffer);
						alt_up_pixel_buffer_dma_draw_rectangle(pixel_buffer, 58, 203, 62, 207, 0x05FF, 0);
						break;
					case 3: //Blizzard
						num_fruits = 10;
						max_speed = 5;
					//	alt_up_pixel_buffer_dma_draw_rectangle(pixel_buffer, 58, 203, 62, 207, 0, 0);
						RedrawBackground(58, 203, 62, 207, pixel_buffer);
						alt_up_pixel_buffer_dma_draw_rectangle(pixel_buffer, 58, 211, 62, 215, 0x05FF, 0);
						break;
				}

				if (!(0x1 & keys) ){//||(data == KEYBOARD_ONE && decode == KB_BREAK_CODE )) { // Right key to choose level
					printf("DOWN, %d \n",key_count);
					key_count++;
					key_count%=4;
					usleep(250000);
				}
				if (!(0x2 & keys) || (data == KEYBOARD_ENTER && decode ==KB_BINARY_MAKE_CODE )) { //Left push key or ENTER to select
					printf("ENTER\n");
					break;
				}
			}

		}

		alt_up_pixel_buffer_dma_clear_screen(pixel_buffer, 0);
		alt_up_char_buffer_clear(char_buffer);

		/*********************************** Initialize Background *******************************************/
		DrawBackground( pixel_buffer );

		/*********************************** Initialization for Basket ***************************************/
		DrawBmpBasket(basket_left, HEIGHT_BASKET, pixel_buffer);

		/************************************ Initialize Fruit structs ****************************************/
		InitBmpFruit(num_fruits);

		/************************************ Action Starts Here ****************************************/
		while(1) {
			ticks_now = alt_nticks();
			elapsed_time = (ticks_now - ticks_start)/ticks_per_s;
			//printf("started: %d\n", ticks_start);
			//printf("%d\n", ticks_now);
			timeleft = GAME_LENGTH - elapsed_time;
			if(timeleft==0){
				printf("\nreally over");
				timeleft = GAME_LENGTH;
				break;
			}
			alt_u8 data = getKeyboard();
			keys = IORD(KEYS_BASE, 0); // IORD prevents processor reading from cache
			int count_f =0;
			for( count_f=0; count_f < num_fruits ; count_f++) {
				MoveBmpFruit(count_f, pixel_buffer);
			}

			if (!(0x1 & keys) || (data == KEYBOARD_RIGHT && decode ==KB_LONG_BINARY_MAKE_CODE )) {
				if(basket_right < 318) 	{
					MoveBasketRight(pixel_buffer);
				}
				else {
					usleep(1500);
				}
			} else if(!(0x2 & keys) || (data == KEYBOARD_LEFT && decode ==KB_LONG_BINARY_MAKE_CODE )) {
				if(basket_left > 1) {
					MoveBasketLeft(pixel_buffer);
					} else {
					usleep(1500);
				}
			} else if (!(0x8 & keys) || (data == KEYBOARD_ESCAPE && decode ==KB_BINARY_MAKE_CODE )) {
				gState = welcome;
				break;

			} else {
				usleep(1500);
			}
			char catched_char[20];
			sprintf(catched_char, "Score: %d  ", catched);
			alt_up_char_buffer_string(char_buffer, catched_char, 25, 2);

			char speed_char[20]; // print the speed of the fruit for debugging
			sprintf(speed_char, "Time Left: %d  ", timeleft);
			alt_up_char_buffer_string(char_buffer, speed_char, 25, 6);
		}
		alt_up_pixel_buffer_dma_clear_screen(pixel_buffer, 0);
		alt_up_char_buffer_clear(char_buffer);
	}

	return 0;
}

/**************************************** Function Definitions ****************************************/

//Fruit Related

/* Generate random parameters for BMP fruit at specified array index
 */
void GenBmpFruit(int fruit_index) {
	bmp_fruits[fruit_index].type = rand()%8+1; // Generate fruit types 1 - 8
	// 1 = apple, 2 = banana, 3 = cherry, 4 = lemon, 5 = orange, 6 = peach, 7 = pear, 8 = strawberry
	bmp_fruits[fruit_index].height = 0;
	bmp_fruits[fruit_index].speed = rand()%max_speed;
	bmp_fruits[fruit_index].sc = 0;
	bmp_fruits[fruit_index].one = 1;
	bmp_fruits[fruit_index].x_coord = rand()%20;
	//swapXCoordWithAvailable( fruit_index);
}

/* This function shuffles the available random x coordinates so fruits are
 * in random non-overlapping x coords
 * 20 is because 320 (width of screen) / 16 = 20 so there are max 20 fruits across screen at once
 */
void shuffleAvailableXcoords() {
	int i;
	int randIndex, temp;
	for(i=0; i<20 ; i++) {
		randIndex = rand()%20;
		temp = availableXcoords[i];
		availableXcoords[i] = availableXcoords[randIndex];
		availableXcoords[randIndex] = temp;
	}
}

void swapXCoordWithAvailable(int fruit_index) {
	int randAvailIndex, temp;
	randAvailIndex = rand()%(20-num_fruits)+ num_fruits; // Array of 20 [first num_fruit indices will be taken ... ]
	temp = availableXcoords[fruit_index];
	availableXcoords[fruit_index] = availableXcoords[randAvailIndex];
	availableXcoords[randAvailIndex] = temp;
}

/* Init n fruits on start
 * calls GenBmpFruit()
 */
void InitBmpFruit(int n) {
	shuffleAvailableXcoords();
	int i;

	for (i=0; i<n ;i++)	{
		bmp_fruits[i].type = rand()%8+1; // Generate fruit types 1 - 8
		// 1 = apple, 2 = banana, 3 = cherry, 4 = lemon, 5 = orange, 6 = peach, 7 = pear, 8 = strawberry
		bmp_fruits[i].height = 0;
		bmp_fruits[i].speed = rand()%max_speed;
		bmp_fruits[i].sc = 0;
		bmp_fruits[i].one = 1;
		bmp_fruits[i].x_coord = availableXcoords[i];
	}
}


/* Contains logic for catching and regenerating new random fruit for different x coordinates */
/* Calls MoveVariousFruits() */

void MoveBmpFruit( int i, alt_up_pixel_buffer_dma_dev* pixel_buffer) {
	//i is the index of the fruit to draw/refresh
	usleep(5000);
	int fruit_x_Left = bmp_fruits[i].x_coord *16;
	int fruit_x_Right = bmp_fruits[i].x_coord *16 + 16;

	if(fruit_x_Right >= basket_left && fruit_x_Left <= basket_right ) {  //if between bounds of boat/bowl
		if(bmp_fruits[i].height+16 >= HEIGHT_BASKET ) { // Caught

			//alt_up_pixel_buffer_dma_draw_box(pixel_buffer, fruit_x_Left, 0, fruit_x_Right , bmp_fruits[i].height+16, 0x0000, PIXEL_BUFFER_BASE);
			RedrawBackground(fruit_x_Left, bmp_fruits[i].height, fruit_x_Right , bmp_fruits[i].height+16, pixel_buffer);
			GenBmpFruit(i);
			catched++;
		} else {
			if( bmp_fruits[i].sc == max_speed - bmp_fruits[i].speed) {
				//alt_up_pixel_buffer_dma_draw_hline(pixel_buffer, 16*bmp_fruits[i].x_coord, 16*bmp_fruits[i].x_coord+16, bmp_fruits[i].height, 0x0000, 0);
				RedrawBackground(fruit_x_Left, bmp_fruits[i].height, fruit_x_Right, bmp_fruits[i].height+1, pixel_buffer);
				MoveVariousFruits(i, pixel_buffer);
				bmp_fruits[i].height+=bmp_fruits[i].one; //increase height of fruit
				bmp_fruits[i].sc = 0;
			} else {
				bmp_fruits[i].sc++; // increment speed count
			}
		}
	} else {
		if(bmp_fruits[i].height >= HEIGHT_BOTTOM) {
			//alt_up_pixel_buffer_dma_draw_box(pixel_buffer, fruit_x_Left, 0, fruit_x_Right , bmp_fruits[i].height+16, 0x0000, PIXEL_BUFFER_BASE);
			RedrawBackground(fruit_x_Left, bmp_fruits[i].height, fruit_x_Right, bmp_fruits[i].height+1, pixel_buffer);
			GenBmpFruit(i);
		} else {
			if( bmp_fruits[i].sc == max_speed - bmp_fruits[i].speed) {
				bmp_fruits[i].sc = 0;
				//alt_up_pixel_buffer_dma_draw_hline(pixel_buffer, 16*bmp_fruits[i].x_coord, 16*bmp_fruits[i].x_coord+16, bmp_fruits[i].height, 0x0000, 0);
				RedrawBackground(fruit_x_Left,  bmp_fruits[i].height, fruit_x_Right, bmp_fruits[i].height+1, pixel_buffer);
				MoveVariousFruits(i, pixel_buffer);
				bmp_fruits[i].height+=bmp_fruits[i].one;
			} else {
				bmp_fruits[i].sc++; // increment speed count
			}
		}
	}
}


void DrawBmpFruit(int fruit_array[], int x, int y, alt_up_pixel_buffer_dma_dev* pixel_buffer) {
	//printf("Draw bmp fruit");
	int i = 0;
	int j = 0;
	int k = 0;
	for (j = 0; j < 16; j++) {
		for (i = 0; i < 16; i++) {
			if(fruit_array[k] == 0x0000) //black pixel
				alt_up_pixel_buffer_dma_draw(pixel_buffer, background[x+i+(y+j)*320] , x+i, y+j); //
			else
				alt_up_pixel_buffer_dma_draw(pixel_buffer, fruit_array[k], x+i, y+j);
			k++;
		}
	}
}

/* Use case statements to draw each fruit represented by type */
/* i is the index of fruit in bmp_fruit[] array */

void MoveVariousFruits(int i, alt_up_pixel_buffer_dma_dev* pixel_buffer) {
	switch(bmp_fruits[i].type) { // Drawing fruit depend on type.

	case 1:
		DrawBmpFruit(apple, 16*bmp_fruits[i].x_coord, bmp_fruits[i].height+1, pixel_buffer);
		break;
	case 2:
		DrawBmpFruit(banana, 16*bmp_fruits[i].x_coord, bmp_fruits[i].height+1, pixel_buffer);
		break;
	case 3:
		DrawBmpFruit(cherry, 16*bmp_fruits[i].x_coord, bmp_fruits[i].height+1, pixel_buffer);
		break;
	case 4:
		DrawBmpFruit(lemon, 16*bmp_fruits[i].x_coord, bmp_fruits[i].height+1, pixel_buffer);
		break;
	case 5:
		DrawBmpFruit(orange, 16*bmp_fruits[i].x_coord, bmp_fruits[i].height+1, pixel_buffer);
		break;
	case 6:
		DrawBmpFruit(peach, 16*bmp_fruits[i].x_coord, bmp_fruits[i].height+1, pixel_buffer);
		break;
	case 7:
		DrawBmpFruit(pear, 16*bmp_fruits[i].x_coord, bmp_fruits[i].height+1, pixel_buffer);
		break;
	case 8:
		DrawBmpFruit(strawberry, 16*bmp_fruits[i].x_coord, bmp_fruits[i].height+1, pixel_buffer);
		break;
	}
}

//Basket related
void MoveBasketLeft(alt_up_pixel_buffer_dma_dev* pixel_buffer) {
	//alt_up_pixel_buffer_dma_draw_vline(pixel_buffer, basket_right, HEIGHT_BASKET, HEIGHT_BOTTOM, 0, 0 );
	RedrawBackground(basket_right-BASKET_MOVEMENT, HEIGHT_BASKET, basket_right, HEIGHT_BOTTOM, pixel_buffer);
	basket_left-=BASKET_MOVEMENT;
	basket_right-=BASKET_MOVEMENT;
	DrawBmpBasket(basket_left, HEIGHT_BASKET, pixel_buffer);
}

void MoveBasketRight(alt_up_pixel_buffer_dma_dev* pixel_buffer) {
	//alt_up_pixel_buffer_dma_draw_vline(pixel_buffer, basket_left, HEIGHT_BASKET, HEIGHT_BOTTOM, 0, 0 );
	RedrawBackground(basket_left, HEIGHT_BASKET, basket_left+ BASKET_MOVEMENT, HEIGHT_BOTTOM, pixel_buffer);
	basket_right+=BASKET_MOVEMENT;
	basket_left+=BASKET_MOVEMENT;
	DrawBmpBasket(basket_left, HEIGHT_BASKET, pixel_buffer);
}

void DrawBmpBasket(int x, int y, alt_up_pixel_buffer_dma_dev* pixel_buffer) {
	//printf("Draw new basket");
	int i = 0;
	int j = 0;
	int k = 0;
	for (j = 0; j < 32; j++) {
		for (i = 0; i < 32; i++) {
			if(basket[k] == 0) //black colour
				alt_up_pixel_buffer_dma_draw(pixel_buffer, background[x+i+(y+j)*320], x+i, y+j);
			else
				alt_up_pixel_buffer_dma_draw(pixel_buffer, basket[k], x+i, y+j);
			k++;
		}
	}
}

//Background Related
void DrawBackground(alt_up_pixel_buffer_dma_dev* pixel_buffer) {
	int a, b, g =0;
	for(a=0; a < 240; a++){
		for (b =0; b < 320; b++)	{
			alt_up_pixel_buffer_dma_draw(pixel_buffer, background[g], b, a);
			g++;
		}
	}
}

/* Draw rectangle of background colour from top left corner x0, y0 to bottom right corner x1, y1 */

void RedrawBackground(int x0, int y0, int x1, int y1, alt_up_pixel_buffer_dma_dev* pixel_buffer) {
	int a ;
	int b ;
		for(a=x0; a < x1+1; a++){
			for ( b=y0; b < y1+1; b++)	{
				alt_up_pixel_buffer_dma_draw(pixel_buffer, background[a+b*320], a, b); //
			}
		}
}

void initKeyboard() {
	ps2 = alt_up_ps2_open_dev("/dev/ps2_0");
	if(ps2 == NULL)
		printf("Error initializing PS2");
	else
		printf("PS2 connected\n");

	alt_up_ps2_init(ps2); //initialize device

	alt_up_ps2_clear_fifo(ps2); //clear buffer

	reset_keyboard(ps2);
	set_keyboard_rate(ps2, 0x00);

}

/* Code copied from Zoe on Github */

alt_u8 getKeyboard(){
	alt_u8 data;

	decode_scancode(ps2, &decode, &data, &ascii);

	return data;
}
