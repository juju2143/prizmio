/**
 * @file console.c
 * @author  Julien "Juju" Savard <juju2143@gmail.com>
 * @author  Julian Mackeben aka compu <compujuckel@googlemail.com>
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
 * Console functions
 */
#include "prizmio.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <fxcg/keyboard.h>

nio_console* nio_default = NULL;

const unsigned short* keyboard_register = (unsigned short*)0xA44B0000;
unsigned short lastkey[8];
unsigned short holdkey[8];

void keyupdate(void)
{
	memcpy(holdkey, lastkey, sizeof(unsigned short)*8);
	memcpy(lastkey, keyboard_register, sizeof(unsigned short)*8);
}
int keydownlast(int basic_keycode)
{
	int row, col, word, bit; 
	row = basic_keycode%10; 
	col = basic_keycode/10-1; 
	word = row>>1; 
	bit = col + 8*(row&1); 
	return (0 != (lastkey[word] & 1<<bit)); 
}
int keydownhold(int basic_keycode)
{
	int row, col, word, bit; 
	row = basic_keycode%10; 
	col = basic_keycode/10-1; 
	word = row>>1; 
	bit = col + 8*(row&1); 
	return (0 != (holdkey[word] & 1<<bit)); 
}

int isKeyPressed(int basic_keycode)
{
	return (keydownlast(basic_keycode) && !keydownhold(basic_keycode));
}

void nio_grid_puts(const int offset_x, const int offset_y, const int x, const int y, const char *str, const unsigned char bgColor, const unsigned char textColor)
{
	nio_pixel_puts(offset_x+x*6,offset_y+y*8,str,bgColor,textColor);
}
void nio_grid_putc(const int offset_x, const int offset_y, const int x, const int y, const char ch, const unsigned char bgColor, const unsigned char textColor)
{
	nio_pixel_putc(offset_x+x*6,offset_y+y*8,ch,bgColor,textColor);
}

void nio_vram_grid_puts(const int offset_x, const int offset_y, const int x, const int y, const char *str, const unsigned char bgColor, const unsigned char textColor)
{
	nio_vram_pixel_puts(offset_x+x*6,offset_y+y*8,str,bgColor,textColor);
}
void nio_vram_grid_putc(const int offset_x, const int offset_y, const int x, const int y, const char ch, const unsigned char bgColor, const unsigned char textColor)
{
	nio_vram_pixel_putc(offset_x+x*6,offset_y+y*8,ch,bgColor,textColor);
}

void nio_use_stdio(void)
{
    nio_default = malloc(sizeof(nio_console));
    nio_init(nio_default,NIO_MAX_COLS,NIO_MAX_ROWS,0,0,NIO_COLOR_WHITE,NIO_COLOR_BLACK,TRUE);
}

void nio_free_stdio(void)
{
    nio_free(nio_default);
}

void nio_load(const char* path, nio_console* c)
{
	FILE* f = fopen(path,"rb");
	
	fread(&c->cursor_x,sizeof(int),1,f);
	fread(&c->cursor_y,sizeof(int),1,f);
	
	fread(&c->max_x,sizeof(int),1,f);
	fread(&c->max_y,sizeof(int),1,f);
	
	fread(&c->offset_x,sizeof(int),1,f);
	fread(&c->offset_y,sizeof(int),1,f);
	
	fread(&c->default_background_color,sizeof(char),1,f);
	fread(&c->default_foreground_color,sizeof(char),1,f);
	
	fread(&c->drawing_enabled,sizeof(BOOL),1,f);
    
    fread(&c->cursor_enabled,sizeof(BOOL),1,f);
    fread(&c->cursor_type,sizeof(int),1,f);
    fread(&c->cursor_line_width,sizeof(int),1,f);
    fread(&c->cursor_custom_data,sizeof(char)*6,1,f);
    fread(&c->cursor_blink_enabled,sizeof(BOOL),1,f);
    fread(&c->cursor_blink_status,sizeof(BOOL),1,f);
    fread(&c->cursor_blink_timestamp,sizeof(BOOL),1,f);
    fread(&c->cursor_blink_duration,sizeof(BOOL),1,f);
	
	c->data = malloc(c->max_x*c->max_y);
	c->color = malloc(c->max_x*c->max_y);
	
	fread(c->data,sizeof(char),c->max_x*c->max_y,f);
	fread(c->color,sizeof(short),c->max_x*c->max_y,f);
	
    if(c->drawing_enabled)
        nio_fflush(c);
    
	fclose(f);
}

