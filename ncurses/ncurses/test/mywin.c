#include </usr/include/ncurses/curses.h>



void draw_shadow(WINDOW * win, int y, int x, int height, int width)
{
    int i;

    if (has_colors ()) {	/* Whether terminal supports color? */
	wattrset (win, A_NORMAL);
	wmove (win, y + height, x + 2);
	for (i = 0; i < width; i++)
	    waddch (win, winch (win) & A_CHARTEXT);
	for (i = y + 1; i < y + height + 1; i++) {
	    wmove (win, i, x + width);
	    waddch (win, winch (win) & A_CHARTEXT);
	    waddch (win, winch (win) & A_CHARTEXT);
	}
	wnoutrefresh (win);
    }
}

void draw_box (WINDOW * win, int y, int x, int height, int width, chtype box, chtype border)
{
    int i, j;

    wattrset (win, 0);
    for (i = 0; i < height; i++) {
	wmove (win, y + i, x);
	for (j = 0; j < width; j++)
	    if (!i && !j)
		waddch (win, border | '+');
	    else if (i == height - 1 && !j)
		waddch (win, border | '+');
	    else if (!i && j == width - 1)
		waddch (win, box | '+');
	    else if (i == height - 1 && j == width - 1)
		waddch (win, box | '+');
	    else if (!i)
		waddch (win, border | '-');
	    else if (i == height - 1)
		waddch (win, box | '-');
	    else if (!j)
		waddch (win, border | '|');
	    else if (j == width - 1)
		waddch (win, box | '|');
	    else
		waddch (win, box | ' ');
    }
}




int main(int argc, char **argv)
{
	int i,x,y,w,h;
	WINDOW *win; 
	w=50;
	h=20;

	initscr();
	nonl();
	cbreak();
	noecho();
	
	fprintf(stderr, "COLS=%d,LINES=%d\n",COLS,LINES);
	x=(COLS-w)/2;
	y=(LINES-h)/2;
	//draw_shadow(stdscr,y,x,h,w);
	win=newwin(h,w,y,x);
	keypad(win, TRUE);
	draw_box(win,0,0,h,w,A_NORMAL,A_NORMAL);	

	wmove(win,0,10);
	waddstr(win,"eye");
	wrefresh(win);
	sleep(2);
	wmove(win,10,30);
	waddstr(win, "bulls");
	wrefresh(win);
	sleep(1);
	delwin(win);
	endwin();
	return 0;
}
