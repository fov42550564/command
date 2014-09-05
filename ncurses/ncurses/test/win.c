/*
 * =====================================================================================
 *
 *	Filename:  win.c
 *
 * 	Description:  
 *
 *	Version:  1.0
 *	Created:  12/23/11 21:28:01
 *	Revision:  none
 * 	Compiler:  gcc
 *
 *	Author:  fangyunjiang (fang), 42550564@qq.com
 *	Company:  star-net
 *
 * =====================================================================================
 */



#include <ncurses.h>


int main(int argc, char **argv)
{
	int x,y,w,h;
	WINDOW *win; 

	w=50;
	h=20;

	initscr();
	nonl();
	cbreak();

	noecho();
	
	x=(COLS-w)/2;
	y=(LINES-h)/2;

	win=newwin(h,w,y,x);
	keypad(win, TRUE);
	box(win, 0, 0);
	wrefresh(win);

	delwin(win);
	endwin();
	return 0;
}