void nio_save(const char* path, const nio_console* c)
{
	FILE* f = fopen(path,"wb");
	
	fwrite(&c->cursor_x,sizeof(int),1,f);
	fwrite(&c->cursor_y,sizeof(int),1,f);
	
	fwrite(&c->max_x,sizeof(int),1,f);
	fwrite(&c->max_y,sizeof(int),1,f);
	
	fwrite(&c->offset_x,sizeof(int),1,f);
	fwrite(&c->offset_y,sizeof(int),1,f);
	
	fwrite(&c->default_background_color,sizeof(char),1,f);
	fwrite(&c->default_foreground_color,sizeof(char),1,f);
	
	fwrite(&c->drawing_enabled,sizeof(BOOL),1,f);
    
    fwrite(&c->cursor_enabled,sizeof(BOOL),1,f);
    fwrite(&c->cursor_type,sizeof(int),1,f);
    fwrite(&c->cursor_line_width,sizeof(int),1,f);
    fwrite(&c->cursor_custom_data,sizeof(char)*6,1,f);
    fwrite(&c->cursor_blink_enabled,sizeof(BOOL),1,f);
    fwrite(&c->cursor_blink_status,sizeof(BOOL),1,f);
    fwrite(&c->cursor_blink_timestamp,sizeof(BOOL),1,f);
    fwrite(&c->cursor_blink_duration,sizeof(BOOL),1,f);
	
	fwrite(c->data,sizeof(char),c->max_x*c->max_y,f);
	fwrite(c->color,sizeof(short),c->max_x*c->max_y,f);
	
	fclose(f);
}

void nio_set_default(nio_console* c)
{
    nio_default = c;
}

