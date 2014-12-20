/*
 * =====================================================================================
 *
 *	Filename:  msgbox.h
 *
 * 	Description:  the file of msgbox header
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

#ifndef _MSGBOX_H_2012_
#define _MSGBOX_H_2012_

#include "win.h"


#define MB_OK  			0
#define MB_OKCANCEL  	1
#define MB_YESNO		2

typedef struct _MSGBOX 
{
	ALL_HAVE_ATRRIBUTE
	int type;
	int focus;
	int select_id;
	
	char *info;
	char *title;

	LABEL label;
	BUTTON button[2];
}MSGBOX, * PMSGBOX;

MSGBOX *create_msgbox(MSGBOX *msgbox, void *parent, char *info,  char *title);
int delete_msgbox(MSGBOX *msgbox);
int show_msgbox(MSGBOX *msgbox);
int msgbox_driver(MSGBOX *msgbox, int msg);


#endif /*_MSGBOX_H_2012_*/

