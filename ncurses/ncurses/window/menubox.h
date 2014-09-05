/*
 * =====================================================================================
 *
 *	Filename:  menubox.h
 *
 * 	Description:  the file of menubox header
 *
 *	Version:  1.0
 *	Created:  12/28/11 20:30:10
 *	Revision:  none
 * 	Compiler:  gcc
 *
 *	Author:  fangyunjiang (fang), 42550564@qq.com
 *	Company:  star-net
 *
 * =====================================================================================
 */

#ifndef _MENUBOX_H_2012_
#define _MENUBOX_H_2012_

#include "win.h"
#include <menu.h>

typedef struct _MENUBOX
{
	ALL_HAVE_ATRRIBUTE
	int rows;
	int cols;

	char *mark;
	int focus;
	MENU *menu;
	ITEM ** items;
	int items_count;
}MENUBOX, *PMENUBOX;


/*
 * create a menu
 * note: options end with null string
 *
 */
void create_menubox(MENUBOX *menubox, void *parent, char **options);
void delete_menubox(MENUBOX *menubox);
int menubox_driver(MENUBOX *menubox, int msg);
#endif /*_MENUBOX_H_2012_*/

