#include <panel.h>

int main()
{
	WINDOW *my_wins[3];
	PANEL *my_panels[3]; 
	int lines =10, cols=40, y=2, x=4,i;
	int top = 0, ch;

	initscr();
	cbreak();
	noecho();
	keypad(stdscr, TRUE);

	my_wins[0] = newwin(lines,cols, y, x);
	my_wins[1] = newwin(lines,cols, y+1, x+5);
	my_wins[2] = newwin(lines,cols, y+2, x+10);

	for (i=0; i<3; ++i)
	{
		box(my_wins[i], 0, 0);
		mvwprintw(my_wins[i], 1, 1, "fang%d",i);
	}
	
	my_panels[0] = new_panel(my_wins[0]);	
	my_panels[1] = new_panel(my_wins[1]);	
	my_panels[2] = new_panel(my_wins[2]);	
	
	update_panels();
	doupdate();
	
	while ((ch=getch())!=KEY_F(1))
	{
		switch (ch)
		{
		case 9:
			top++;
			if(top==3)top=0;
			//top_panel(my_panels[top]);
			for (i=0; i<3; i++)
			{
				if(top==i)
					show_panel(my_panels[i]);
				else
					hide_panel(my_panels[i]);
			}
			break;
		}	
		update_panels();
		doupdate();
	}

	endwin();
}
