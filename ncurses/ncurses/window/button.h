/*
 * =====================================================================================
 *
 *	Filename:  button.h
 *
 * 	Description:  the file of button header
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

#ifndef _BUTTON_H_2012_
#define _BUTTON_H_2012_

#include "win.h"

typedef struct _BUTTON
{
	ALL_HAVE_ATRRIBUTE
	bool focus;
	char *value;
}BUTTON;

BUTTON *create_button(BUTTON *button, void *parent, char *value, int x, int y);
int show_button(BUTTON *button);
int delete_button(BUTTON *button);
int button_driver(BUTTON *button, int msg);

#endif /*_BUTTON_H_2012_*/

