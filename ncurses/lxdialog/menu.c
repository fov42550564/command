/*
 * 	file:snake.c
 *	discription:show a snake game
 * author: fangyunjiang
 * date:2011-12-24
 */

#include "dialog.h"
#include <menu.h>

char *options[] = {
                        "Choice 1",
                        "Choice 2",
                        "Choice 3",
                        "Choice 4",
			   "Choice 5",
			   "Choice 6",
			   "Choice 7",
			   "Choice 8",
			   "Choice 9",
			   "Choice 10",
			   (char *)NULL,
                       };

typedef struct _LXMENU
{
	int h;
	int w;
	int y;
	int x;

	int rows;
	int cols;

	char *mark;
	
	WINDOW *parwin;
	ITEM ** items;
	MENU *menu;
	int items_cnt;
	
}LXMENU, *PLXMENU;

/*
 * create a menu
 * note: options end with null string
 *
 */
void create_lxmenu(LXMENU *lxmenu, char **options)
{
	int i;

	if (0==lxmenu->h)
		lxmenu->h=lxmenu->rows+1;
	
	for (lxmenu->items_cnt=0; options[lxmenu->items_cnt]; lxmenu->items_cnt++);
	lxmenu->items = (ITEM **)calloc(lxmenu->items_cnt+1, sizeof(ITEM *));
	for (i=0; i<lxmenu->items_cnt; ++i)
               lxmenu->items[i] = new_item(options[i], "");
	lxmenu->items[i] = (ITEM *)NULL;
	lxmenu->menu = new_menu(lxmenu->items);
	
	set_menu_win(lxmenu->menu, lxmenu->parwin);
	set_menu_sub(lxmenu->menu, derwin(lxmenu->parwin, lxmenu->h,lxmenu->w,lxmenu->y,lxmenu->x));
	set_menu_format(lxmenu->menu, lxmenu->rows, lxmenu->cols);
	set_menu_mark(lxmenu->menu, lxmenu->mark);
	
	draw_box (lxmenu->parwin, lxmenu->y-1,lxmenu->x-1, lxmenu->h+1, \
		lxmenu->w, dialog_attr, border_attr);
	post_menu(lxmenu->menu);
}


/*
 * release a menu
 *
 */
void release_lxmenu(LXMENU *lxmenu)
{
	int i;
	
	unpost_menu(lxmenu->menu);
	free_menu(lxmenu->menu);
       for(i = 0; i < lxmenu->items_cnt; ++i)
		free_item(lxmenu->items[i]);
	 free(lxmenu->items);
}



/*
 * Display a progress
 * 
 */
int dialog_lxmenu ( int height, int width)
{
	int i,j, ret=1,  x, y, key = 0;
	int count, c;
	WINDOW *dialog;
	LXMENU lxmenu = {0, 20, 3, 3, 5, 1, " * "};
	 
	/* center dialog box on screen */
	x = (COLS - width) / 2;
	y = (LINES - height) / 2;
	    
	dialog = newwin (height, width, y, x);
	keypad (dialog, TRUE);

	lxmenu.parwin = dialog;
	create_lxmenu(&lxmenu, options);
	
	draw_box (dialog, 0, 0, height, width, dialog_attr, border_attr);
	wrefresh(dialog);
	while((c = wgetch(dialog)) != KEY_F(1))
	{       switch(c)
	        {	case KEY_DOWN:
				menu_driver(lxmenu.menu, REQ_DOWN_ITEM);
				break;
			case KEY_UP:
				menu_driver(lxmenu.menu, REQ_UP_ITEM);
				break;
		}
                wrefresh(dialog);
	}

	release_lxmenu(&lxmenu);
	
	delwin (dialog);
	endwin();
    return ret;
}

