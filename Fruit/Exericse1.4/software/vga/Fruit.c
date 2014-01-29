/*
 * VGA Display code -- The imported libraries will provide methods to draw lines and characters to VGA.:update
 *
 *
 */
#include <stdio.h>
#include <string.h>
#include <unistd.h>
//Library for writing to LCD
#include "altera_up_avalon_character_lcd.h"
//Library for VGA
#include "altera_up_avalon_video_pixel_buffer_dma.h"
//Library for Character buffer
#include "altera_up_avalon_video_character_buffer_with_dma.h"
#define switches (volatile char *) 0x0004010
#define leds (char *) 0x0004000
#define keys (volatile char * ) 0x0004020
#define WIDTH 320
#define HEIGHT 240

int main(void)
{
	printf("Hello from Nios II!\n");
	alt_up_pixel_buffer_dma_dev* pixel_buffer;
	// Use the name of your pixel buffer DMA core
	pixel_buffer =
	alt_up_pixel_buffer_dma_open_dev("/dev/video_pixel_buffer_dma_0");
	// Set the background buffer address – Although we don’t use the background,
	// they only provide a function to change the background buffer address, so
	// we must set that, and then swap it to the foreground.
	alt_up_pixel_buffer_dma_change_back_buffer_address(pixel_buffer,	PIXEL_BUFFER_BASE);
	// Swap background and foreground buffers
	alt_up_pixel_buffer_dma_swap_buffers(pixel_buffer);
	// Wait for the swap to complete
	while(alt_up_pixel_buffer_dma_check_swap_buffers_status(pixel_buffer));
	// Clear the screen
	alt_up_pixel_buffer_dma_clear_screen(pixel_buffer, 0);


	// Initialize
	alt_up_char_buffer_dev *char_buffer;
	char_buffer = alt_up_char_buffer_open_dev("/dev/video_character_buffer_with_dma_0");
	alt_up_char_buffer_init(char_buffer);

	//LCD CODE
	alt_up_character_lcd_dev * char_lcd_dev;  // open the Character LCD port
	char_lcd_dev = alt_up_character_lcd_open_dev ("/dev/character_lcd_0");

	if ( char_lcd_dev == NULL)
		alt_printf ("Error: could not open character LCD device\n");
	else
		alt_printf ("Opened character LCD device\n");

	alt_up_character_lcd_init (char_lcd_dev);	/* Initialize the character display */

	alt_up_character_lcd_set_cursor_pos(char_lcd_dev, 0, 0);
	alt_up_character_lcd_string(char_lcd_dev, "Switches");
	alt_up_character_lcd_set_cursor_pos(char_lcd_dev, 12, 0);
	alt_up_character_lcd_string(char_lcd_dev, "Keys");

	// Draw a white line to the foreground buffer
	alt_up_pixel_buffer_dma_draw_line(pixel_buffer, 0, 0, 320, 240, 0x7CFC00 ,	0);
	//alt_up_pixel_buffer_dma_draw_rectangle(pixel_buffer, 0, 0, 5, 5, 0xFFFF, PIXEL_BUFFER_BASE);
	alt_up_pixel_buffer_dma_draw_box(pixel_buffer, 0, 0, 5, 5, 0xFFFF, PIXEL_BUFFER_BASE);
	while (1) {
		int i;
		int box_width = 5;
		int box_height = 5;
		for (i = 0; i<=HEIGHT; i++) {
			usleep(10000);
			int x0 = 0;
			int y0 = 0;

			alt_up_pixel_buffer_dma_draw_box(pixel_buffer, x0, y0, box_width , i, 0x0000, PIXEL_BUFFER_BASE);
			alt_up_pixel_buffer_dma_draw_box(pixel_buffer, x0, i, box_width, i+box_height, 0xFFFF, PIXEL_BUFFER_BASE);
		}

		*leds = *switches;

		if (!(0x1 & *keys)) {
			alt_up_character_lcd_set_cursor_pos(char_lcd_dev, 14, 1);
			alt_up_character_lcd_string(char_lcd_dev, "0");
			alt_up_char_buffer_string(char_buffer, "Key 0   ", 40, 30);
		} else {
			alt_up_character_lcd_set_cursor_pos(char_lcd_dev, 14, 1);
			alt_up_character_lcd_string(char_lcd_dev, " ");
		}
		if (!(0x2 & *keys)) {
			alt_up_character_lcd_set_cursor_pos(char_lcd_dev, 13, 1);
			alt_up_character_lcd_string(char_lcd_dev, "1");
			alt_up_char_buffer_string(char_buffer, "Key 1   ", 40, 30);
		} else {
			alt_up_character_lcd_set_cursor_pos(char_lcd_dev, 13, 1);
			alt_up_character_lcd_string(char_lcd_dev, " ");
		}
		if (!(0x4 & *keys)) {
			alt_up_character_lcd_set_cursor_pos(char_lcd_dev, 12, 1);
			alt_up_character_lcd_string(char_lcd_dev, "2");
			alt_up_char_buffer_string(char_buffer, "Key 2   ", 40, 30);
		} else {
			alt_up_character_lcd_set_cursor_pos(char_lcd_dev, 12, 1);
			alt_up_character_lcd_string(char_lcd_dev, " ");
		}
		if (!(0x8 & *keys)) {
			alt_up_character_lcd_set_cursor_pos(char_lcd_dev, 11, 1);
			alt_up_character_lcd_string(char_lcd_dev, "3");
			alt_up_char_buffer_string(char_buffer, "Key 3", 40, 30);
		} else {
			alt_up_character_lcd_set_cursor_pos(char_lcd_dev, 11, 1);
			alt_up_character_lcd_string(char_lcd_dev, " ");
		}
		//Switches start here
		if (0x01 & *switches){
			alt_up_character_lcd_set_cursor_pos(char_lcd_dev, 7, 1);
			alt_up_character_lcd_string(char_lcd_dev, "0");
			alt_up_char_buffer_string(char_buffer, "Switch 0 ", 40, 30);
		} else {
			alt_up_character_lcd_set_cursor_pos(char_lcd_dev, 7, 1);
			alt_up_character_lcd_string(char_lcd_dev, " ");
		}
		if (0x02 & *switches){
			alt_up_character_lcd_set_cursor_pos(char_lcd_dev, 6, 1);
			alt_up_character_lcd_string(char_lcd_dev, "1");
			alt_up_char_buffer_string(char_buffer, "Switch 1 ", 40, 30);
		} else {
			alt_up_character_lcd_set_cursor_pos(char_lcd_dev, 6, 1);
			alt_up_character_lcd_string(char_lcd_dev, " ");
		}
		if (0x04 & *switches){
			alt_up_character_lcd_set_cursor_pos(char_lcd_dev, 5, 1);
			alt_up_character_lcd_string(char_lcd_dev, "2");
			alt_up_char_buffer_string(char_buffer, "Switch 2 ", 40, 30);
		} else {
			alt_up_character_lcd_set_cursor_pos(char_lcd_dev, 5, 1);
			alt_up_character_lcd_string(char_lcd_dev, " ");
		}
		if (0x08 & *switches){
			alt_up_character_lcd_set_cursor_pos(char_lcd_dev, 4, 1);
			alt_up_character_lcd_string(char_lcd_dev, "3");
			alt_up_char_buffer_string(char_buffer, "Switch 3 ", 40, 30);
		} else {
			alt_up_character_lcd_set_cursor_pos(char_lcd_dev, 4, 1);
			alt_up_character_lcd_string(char_lcd_dev, " ");
		}
		if (0x10 & *switches){
			alt_up_character_lcd_set_cursor_pos(char_lcd_dev, 3, 1);
			alt_up_character_lcd_string(char_lcd_dev, "4");
			alt_up_char_buffer_string(char_buffer, "Switch 4 ", 40, 30);
		} else {
			alt_up_character_lcd_set_cursor_pos(char_lcd_dev, 3, 1);
			alt_up_character_lcd_string(char_lcd_dev, " ");
		}
		if (0x20 & *switches){
			alt_up_character_lcd_set_cursor_pos(char_lcd_dev, 2, 1);
			alt_up_character_lcd_string(char_lcd_dev, "5");
			alt_up_char_buffer_string(char_buffer, "Switch 5 ", 40, 30);
		} else {
			alt_up_character_lcd_set_cursor_pos(char_lcd_dev, 2, 1);
			alt_up_character_lcd_string(char_lcd_dev, " ");
		}
		if (0x40 & *switches){
			alt_up_character_lcd_set_cursor_pos(char_lcd_dev, 1, 1);
			alt_up_character_lcd_string(char_lcd_dev, "6");
			alt_up_char_buffer_string(char_buffer, "Switch 6 ", 40, 30);
		} else {
			alt_up_character_lcd_set_cursor_pos(char_lcd_dev, 1, 1);
			alt_up_character_lcd_string(char_lcd_dev, " ");
		}
		if (0x80 & *switches){
			alt_up_character_lcd_set_cursor_pos(char_lcd_dev, 0, 1);
			alt_up_character_lcd_string(char_lcd_dev, "7");
			alt_up_char_buffer_string(char_buffer, "Switch 7 ", 40, 30);
		} else {
			alt_up_character_lcd_set_cursor_pos(char_lcd_dev, 0, 1);
			alt_up_character_lcd_string(char_lcd_dev, " ");
		}
	}
	return 0;
}
