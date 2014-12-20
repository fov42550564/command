/*
 * =====================================================================================
 *
 *	Filename:  button.c
 *
 * 	Description:  button c file
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

#include "button.h"

BUTTON *create_button(BUTTON *button, void *parent, char *value, int x, int y)
{
    button->x = x;
    button->y = y;
    button->parent = (AREAR *)parent;
    button->value = strdup(value);
    button->focus = 0;
    return button;
}

int show_button(BUTTON *button)
{
	int i, temp, x0, y0;
	char *value;
	
	x0 = offset_x(button);
	y0 = offset_y(button);
	value = button->value;
	
	wmove(MAIN_SCREEN, y0, x0);
  wattrset(MAIN_SCREEN, button->focus ? A_REVERSE : A_DIM);
  waddstr (MAIN_SCREEN, "<");
  temp = strspn(value, " ");
  value += temp;
  wattrset(MAIN_SCREEN, button->focus ? A_REVERSE : A_NORMAL);
  for (i = 0; i < temp; i++)
	waddch(MAIN_SCREEN, ' ');
  wattrset(MAIN_SCREEN, button->focus ? A_REVERSE : A_BOLD);
  waddch(MAIN_SCREEN, value[0]);
  wattrset(MAIN_SCREEN, button->focus ? A_REVERSE : A_NORMAL);
  waddstr(MAIN_SCREEN, value + 1);
  wattrset(MAIN_SCREEN, button->focus ? A_REVERSE : A_DIM);
  waddstr(MAIN_SCREEN, ">");
  wmove(MAIN_SCREEN, y0, x0 + temp + 1);

  return 0;
}

int delete_button(BUTTON *button)
{
	safe_free(button->value);
	return 0;
}


int button_driver(BUTTON *button, int msg)
{
	if (!button->focus)
		return RET_ERROR;
	switch (msg)
	{
		case VK_ENTER:
			return RET_CLICK;
		default:
			return RET_ERROR;
	}
	return RET_OK;
}