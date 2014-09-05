/*
 * =====================================================================================
 *
 *	Filename:  menubox.c
 *
 * 	Description:  menubox c file
 *
 *	Version:  1.0
 *	Created:  12/28/11 20:30:30
 *	Revision:  none
 * 	Compiler:  gcc
 *
 *	Author:  fangyunjiang (fang), 42550564@qq.com
 *	Company:  star-net
 *
 * =====================================================================================
 */

#include "menubox.h"

/*
 * create a menu
 * note: options end with null string
 *
 */
void create_menubox(MENUBOX *menubox, void *parent, char **options)
{
	int i,x0,y0;

	menubox->parent = parent;
	x0 = offset_x(menubox);
	y0 = offset_y(menubox);
	
	if (0 == menubox->rows)
		menubox->rows=menubox->h;
	
	for (menubox->items_count=0; options[menubox->items_count]; menubox->items_count++);
	
	menubox->items = (ITEM **)calloc(menubox->items_count+1, sizeof(ITEM *));
	for (i=0; i<menubox->items_count; ++i)
               menubox->items[i] = new_item(options[i], "");
	menubox->items[i] = (ITEM *)NULL;
	menubox->menu = new_menu(menubox->items);
	
	set_menu_win(menubox->menu, MAIN_SCREEN);
	set_menu_sub(menubox->menu, derwin(MAIN_SCREEN, menubox->h-2,menubox->w-2,y0+1,x0+1));
	set_menu_format(menubox->menu, menubox->rows, menubox->cols);
	if (menubox->mark)
		set_menu_mark(menubox->menu, menubox->mark);
	else
		set_menu_mark(menubox->menu, "*");
	draw_boder(menubox);
	post_menu(menubox->menu);
}


void delete_menubox(MENUBOX *menubox)
{
	int i;
	
	unpost_menu(menubox->menu);
	free_menu(menubox->menu);
       for(i = 0; i < menubox->items_count; ++i)
		free_item(menubox->items[i]);
	 free(menubox->items);
}


int menubox_driver(MENUBOX *menubox, int msg)
{
	if (!menubox->focus)
		return RET_ERROR;
	
	switch (msg)
	{
		case KEY_DOWN:
			menu_driver(menubox->menu, REQ_DOWN_ITEM);
			break;
		case KEY_UP:
			menu_driver(menubox->menu, REQ_UP_ITEM);
			break;
		case KEY_LEFT:
			menu_driver(menubox->menu, REQ_LEFT_ITEM);
			break;
		case KEY_RIGHT:
			menu_driver(menubox->menu, REQ_RIGHT_ITEM);
			break;
		default:
			break;
	}
	return RET_OK;
}