BOOL shift = FALSE;
BOOL caps = FALSE;
BOOL ctrl = FALSE;
BOOL optn = FALSE;
static char shiftKey(char normalc, char shiftc)
{
	if(shift || caps) 
	{
		shift = FALSE;
		return shiftc;
	}
	else return normalc;
}
static char shiftOrCtrlKey(char normalc, char shiftc, char ctrlc)
{
	if(shift || caps)
	{
		shift = FALSE;
		return shiftc;
	}
	else if(ctrl)
	{
		ctrl = FALSE;
		return ctrlc;
	}
	else return normalc;
}
static char lowerOrUpperKey(char lowerc, char upperc)
{
	if(optn) return upperc;
	else return lowerc;
}
char nio_getch(nio_console* c)
{
	int key = 0;
	while(1)
	{
		while (!KeyPressed())
            nio_cursor_blinking_draw(c);
		
        nio_cursor_erase(c);
		keyupdate();

		// Ctrl, Shift, Caps first
		if(isKeyPressed(KEY_PRGM_SHIFT))
		{
			if(ctrl) ctrl = FALSE;
			else ctrl = TRUE;
		}
		if(isKeyPressed(68)) // OPTN
		{
			if(optn) optn = FALSE;
			else optn = TRUE;
		}
		if(isKeyPressed(KEY_PRGM_ALPHA))
		{
			if(ctrl)
			{
				ctrl = FALSE;
				shift = FALSE;
				caps = TRUE;
			}
			else if(caps) caps = FALSE;
			else if(shift) shift = FALSE;
			else shift = TRUE;
		}

		if(isKeyPressed(KEY_PRGM_MENU)) return 0;
		if(isKeyPressed(KEY_PRGM_EXIT)) return 0;
		if(isKeyPressed(KEY_PRGM_ACON)) return 0;

		if(isKeyPressed(67)) return shiftKey('`', '$');
		if(isKeyPressed(57)) return shiftKey('^', '#');
		
		// Characters
		if(isKeyPressed(76)) return lowerOrUpperKey('a', 'A');
		if(isKeyPressed(66)) return lowerOrUpperKey('b', 'B');
		if(isKeyPressed(56)) return lowerOrUpperKey('c', 'C');
		if(isKeyPressed(46)) return lowerOrUpperKey('d', 'D');
		if(isKeyPressed(36)) return lowerOrUpperKey('e', 'E');
		if(isKeyPressed(26)) return lowerOrUpperKey('f', 'F');

		if(isKeyPressed(75)) return shiftKey('<', lowerOrUpperKey('g', 'G'));
		if(isKeyPressed(65)) return shiftKey('>', lowerOrUpperKey('h', 'H'));
		if(isKeyPressed(55)) return shiftKey('(', lowerOrUpperKey('i', 'I'));
		if(isKeyPressed(45)) return shiftKey(')', lowerOrUpperKey('j', 'J'));
		if(isKeyPressed(35)) return shiftKey(',', lowerOrUpperKey('k', 'K'));
		if(isKeyPressed(25)) return shiftKey('\t', lowerOrUpperKey('l', 'L'));

		if(isKeyPressed(74)) return shiftKey('7', lowerOrUpperKey('m', 'M'));
		if(isKeyPressed(64)) return shiftKey('8', lowerOrUpperKey('n', 'N'));
		if(isKeyPressed(54)) return shiftKey('9', lowerOrUpperKey('o', 'O'));
		if(isKeyPressed(44)) return '\b';

		if(isKeyPressed(73)) return shiftKey('4', lowerOrUpperKey('p', 'P'));
		if(isKeyPressed(63)) return shiftKey('5', lowerOrUpperKey('q', 'Q'));
		if(isKeyPressed(53)) return shiftKey('6', lowerOrUpperKey('r', 'R'));
		if(isKeyPressed(43)) return shiftOrCtrlKey('*', lowerOrUpperKey('s', 'S'), '{');
		if(isKeyPressed(33)) return shiftOrCtrlKey('/', lowerOrUpperKey('t', 'T'), '}');

		if(isKeyPressed(72)) return shiftKey('1', lowerOrUpperKey('u', 'U'));
		if(isKeyPressed(62)) return shiftKey('2', lowerOrUpperKey('v', 'V'));
		if(isKeyPressed(52)) return shiftOrCtrlKey('3', lowerOrUpperKey('w', 'W'), ';');
		if(isKeyPressed(42)) return shiftOrCtrlKey('+', lowerOrUpperKey('x', 'X'), '[');
		if(isKeyPressed(32)) return shiftOrCtrlKey('-', lowerOrUpperKey('y', 'Y'), ']');

		if(isKeyPressed(71)) return shiftOrCtrlKey('0', lowerOrUpperKey('z', 'Z'), '|');
		if(isKeyPressed(61)) return shiftOrCtrlKey('.', ' ', '=');
		if(isKeyPressed(51)) return shiftOrCtrlKey('_', '"', '?');
		if(isKeyPressed(41)) return shiftOrCtrlKey('~', ':', '!');
		if(isKeyPressed(31)) return '\n';
		
/*		// Symbols
		if(isKeyPressed(KEY_CHAR_COMMA))		return shiftKey(',',';');
		if(isKeyPressed(KEY_CHAR_DP)) 	return shiftKey('.',':');
		//if(isKeyPressed(KEY_CHAR_COLON))		return ':';
		if(isKeyPressed(KEY_CHAR_LPAR))			return '(';
		if(isKeyPressed(KEY_CHAR_RPAR))			return ')';
		if(isKeyPressed(KEY_CHAR_SPACE))		return shiftKey(' ','_');
		if(isKeyPressed(KEY_CHAR_DIV))		return shiftKey('/','\\');
		if(isKeyPressed(KEY_CHAR_MULT))	return shiftKey('*','\"');
		if(isKeyPressed(KEY_CHAR_MINUS))		return shiftKey('-','_');
		if(isKeyPressed(KEY_CHAR_PMINUS))	return shiftKey('-','_');
		if(isKeyPressed(KEY_CHAR_PLUS))		return '+';
		if(isKeyPressed(KEY_CHAR_EQUAL))		return '=';
		//if(isKeyPressed(KEY_CHAR_LTHAN))		return '<';
		//if(isKeyPressed(KEY_CHAR_GTHAN))		return '>';
		if(isKeyPressed(KEY_CHAR_LBRCKT))		return '[';
		if(isKeyPressed(KEY_CHAR_RBRCKT))		return ']';
		if(isKeyPressed(KEY_CHAR_LBRACE))		return '{';
		if(isKeyPressed(KEY_CHAR_RBRACE))		return '}';
		if(isKeyPressed(KEY_CHAR_DQUATE))		return '\"';
		//if(isKeyPressed(KEY_CHAR_APOSTROPHE))	return '\'';
		//if(isKeyPressed(KEY_CHAR_QUES))		return shiftKey('?','!');
		//if(isKeyPressed(KEY_CHAR_QUESEXCL))	return shiftKey('?','!');
		if(isKeyPressed(KEY_CHAR_ANS))		return '|';
		if(isKeyPressed(KEY_CHAR_EXP))		return '^';
		if(isKeyPressed(KEY_CTRL_EXE))		return shiftKey('\n','~');
		if(isKeyPressed(KEY_CHAR_SQUARE))		return '\B2';
		
		// Special chars
		if(isKeyPressed(KEY_CHAR_CR))		return '\n';
		if(isKeyPressed(KEY_CTRL_DEL))		return '\b';
		if(isKeyPressed(KEY_CHAR_STORE))		return '\t';
*/
	}
	return 0;
}

