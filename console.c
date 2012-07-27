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
#include <stdlib.h>
#include <string.h>
#include <keyboard.hpp>
#include "prizmio.h"

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

void nio_drawstr(int offset_x, int offset_y, int x, int y, char *str, char bgColor, char textColor)
{
	putStr(offset_x+x*6,offset_y+y*8,str,bgColor,textColor);
}
void nio_drawch(int offset_x, int offset_y, int x, int y, char ch, char bgColor, char textColor)
{
	putChar(offset_x+x*6,offset_y+y*8,ch,bgColor,textColor);
}

void nio_load(char* path, nio_console* c)
{
/*	FILE* f = fopen(path,"rb");
	
	fread(&c->cursor_x,sizeof(int),1,f);
	fread(&c->cursor_y,sizeof(int),1,f);
	
	fread(&c->max_x,sizeof(int),1,f);
	fread(&c->max_y,sizeof(int),1,f);
	
	fread(&c->offset_x,sizeof(int),1,f);
	fread(&c->offset_y,sizeof(int),1,f);
	
	fread(&c->default_background_color,sizeof(char),1,f);
	fread(&c->default_foreground_color,sizeof(char),1,f);
	
	fread(&c->drawing_enabled,sizeof(BOOL),1,f);
	
	c->data = malloc(c->max_x*c->max_y);
	c->color = malloc(c->max_x*c->max_y);
	
	fread(c->data,sizeof(char),c->max_x*c->max_y,f);
	fread(c->color,sizeof(char),c->max_x*c->max_y,f);
	
	fclose(f);*/
}

void nio_save(char* path, nio_console* c)
{
/*	FILE* f = fopen(path,"wb");
	
	fwrite(&c->cursor_x,sizeof(int),1,f);
	fwrite(&c->cursor_y,sizeof(int),1,f);
	
	fwrite(&c->max_x,sizeof(int),1,f);
	fwrite(&c->max_y,sizeof(int),1,f);
	
	fwrite(&c->offset_x,sizeof(int),1,f);
	fwrite(&c->offset_y,sizeof(int),1,f);
	
	fwrite(&c->default_background_color,sizeof(char),1,f);
	fwrite(&c->default_foreground_color,sizeof(char),1,f);
	
	fwrite(&c->drawing_enabled,sizeof(BOOL),1,f);
	
	fwrite(c->data,sizeof(char),c->max_x*c->max_y,f);
	fwrite(c->color,sizeof(char),c->max_x*c->max_y,f);
	
	fclose(f);*/
}

