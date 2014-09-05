/*
 * =====================================================================================
 *
 *	Filename:  list.c
 *
 * 	Description:  list c file
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

#include "list.h"

#if 1


/*
 * Print list item
 * show_id start from 0
 */
static void print_item (LIST *list, int show_id)
{
	int i, x0, y0, selected, id;
	
	x0 = offset_x(list)+1;
	y0 = offset_y(list)+show_id+1;
	
	selected = (list->select_show_id == show_id+1);
	id = show_id+list->up_hide_num;
	
	/* Clear 'residue' of last item */
	wattrset (MAIN_SCREEN, A_NORMAL);
	wmove (MAIN_SCREEN,y0, x0);
	for (i = 2; i < list->w; i++)
		waddch (MAIN_SCREEN, ' ');

	wmove (MAIN_SCREEN, y0, x0+3);
	wattrset (MAIN_SCREEN, selected ? A_REVERSE : A_BOLD);
	if (list->type == FLAG_CHECK)
		wprintw (MAIN_SCREEN, "[%c]", list->status[id] ? 'X' : ' ');
	else
		wprintw (MAIN_SCREEN, "(%c)", list->status[id] ? '*' : ' ');

	wattrset (MAIN_SCREEN, selected ? A_REVERSE : A_BOLD);
	mvwaddch(MAIN_SCREEN, y0, x0+7, list->items[id][0]);
	wattrset (MAIN_SCREEN, selected ? A_REVERSE : A_BOLD);
	waddstr (MAIN_SCREEN, (char *)(list->items[id])+1);
	if (selected) 
	{
		wmove (MAIN_SCREEN, y0, x0+4);
		wrefresh (MAIN_SCREEN);
	}
}


/*
 * Print the scroll indicators.
 */
static void print_arrows (LIST *list)
{
	int x0, y0;

	x0 = offset_x(list)+ list->w/2 -2;
	y0 = offset_y(list);
	
	wmove(MAIN_SCREEN, y0, x0);
	if (list->up_hide_num > 0)
	{
		wattrset (MAIN_SCREEN, A_BOLD);
		waddstr (MAIN_SCREEN, "(^)");
	}
	else 
	{
		wattrset (MAIN_SCREEN, A_NORMAL);
		waddch (MAIN_SCREEN, ACS_HLINE);
		waddch (MAIN_SCREEN, ACS_HLINE);
		waddch (MAIN_SCREEN, ACS_HLINE);
    	}

	y0 += list->h-1 ;
	wmove(MAIN_SCREEN, y0, x0);

	if (list->up_hide_num + list->h-2 < list->item_no )
	{
		wattrset (MAIN_SCREEN, A_BOLD);
		waddstr (MAIN_SCREEN, "(v)");
	}
	else 
	{
		wattrset (MAIN_SCREEN, A_NORMAL);
		waddch (MAIN_SCREEN, ACS_HLINE);
		waddch (MAIN_SCREEN, ACS_HLINE);
		waddch (MAIN_SCREEN, ACS_HLINE);
	}
}
#endif


LIST *create_list(LIST *list, void *parent, char **items, int item_no)
{
	int i, len;
	list->parent = (AREAR *)parent;
	list->item_no = item_no;
	list->select_id = 1;
	list->select_show_id = 1;
	list->up_hide_num= 0;
	list->items = (char **)calloc(item_no, sizeof(char *));
	for (i=0; i<item_no; i++)
	{
		list->items[i] = strdup(items[i]);
		list->status = (int *)calloc(item_no, sizeof(int));
	}
	draw_boder(list);
	
	return list;
}


int show_list(LIST *list)
{
	int i, max_choice, list_height;

	list_height = list->h-2 ;
	max_choice = MIN(list_height, list->item_no);

	if (list->select_id > max_choice)
		list->up_hide_num = list->select_id-max_choice;
	else
		list->up_hide_num = 0;
	
	for (i = 0; i < max_choice; i++) 
		print_item (list, i);
	 
	print_arrows(list);
	
	return 0;
}


int delete_list(LIST *list)
{
	int i;
	for (i=0; i<list->item_no; i++)
		safe_free(list->items[i]);
	safe_free(list->items);
	safe_free(list->status);
	return 0;
}



static int set_check(LIST *list)
{
	if (list->type==FLAG_CHECK)
		list->status[list->select_id-1] = !(list->status[list->select_id-1]);
	else
	{
		memset(list->status, 0x00, list->item_no*sizeof(int));
		list->status[list->select_id-1]  = 1;
	}
}


int list_driver(LIST *list, int msg)
{
	int max_choice, list_height;
	
	if (!list->focus)
		return RET_ERROR;

	list_height = list->h-2 ;
	max_choice = MIN(list_height, list->item_no);

	if (msg == KEY_UP || msg == KEY_DOWN || msg == VK_SPACE)
	{
		switch (msg)
		{
			case KEY_UP:
				if (list->select_id>1)
					list->select_id--;
				if (list->up_hide_num == 0 && list->select_show_id>1)
					list->select_show_id--;
				break;
			case KEY_DOWN:
				if (list->select_id<list->item_no)
					list->select_id++;
				if (list->select_show_id<max_choice)
					list->select_show_id++;
				break;
			case VK_SPACE:
				set_check(list);
				break;
		}
		show_list(list);
	}
	return RET_OK;
}