void nio_init(nio_console* c, const int size_x, const int size_y, const int offset_x, const int offset_y, const unsigned char background_color, const unsigned char foreground_color, const BOOL drawing_enabled)
{
	c->max_x = size_x;
	c->max_y = size_y;
	c->offset_x = offset_x;
	c->offset_y = offset_y;
	c->cursor_x = 0;
	c->cursor_y = 0;
	c->drawing_enabled = TRUE;
	c->default_background_color = background_color;
	c->default_foreground_color = foreground_color;
	c->data = malloc(c->max_x*c->max_y);
	c->color = malloc(c->max_x*c->max_y*2);
    c->color = malloc(c->max_x*c->max_y);
    c->cursor_enabled = TRUE;
	c->cursor_blink_enabled = TRUE;
	c->cursor_blink_duration = 1;
	c->cursor_type = 0;
	c->cursor_line_width = 1;
	nio_clear(c);
}

int nio_fflush(nio_console* c)
{
	int row, col;
	for(row = 0; row < c->max_y; row++)
	{
		for(col = 0; col < c->max_x; col++)
		{
			nio_vram_csl_drawchar(c,col,row);
		}
	}
	Bdisp_PutDisp_DD();
    return 0;
}

void nio_clear(nio_console* c)
{
	unsigned short color = (c->default_background_color << 8) | c->default_foreground_color;
	memset(c->data,0,c->max_x*c->max_y);
	int i;
	for(i = 0; i < c->max_x*c->max_y; i++)
	{
		c->color[i] = color;
	}
	c->cursor_x = 0;
	c->cursor_y = 0;
	if(c->drawing_enabled)
		nio_fflush(c);
}

void nio_scroll(nio_console* c)
{
	char* temp;
	unsigned short* temp2;
	temp = malloc(c->max_x*c->max_y);
	temp = malloc(c->max_x*c->max_y*2);
	
	memset(temp,0,c->max_x*c->max_y);
	memcpy(temp,c->data+c->max_x,c->max_x*(c->max_y-1));
	memcpy(c->data,temp,c->max_x*c->max_y);
	
	unsigned short color = (c->default_background_color << 8) | c->default_foreground_color;
	int i;
	for(i = 0; i < c->max_x*c->max_y; i++)
	{
		temp2[i] = color;
	}
	memcpy(temp2,c->color+c->max_x,c->max_x*(c->max_y-1)*2);
	memcpy(c->color,temp2,c->max_x*c->max_y*2);
	
	if(c->cursor_y > 0)
		c->cursor_y--;
	c->cursor_x = 0;
	
	free(temp);
}

void nio_csl_drawchar(nio_console* c, const int pos_x, const int pos_y)
{
	char ch = c->data[pos_y*c->max_x+pos_x];
	unsigned short color = c->color[pos_y*c->max_x+pos_x];
	
	unsigned char background_color = (color & 0xFF00) >> 8;
	unsigned char foreground_color = color & 0xFF;
	
	nio_grid_putc(c->offset_x, c->offset_y, pos_x, pos_y, ch == 0 ? ' ' : ch, background_color, foreground_color);
}

void nio_vram_csl_drawchar(nio_console* c, const int pos_x, const int pos_y)
{
	char ch = c->data[pos_y*c->max_x+pos_x];
	unsigned short color = c->color[pos_y*c->max_x+pos_x];
	
	unsigned char background_color = (color & 0xFF00) >> 8;
	unsigned char foreground_color = color & 0xFF;
	
	nio_vram_grid_putc(c->offset_x, c->offset_y, pos_x, pos_y, ch == 0 ? ' ' : ch, background_color, foreground_color);
}

void nio_csl_savechar(nio_console* c, const char ch, const int pos_x, const int pos_y)
{
	unsigned short color = (c->default_background_color << 8) | c->default_foreground_color;
	
	c->data[pos_y*c->max_x+pos_x] = ch;
	c->color[pos_y*c->max_x+pos_x] = color;
}