BOOL shift = FALSE;
BOOL caps = FALSE;
BOOL ctrl = FALSE;
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
char nio_getch(void)
{
	int key = 0;
	while(1)
	{
		keyupdate();

		// Ctrl, Shift, Caps first
		if(isKeyPressed(KEY_CTRL_VARS))
		{
			if(ctrl) ctrl = FALSE;
			else ctrl = TRUE;
		}
		if(isKeyPressed(KEY_CTRL_OPTN))
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

		if(isKeyPressed(KEY_CTRL_MENU)) return 0;
		if(isKeyPressed(KEY_CTRL_EXIT)) return 0;
		if(isKeyPressed(KEY_CTRL_QUIT)) return 0;
		
		// Characters
		if(isKeyPressed(KEY_CHAR_A)) return shiftKey('a','A');
		if(isKeyPressed(KEY_CHAR_B)) return shiftKey('b','B');
		if(isKeyPressed(KEY_CHAR_C)) return shiftKey('c','C');
		if(isKeyPressed(KEY_CHAR_D)) return shiftKey('d','D');
		if(isKeyPressed(KEY_CHAR_E)) return shiftKey('e','E');
		if(isKeyPressed(KEY_CHAR_F)) return shiftKey('f','F');
		if(isKeyPressed(KEY_CHAR_G)) return shiftKey('g','G');
		if(isKeyPressed(KEY_CHAR_H)) return shiftKey('h','H');
		if(isKeyPressed(KEY_CHAR_I)) return shiftKey('i','I');
		if(isKeyPressed(KEY_CHAR_J)) return shiftKey('j','J');
		if(isKeyPressed(KEY_CHAR_K)) return shiftKey('k','K');
		if(isKeyPressed(KEY_CHAR_L)) return shiftKey('l','L');
		if(isKeyPressed(KEY_CHAR_M)) return shiftKey('m','M');
		if(isKeyPressed(KEY_CHAR_N)) return shiftKey('n','N');
		if(isKeyPressed(KEY_CHAR_O)) return shiftKey('o','O');
		if(isKeyPressed(KEY_CHAR_P)) return shiftKey('p','P');
		if(isKeyPressed(KEY_CHAR_Q)) return shiftKey('q','Q');
		if(isKeyPressed(KEY_CHAR_R)) return shiftKey('r','R');
		if(isKeyPressed(KEY_CHAR_S)) return shiftKey('s','S');
		if(isKeyPressed(KEY_CHAR_T)) return shiftKey('t','T');
		if(isKeyPressed(KEY_CHAR_U)) return shiftKey('u','U');
		if(isKeyPressed(KEY_CHAR_V)) return shiftKey('v','V');
		if(isKeyPressed(KEY_CHAR_W)) return shiftKey('w','W');
		if(isKeyPressed(KEY_CHAR_X)) return shiftKey('x','X');
		if(isKeyPressed(KEY_CHAR_Y)) return shiftKey('y','Y');
		if(isKeyPressed(KEY_CHAR_Z)) return shiftKey('z','Z');
		
		// Numbers
		if(isKeyPressed(KEY_CHAR_0)) return '0';
		if(isKeyPressed(KEY_CHAR_1)) return '1';
		if(isKeyPressed(KEY_CHAR_2)) return '2';
		if(isKeyPressed(KEY_CHAR_3)) return '3';
		if(isKeyPressed(KEY_CHAR_4)) return '4';
		if(isKeyPressed(KEY_CHAR_5)) return '5';
		if(isKeyPressed(KEY_CHAR_6)) return '6';
		if(isKeyPressed(KEY_CHAR_7)) return '7';
		if(isKeyPressed(KEY_CHAR_8)) return '8';
		if(isKeyPressed(KEY_CHAR_9)) return '9';
		
		// Symbols
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
		if(isKeyPressed(KEY_CHAR_SQUARE))		return '²';
		
		// Special chars
		if(isKeyPressed(KEY_CHAR_CR))		return '\n';
		if(isKeyPressed(KEY_CTRL_DEL))		return '\b';
		if(isKeyPressed(KEY_CHAR_STORE))		return '\t';

	}
	return 0;
}

void nio_InitConsole(nio_console* c, int size_x, int size_y, int offset_x, int offset_y, char background_color, char foreground_color)
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
	c->color = malloc(c->max_x*c->max_y);
	nio_Clear(c);
}

void nio_DrawConsole(nio_console* c)
{
	int row, col;
	for(row = 0; row < c->max_y; row++)
	{
		for(col = 0; col < c->max_x; col++)
		{
			nio_DrawChar(c,col,row);
		}
	}
}

void nio_Clear(nio_console* c)
{
	char color = (c->default_background_color << 4) | c->default_foreground_color;
	memset(c->data,0,c->max_x*c->max_y);
	memset(c->color,color,c->max_x*c->max_y);
	c->cursor_x = 0;
	c->cursor_y = 0;
	if(c->drawing_enabled)
		nio_DrawConsole(c);
}

