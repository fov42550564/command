/*
 * =====================================================================================
 *
 *	Filename:  common.c
 *
 * 	Description:  common c file
 *
 *	Version:  1.0
 *	Created:  12/28/11 20:32:17
 *	Revision:  none
 * 	Compiler:  gcc
 *
 *	Author:  fangyunjiang (fang), 42550564@qq.com
 *	Company:  star-net
 *
 * =====================================================================================
 */

#include "common.h"


int client_x_to_screen(/*AREAR*/void *ptr)
{
	int x = 0;
	AREAR *arear = (AREAR *)ptr;
	
	while (arear != NULL)
	{
		x += arear->x;
		arear = arear->parent;
	}
	return x;
}



int client_y_to_screen(/*AREAR*/void *ptr)
{
	int y = 0;
	AREAR *arear = (AREAR *)ptr;
	
	while (arear != NULL)
	{
		y += arear->y;
		arear = arear->parent;
	}
	return y;
}


int move_to(/*AREAR*/void *ptr, int x, int y)
{
	int  x0, y0;
	x0 = offset_x(ptr)+x;
	y0 = offset_y(ptr)+y;
	
	wmove(MAIN_SCREEN, y0, x0);
}


int draw_box(/*AREAR*/void *ptr, int x, int y, int w, int h)
{
	int i, j, x0, y0;
	x0 = offset_x(ptr)+x;
	y0 = offset_y(ptr)+y;
	
	wattrset(MAIN_SCREEN, 0);
	for (i=0; i<h; i++) 
	{
		for (j=0; j<w; j++)
		{
	    if (!i && !j)
				mvwaddch(MAIN_SCREEN, y0+i, x0+j, ACS_ULCORNER);
	    else if (i == h - 1 && !j)
				mvwaddch(MAIN_SCREEN, y0+i, x0+j, ACS_LLCORNER);
	    else if (!i && j == w - 1)
				mvwaddch(MAIN_SCREEN, y0+i, x0+j, ACS_URCORNER);
	    else if (i == h - 1 && j == w - 1)
				mvwaddch(MAIN_SCREEN, y0+i, x0+j, ACS_LRCORNER);
	    else if (!i)
				mvwaddch(MAIN_SCREEN, y0+i, x0+j, ACS_HLINE);
	    else if (i == h - 1)
				mvwaddch(MAIN_SCREEN, y0+i, x0+j, ACS_HLINE);
	    else if (!j)
				mvwaddch(MAIN_SCREEN, y0+i, x0+j, ACS_VLINE);
	    else if (j == w - 1)
				mvwaddch(MAIN_SCREEN, y0+i, x0+j, ACS_VLINE);
		}
	}
	
	return 0;
}


int cover_box(/*AREAR*/void *ptr, int x, int y, int w, int h)
{
	int i, j, x0, y0;
	x0 = offset_x(ptr)+x;
	y0 = offset_y(ptr)+y;
	
	wattrset(MAIN_SCREEN, 0);
	for (i=0; i<h; i++) 
	{
		wmove(MAIN_SCREEN, y0 + i, x0);
		for (j=0; j<w; j++)
		{
	    if (!i && !j)
				waddch(MAIN_SCREEN, ACS_ULCORNER);
	    else if (i == h - 1 && !j)
				waddch(MAIN_SCREEN, ACS_LLCORNER);
	    else if (!i && j == w - 1)
				waddch(MAIN_SCREEN, ACS_URCORNER);
	    else if (i == h - 1 && j == w - 1)
				waddch(MAIN_SCREEN, ACS_LRCORNER);
	    else if (!i)
				waddch(MAIN_SCREEN, ACS_HLINE);
	    else if (i == h - 1)
				waddch(MAIN_SCREEN, ACS_HLINE);
	    else if (!j)
				waddch(MAIN_SCREEN, ACS_VLINE);
	    else if (j == w - 1)
				waddch(MAIN_SCREEN, ACS_VLINE);
			else
				waddch(MAIN_SCREEN, ' ');
		}
	}
	return 0;
}



int clear_box(/*AREAR*/void *ptr, int x, int y, int w, int h)
{
	int i, j, x0, y0;
	x0 = offset_x(ptr)+x;
	y0 = offset_y(ptr)+y;
	
	wattrset (MAIN_SCREEN, 0);
	for (i = 0; i < h; i++) 
	{
		wmove (MAIN_SCREEN, y0 + i, x0);
		for (j = 0; j < w; j++)
			waddch (MAIN_SCREEN,  ' ');
	}
	return 0;
}


int draw_boder(/*AREAR*/void *ptr)
{
	draw_box(ptr, 0, 0, ((AREAR *)ptr)->w,  ((AREAR *)ptr)->h);
	return 0;
}


int draw_hline(/*AREAR*/void *ptr, int x, int y, int len)
{
	int x0, y0, i;
	x0 = offset_x(ptr)+x;
	y0 = offset_y(ptr)+y;
	wmove(MAIN_SCREEN, y0, x0);

	for (i=0; i<len; i++)
		waddch (MAIN_SCREEN,  ACS_HLINE);
	return 0;
}

int draw_vline(/*AREAR*/void *ptr, int x, int y, int len)
{
	int x0, y0, i;
	x0 = offset_x(ptr)+x;
	y0 = offset_y(ptr)+y;
	wmove(MAIN_SCREEN, y0, x0);

	for (i=0; i<len; i++)
		waddch (MAIN_SCREEN,  ACS_VLINE);
	return 0;
}


int draw_up_start_obline(/*AREAR*/void *ptr, int x, int y, int len) /*\*/
{
	int x0, y0, i;
	x0 = offset_x(ptr)+x;
	y0 = offset_y(ptr)+y;
	wmove(MAIN_SCREEN, y0, x0);

	for (i=0; i<len; i++)
		waddch (MAIN_SCREEN,  0x5c);
	return 0;
}


int draw_down_start_obline(/*AREAR*/void *ptr, int x, int y, int len) /*/*/
{
	int x0, y0, i;
	x0 = offset_x(ptr)+x;
	y0 = offset_y(ptr)+y;
	wmove(MAIN_SCREEN, y0, x0);

	for (i=0; i<len; i++)
		waddch (MAIN_SCREEN,  0x2f);
	return 0;
}


int mvbaddch(/*AREAR*/void *ptr, int x, int y, int ch)
{
	int x0, y0;
	x0 = offset_x(ptr)+x;
	y0 = offset_y(ptr)+y;
	mvwaddch(MAIN_SCREEN, y0, x0, ch);
	return 0;
}


int mvbaddstr(/*AREAR*/void *ptr, int x, int y, char *str)
{
	int x0, y0;
	x0 = offset_x(ptr)+x;
	y0 = offset_y(ptr)+y;
	mvwaddstr(MAIN_SCREEN, y0, x0, str);
	return 0;
}

int mvbprintw(/*AREAR*/void *ptr, int x, int y, char *fmt, ...)
{
	int x0, y0;
	char dest[126];
	x0 = offset_x(ptr)+x;
	y0 = offset_y(ptr)+y;
	
	va_list vargs;
	va_start(vargs, fmt);
	vsprintf(dest, fmt, vargs);
	va_end(vargs);	
	mvwaddstr(MAIN_SCREEN, y0, x0, dest);
	return 0;
}



int update(void)
{
	wrefresh(MAIN_SCREEN);
	return 0;
}

int init_all(void)
{
	initscr();
	keypad(MAIN_SCREEN, TRUE);
	start_color();
	set_escdelay(25);
	cbreak();
	noecho();
	set_cusoroff();
}


int exit_all(void)
{
	endwin();
	set_cusoron();
}