char nio_fputc(char ch, nio_console* c)
{
	// Newline. Increment Y cursor, set X cursor to zero. Scroll if necessary.
	if(ch == '\n')
	{
		c->cursor_x = 0;
		c->cursor_y++;
		// Scrolling necessary?
		if(c->cursor_y >= c->max_y)
		{
			nio_scroll(c);
			if(c->drawing_enabled)
				nio_fflush(c);
		}
	}
	// Carriage return. Set X cursor to zero.
	else if(ch == '\r')
	{
		c->cursor_x = 0;
	}
	// Backspace. Decrement X cursor.
	else if(ch == '\b')
	{
		if(c->cursor_x > 0)
			c->cursor_x--;
	}
	else if(ch == '\t')
	{
		// tabs are 8 character long
		c->cursor_x += (9-1) - c->cursor_x % 8;
		ch = ' ';
	}
	// Must be a normal character...
	else
	{
		// Check if the cursor is valid
		if(c->cursor_x >= c->max_x)
		{
			c->cursor_x = 0;
			c->cursor_y++;
		}
		if(c->cursor_y >= c->max_y)
		{
			nio_scroll(c);
			if(c->drawing_enabled)
				nio_fflush(c);
		}
		// Then store it.
		nio_csl_savechar(c,ch,c->cursor_x,c->cursor_y);
		
		// Draw it when BOOL draw is true
		if(c->drawing_enabled) nio_csl_drawchar(c,c->cursor_x,c->cursor_y);
		
		// Increment X cursor. It will be checked for validity next time.
		c->cursor_x++;
	}
    return ch;
}

char nio_putchar(const char ch)
{
    return nio_fputc(ch,nio_default);
}

int nio_fputs(const char* str, nio_console* c)
{
	int len = strlen(str);
	int i;
	for(i = 0; i < len; i++)
	{
		nio_fputc(str[i], c);
	}
    return 1;
}

int nio_puts(const char* str)
{
    return nio_fputs(str,nio_default);
}

int nio_fprintf(nio_console* c, const char *format, ...)
{
	char buf[1000];
	memset(buf,'\0',sizeof(buf));
	__builtin_va_list arglist;
	__builtin_va_start(arglist,format);
	vsprintf(buf,format,*(char **)&arglist);
	nio_fputs(buf,c);
	__builtin_va_end(arglist);
    return strlen(buf);
}

int nio_printf(const char *format, ...)
{
	char buf[1000];
	memset(buf,'\0',sizeof(buf));
	__builtin_va_list arglist;
	__builtin_va_start(arglist,format);
	vsprintf(buf,format,*(char **)&arglist);
	nio_puts(buf);
	__builtin_va_end(arglist);
    return strlen(buf);
}

void nio_perror(const char* str)
{
    //nio_printf("%s%s",str,strerror(errno));
}

void nio_color(nio_console* c, const unsigned char background_color, const unsigned char foreground_color)
{
	c->default_background_color = background_color;
	c->default_foreground_color = foreground_color;
}

void nio_drawing_enabled(nio_console* c, const BOOL enable_drawing)
{
	c->drawing_enabled = enable_drawing;
}

char nio_fgetc(nio_console* c)
{
	char ch = nio_getch(c);
	nio_fputc(ch,c);
    nio_cursor_draw(c);
	return ch;
}

char nio_getchar(void)
{
    return nio_fgetc(nio_default);
}

char* nio_fgets(char* str, int num, nio_console* c)
{
	char tmp;
	int old_x = c->cursor_x;
	int old_y = c->cursor_y;
	
	int i = 0;
	while(1)
	{
        nio_cursor_draw(c);
		c->cursor_blink_status = TRUE;
		nio_cursor_blinking_reset(c);
		//wait_no_key_pressed();
		tmp = nio_getch(c);
		if(tmp == '\n')
		{
			str[i] = '\0';
			nio_fputc('\n',c);
			return i > 0 ? str : NULL;
		}
		else if(tmp == '\b')
		{
			if(c->cursor_x == 0 && c->cursor_y > old_y && i > 0)
			{
				c->cursor_y--;
				c->cursor_x = c->max_x;
			}
			if((c->cursor_x > old_x || (c->cursor_x > 0 && c->cursor_y > old_y )) && i > 0)
			{
				nio_fputs("\b \b",c);
				i--;
			}
		}
		else if(tmp == '\0')
		{
			str[0] = '\0';
			nio_fputc('\n',c);
			return NULL;
		}
		else
		{
			str[i] = tmp;
			nio_fputc(tmp,c);
			i++;
		}
	}
}

char* nio_gets(char* str)
{
    return nio_fgets(str,1000,nio_default); // using 1000 as default here
}

void nio_free(nio_console* c)
{
	free(c->data);
	free(c->color);
}
