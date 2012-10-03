/**
 * @file screen.c
 * @author Julien "Juju" Savard <juju2143@gmail.com>
 * @author Julian Mackeben aka compu <compujucke@googlemail.com>
 * @version 3.0
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

#define VRAM (unsigned short*)0xA8000000;

unsigned short getPaletteColor(unsigned int color)
{
	unsigned short palette[16] = {0x0000, 0xa800, 0x0540, 0xaaa0, 0x0015, 0xa815, 0x0555, 0xad55,
					0x5aab, 0xfaab, 0x5feb, 0xffeb, 0x5abf, 0xfabf, 0x5fff, 0xffff};
	int rbtable[6] = {0,6,12,18,24,31};
	int gtable[6] = {0,12,25,37,50,63};
        unsigned int c = color;
        unsigned int d;
        if(c < 16)
        {
                return palette[c];
        }
        else if(c < 232)
        {
                d = c-16;
                return (rbtable[d/36]<<11)+(gtable[(d/6)%6]<<5)+rbtable[d%6];
        }
        else if(c < 256)
        {
                d = c-232;
                return ((d+1)<<11)+((d*2+2)<<5)+(d+1);
        }
	return 0;
}

void nio_pixel_set(int x, int y, unsigned int color)
{
	unsigned short *scr = VRAM;
	if(x >= 0 && x < LCD_WIDTH_PX && y >= 0 && y < LCD_HEIGHT_PX)
	{
		scr[y*LCD_WIDTH_PX+x] = getPaletteColor(color);
		Bdisp_SetPoint_DD(x, y, getPaletteColor(color));
	}
}

void nio_vram_pixel_set(int x, int y, unsigned int color)
{
	unsigned short *scr = VRAM;
	if(x >= 0 && x < LCD_WIDTH_PX && y >= 0 && y < LCD_HEIGHT_PX)
	{
		scr[y*LCD_WIDTH_PX+x] = getPaletteColor(color);
	}
}

void nio_pixel_putc(int x, int y, char ch, int bgColor, int textColor)
{
	int i, j, pixelOn;
	for(i = 0; i < NIO_CHAR_WIDTH; i++)
	{
		for(j = NIO_CHAR_HEIGHT; j > 0; j--)
		{
			pixelOn = MBCharSet8x6_definition[(unsigned char)ch][i] << j ;
			pixelOn = pixelOn & 0x80 ;
			if (pixelOn) 		nio_pixel_set(x+i,y+NIO_CHAR_HEIGHT-j,textColor);
			else if(!pixelOn) 	nio_pixel_set(x+i,y+NIO_CHAR_HEIGHT-j,bgColor);
		}
	}
}
void nio_pixel_puts(int x, int y, char* str, int bgColor, int textColor)
{
	int l = strlen(str);
	int i;
	int stop=0;
	for (i = 0; i < l && !stop; i++)
	{
		nio_pixel_putc(x, y, str[i], bgColor, textColor);
		x += NIO_CHAR_WIDTH;
		if (x >= LCD_WIDTH_PX-NIO_CHAR_WIDTH)
		{
			stop=1;
		}
	}
}
void nio_vram_pixel_putc(int x, int y, char ch, int bgColor, int textColor)
{
	int i, j, pixelOn;
	for(i = 0; i < NIO_CHAR_WIDTH; i++)
	{
		for(j = NIO_CHAR_HEIGHT; j > 0; j--)
		{
			pixelOn = MBCharSet8x6_definition[(unsigned char)ch][i] << j ;
			pixelOn = pixelOn & 0x80 ;
			if (pixelOn) 		nio_vram_pixel_set(x+i,y+NIO_CHAR_HEIGHT-j,textColor);
			else if(!pixelOn) 	nio_vram_pixel_set(x+i,y+NIO_CHAR_HEIGHT-j,bgColor);
		}
	}
}
void nio_vram_pixel_puts(int x, int y, char* str, int bgColor, int textColor)
{
	int l = strlen(str);
	int i;
	int stop=0;
	for (i = 0; i < l && !stop; i++)
	{
		nio_vram_pixel_putc(x, y, str[i], bgColor, textColor);
		x += NIO_CHAR_WIDTH;
		if (x >= LCD_WIDTH_PX-NIO_CHAR_WIDTH)
		{
			stop=1;
		}
	}
}
