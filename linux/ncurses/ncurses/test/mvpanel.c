#include <panel.h>

int main()
{
	WINDOW *my_wins, *old_wins;
	PANEL *my_panels; 
	int lines =10, cols=40, y=10, x=40,i;
	int top = 0, ch;

	initscr();
	cbreak();
	noecho();
	keypad(stdscr, TRUE);

	my_wins = newwin(lines,cols, y, x);

	box(my_wins, 0, 0);
	mvwprintw(my_wins, 1, 1, "fang%d",5);

	
	my_panels = new_panel(my_wins);	
	
	update_panels();
	doupdate();
	
	while ((ch=getch())!=KEY_F(1))
	{
		switch (ch)
		{
		case KEY_UP:
			y--;
			break;
		case KEY_DOWN:
			y++;
			break;
		case KEY_LEFT:
			x--;
			break;
		case KEY_RIGHT:
			x++;
			break;
		}	
		old_wins=panel_window(my_panels);
		my_wins = newwin(lines,cols, y, x);
		box(my_wins, 0, 0);
		mvwprintw(my_wins, 1, 1, "fang%d",5);
		replace_panel(my_panels, my_wins);
		delwin(old_wins);
		update_panels();
		doupdate();
	}

	endwin();
}
