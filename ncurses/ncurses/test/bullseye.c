#include </usr/include/ncurses/curses.h>

static void finish(int sig);

int main(int argc, char **argv)
{
	initscr();
	keypad(stdscr, TRUE);
	nonl();
	cbreak();
	//noecho();
	
	move(LINES/2+1, COLS-4);
	addstr("eye");
	refresh();
	sleep(2);
	move(LINES/2-3,COLS/2-3);
	addstr("bulls");
	refresh();
	sleep(5);
	finish(0);
	return 0;
}

static void finish(int sig)
{
	endwin();
	exit(0);
}
