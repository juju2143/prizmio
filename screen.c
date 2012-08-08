/**
 * @file screen.c
 * @author Julien "Juju" Savard <juju2143@gmail.com>
 * @author Julian Mackeben aka compu <compujucke@googlemail.com>
 * @version 0.1
 *
 * @section LICENSE
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 * MA 02110-1301  USA
 *
 * @section DESCRIPTION
 *
 * Screen functions
 */
#include <stdlib.h>
#include <string.h>
#include <fxcg/display.h>
#include "charmap.h"
#include "prizmio.h"

unsigned short palette[16] = { 
	COLOR_BLACK,
	COLOR_DARKRED,
	COLOR_GREEN,
	COLOR_OLIVE,
	COLOR_DARKBLUE,
	COLOR_DARKMAGENTA,
	COLOR_DARKCYAN,
	COLOR_GRAY,
	COLOR_DARKGRAY,
	COLOR_RED,
	COLOR_LIME,
	COLOR_YELLOW,
	COLOR_BLUE,
	COLOR_MAGENTA,
	COLOR_CYAN,
	COLOR_WHITE
};

void setPixel(int x, int y, unsigned int color)
{
	unsigned short *scr = GetVRAMAddress();
	if(x >= 0 && x < LCD_WIDTH_PX && y >= 0 && y < LCD_HEIGHT_PX)
	{
		scr[y*LCD_WIDTH_PX+x] = palette[color];
		Bdisp_SetPoint_DD(x, y, palette[color]);
	}
}

void setPixelVRAM(int x, int y, unsigned int color)
{
	unsigned short *scr = GetVRAMAddress();
	if(x >= 0 && x < LCD_WIDTH_PX && y >= 0 && y < LCD_HEIGHT_PX)
	{
		scr[y*LCD_WIDTH_PX+x] = palette[color];
	}
}

void putChar(int x, int y, char ch, int bgColor, int textColor)
{
	int i, j, pixelOn;
	for(i = 0; i < CHAR_WIDTH; i++)
	{
		for(j = CHAR_HEIGHT; j > 0; j--)
		{
			pixelOn = MBCharSet8x6_definition[(unsigned char)ch][i] << j ;
			pixelOn = pixelOn & 0x80 ;
			if (pixelOn) 		setPixel(x+i,y+CHAR_HEIGHT-j,textColor);
			else if(!pixelOn) 	setPixel(x+i,y+CHAR_HEIGHT-j,bgColor);
		}
	}
}
void putStr(int x, int y, char* str, int bgColor, int textColor)
{
	int l = strlen(str);
	int i;
	int stop=0;
	for (i = 0; i < l && !stop; i++)
	{
		putChar(x, y, str[i], bgColor, textColor);
		x += CHAR_WIDTH;
		if (x >= LCD_WIDTH_PX-CHAR_WIDTH)
		{
			stop=1;
		}
	}
}
void putCharVRAM(int x, int y, char ch, int bgColor, int textColor)
{
	int i, j, pixelOn;
	for(i = 0; i < CHAR_WIDTH; i++)
	{
		for(j = CHAR_HEIGHT; j > 0; j--)
		{
			pixelOn = MBCharSet8x6_definition[(unsigned char)ch][i] << j ;
			pixelOn = pixelOn & 0x80 ;
			if (pixelOn) 		setPixelVRAM(x+i,y+CHAR_HEIGHT-j,textColor);
			else if(!pixelOn) 	setPixelVRAM(x+i,y+CHAR_HEIGHT-j,bgColor);
		}
	}
}
void putStrVRAM(int x, int y, char* str, int bgColor, int textColor)
{
	int l = strlen(str);
	int i;
	int stop=0;
	for (i = 0; i < l && !stop; i++)
	{
		putCharVRAM(x, y, str[i], bgColor, textColor);
		x += CHAR_WIDTH;
		if (x >= LCD_WIDTH_PX-CHAR_WIDTH)
		{
			stop=1;
		}
	}
}
