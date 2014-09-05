/*
 * =====================================================================================
 *
 *	Filename:  msgbox.c
 *
 * 	Description:  msgbox c file
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

#include "label.h"
#include "button.h"
#include "msgbox.h"


MSGBOX *create_msgbox(MSGBOX *msgbox, void *parent, char *info,  char *title)
{
	int x, y;
	
	msgbox->parent = (AREAR *)parent;
	y = msgbox->h -2;
	x = msgbox->w/2;
	
	create_label_ex(&(msgbox->label), msgbox, info, 2,  2, msgbox->w-4, msgbox->h-6);
	switch (msgbox->type)
	{
	case MB_OK:
		create_button(&(msgbox->button[0]), msgbox, "  OK  " , x-8, y);
		break;
	case MB_OKCANCEL:
		create_button(&(msgbox->button[0]), msgbox, "  OK  " , x-8, y);
		create_button(&(msgbox->button[1]), msgbox, "CANCEL" , x+2, y);
		break;
	case MB_YESNO:
		create_button(&(msgbox->button[0]), msgbox, " YES  " , x-8, y);
		create_button(&(msgbox->button[1]), msgbox, "  NO  " , x+2, y);
		break;
	}
	msgbox->button[0].focus = 1;
	msgbox->button[1].focus = 0;
	msgbox->info = strdup(info);
	msgbox->title = strdup(title);
	
	return msgbox;
}



int delete_msgbox(MSGBOX *msgbox)
{
	switch (msgbox->type)
	{
	case MB_OK:
		delete_button(&(msgbox->button[0]));
		break;
	case MB_OKCANCEL:
	case MB_YESNO:
		delete_button(&(msgbox->button[0]));
		delete_button(&(msgbox->button[1]));
		break;
	}
	delete_label(&(msgbox->label));
	safe_free(msgbox->info);
	safe_free(msgbox->title);
	return 0;
}


int show_msgbox(MSGBOX *msgbox)
{
	int x, y;
	x = offset_x(msgbox);
	y = offset_y(msgbox);
	draw_boder(msgbox);
	mvwaddstr(MAIN_SCREEN, y, x+2, msgbox->title);
	show_label(&(msgbox->label));
	show_button(&(msgbox->button[0]));
	if (msgbox->type == MB_OKCANCEL || msgbox->type == MB_YESNO)
		show_button(&(msgbox->button[1]));
	return 0;
}


int msgbox_driver(MSGBOX *msgbox, int msg)
{
	if (!msgbox->focus)
		return RET_ERROR;
	
	if (msgbox->type == MB_OK)
	{
		if (msg==VK_ENTER)
			return RET_YES;
	}
	else
	{
		switch (msg)
		{
			case KEY_LEFT:
			case KEY_RIGHT:
				REVERSE(msgbox->button[0].focus);
				REVERSE(msgbox->button[1].focus);
				show_button(&(msgbox->button[0]));
				show_button(&(msgbox->button[1]));
				break ;
			case VK_ENTER:
				return (msgbox->button[0].focus) ? RET_YES: RET_NO;
			default:
				return RET_ERROR;
		}
	}
	return RET_OK;
}


