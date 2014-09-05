/*
 * =====================================================================================
 *
 *	Filename:  win.h
 *
 * 	Description:  the file of win header
 *
 *	Version:  1.0
 *	Created:  12/28/11 19:58:00
 *	Revision:  none
 * 	Compiler:  gcc
 *
 *	Author:  fangyunjiang (fang), 42550564@qq.com
 *	Company:  star-net
 *
 * =====================================================================================
 */

#ifndef _WIN_H_2012_
#define _WIN_H_2012_

#include "common.h"

#define MAX_CHILD_COUNT 		10
#define WIN_TITLE_HEIGHT 		1
#define WIN_STATUS_HEIGHT 	1

typedef enum
{
	CT_WINDOW,
	CT_BUTTON,
	CT_TEXTBOX,
	CT_LIST,
	CT_MSGBOX,
	CT_PROGRESSBAR,
	CT_MAX ,
}CHILD_TYPE;


typedef struct _CHILD_COM
{
	CHILD_TYPE ctype;
	void *ptr;
}CHILD_COM, *PCHILD_COM;


typedef struct _WIN
{
	ALL_HAVE_ATRRIBUTE
	
	bool focus;
	int active_child_id;
	int last_child_id;
	
	char *status;
	CHILD_COM focus_childs[MAX_CHILD_COUNT];
}WIN, *PWIN;

typedef int (*WIN_CALL_PROC)(WIN *win, int msg, void *param);

WIN *create_win(WIN *win, void *parent, char *title);
WIN *create_win_ex(WIN *win, void *parent, int x, int y, int w, int h);

int win_driver(WIN *win, void *proc, void *param);

int set_focus(WIN *win, int id,  bool focus);
int add_focus_child(WIN *win, CHILD_TYPE ctype, void *ptr, int index);
int del_focus_child(WIN *win, int index);



#endif /*_WIN_H_2012_*/
