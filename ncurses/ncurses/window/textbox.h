/*
 * =====================================================================================
 *
 *	Filename:  textbox.h
 *
 * 	Description:  the file of textbox header
 *
 *	Version:  1.0
 *	Created:  12/28/11 23:56:48
 *	Revision:  none
 * 	Compiler:  gcc
 *
 *	Author:  fangyunjiang (fang), 42550564@qq.com
 *	Company:  star-net
 *
 * =====================================================================================
 */

#ifndef _TEXTBOX_H_2012_
#define _TEXTBOX_H_2012_

#include <form.h>
#include "win.h"


#define TEXTBOX_LINE_STYLE 	0
#define TEXTBOX_BOX_STYLE 	1


typedef struct _TEXTBOX
{
	ALL_HAVE_ATRRIBUTE
	
	int type;
	bool focus;
	
	FIELD *field;
	FORM *form;
}TEXTBOX;

TEXTBOX *create_textbox(TEXTBOX *textbox, void *parent, char *value);
int delete_textbox(TEXTBOX *textbox);
int set_textbox_value(TEXTBOX *textbox, char *value);
int get_textbox_value(TEXTBOX *textbox, char *value, int len);

int textbox_driver(TEXTBOX *textbox, int msg);


#endif /*_TEXTBOX_H_2012_*/