void nio_ScrollDown(nio_console* c)
{
	char* temp;
	temp = malloc(c->max_x*c->max_y);
	
	memset(temp,0,c->max_x*c->max_y);
	memcpy(temp,c->data+c->max_x,c->max_x*(c->max_y-1));
	memcpy(c->data,temp,c->max_x*c->max_y);
	
	char color = (c->default_background_color << 4) | c->default_foreground_color;
	memset(temp,color,c->max_x*c->max_y);
	memcpy(temp,c->color+c->max_x,c->max_x*(c->max_y-1));
	memcpy(c->color,temp,c->max_x*c->max_y);
	
	if(c->cursor_y > 0)
		c->cursor_y--;
	c->cursor_x = 0;
	
	free(temp);
}

void nio_DrawChar(nio_console* c, int pos_x, int pos_y)
{
	char ch = c->data[pos_y*c->max_x+pos_x];
	char color = c->color[pos_y*c->max_x+pos_x];
	
	char background_color = (color & 0xF0) >> 4;
	char foreground_color = color & 0x0F;
	
	nio_drawch(c->offset_x, c->offset_y, pos_x, pos_y, ch == 0 ? ' ' : ch, background_color, foreground_color);
}

void nio_SetChar(nio_console* c, char ch, int pos_x, int pos_y)
{
	char color = (c->default_background_color << 4) | c->default_foreground_color;
	
	c->data[pos_y*c->max_x+pos_x] = ch;
	c->color[pos_y*c->max_x+pos_x] = color;
}

void nio_PrintChar(nio_console* c, char ch)
{
	// Newline. Increment Y cursor, set X cursor to zero. Scroll if necessary.
	if(ch == '\n')
	{
		c->cursor_x = 0;
		c->cursor_y++;
		// Scrolling necessary?
		if(c->cursor_y >= c->max_y)
		{
			nio_ScrollDown(c);
			if(c->drawing_enabled)
				nio_DrawConsole(c);
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
			nio_ScrollDown(c);
			if(c->drawing_enabled)
				nio_DrawConsole(c);
		}
		// Then store it.
		nio_SetChar(c,ch,c->cursor_x,c->cursor_y);
		
		// Draw it when BOOL draw is true
		if(c->drawing_enabled) nio_DrawChar(c,c->cursor_x,c->cursor_y);
		
		// Increment X cursor. It will be checked for validity next time.
		c->cursor_x++;
	}
}

void nio_PrintStr(nio_console* c, char* str)
{
	int len = strlen(str);
	int i;
	for(i = 0; i < len; i++)
	{
		nio_PrintChar(c, str[i]);
	}
}

void nio_printf(nio_console* c, char *format, ...)
{
	char buf[1000];
	memset(buf,'\0',sizeof(buf));
	__builtin_va_list arglist;
	__builtin_va_start(arglist,format);
	vsprintf(buf,format,*(char **)&arglist);
	nio_PrintStr(c,buf);
	__builtin_va_end(arglist);
}

void nio_SetColor(nio_console* c, char background_color, char foreground_color)
{
	c->default_background_color = background_color;
	c->default_foreground_color = foreground_color;
}

void nio_EnableDrawing(nio_console* c, BOOL enable_drawing)
{
	c->drawing_enabled = enable_drawing;
}

char nio_GetChar(nio_console* c)
{
	char ch = nio_getch();
	nio_PrintChar(c,ch);
	return ch;
}

int nio_GetStr(nio_console* c, char* str)
{
	char tmp;
	int old_x = c->cursor_x;
	int old_y = c->cursor_y;
	
	int i = 0;
	while(1)
	{
		//wait_no_key_pressed();
		tmp = nio_getch();
		if(tmp == '\n')
		{
			str[i] = '\0';
			nio_PrintChar(c,'\n');
			return i > 0 ? 1 : 0;
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
				nio_PrintStr(c,"\b \b");
				i--;
			}
		}
		else if(tmp == '\0')
		{
			str[0] = '\0';
			nio_PrintChar(c,'\n');
			return 0;
		}
		else
		{
			str[i] = tmp;
			nio_PrintChar(c,tmp);
			i++;
		}
	}
}

void nio_CleanUp(nio_console* c)
{
	free(c->data);
	free(c->color);
}
