/*
 * =====================================================================================
 *
 *	Filename:  list.h
 *
 * 	Description:  the file of list header
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

#ifndef _LIST_H_2012_
#define _LIST_H_2012_

#include "win.h"

#define MAX_ITEM_COUNT 100
#define FLAG_CHECK 1
#define FLAG_RADIO 0


typedef struct _LIST
{
	ALL_HAVE_ATRRIBUTE
	int type;
	bool focus;

	int select_id;
	int select_show_id;
	int up_hide_num;
	
	int *status;

	int item_no;
	char **items;
}LIST, *PLIST;

LIST *create_list(LIST *list, void *parent, char **items, int item_no);
int show_list(LIST *list);
int delete_list(LIST *list);
int list_driver(LIST *list, int msg);

#endif /*_LIST_H_2012_*/

