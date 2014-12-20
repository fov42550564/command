/*
 * =====================================================================================
 *
 *	Filename:  label.c
 *
 * 	Description:  label c file
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


LABEL *create_label(LABEL *label,  void *parent, char *value)
{
	int len, i;

	label->parent = (AREAR *)parent;
	len = strlen(value);
	label->lines = (len-1) / label->w + 1;
	label->value =  (char **)malloc(label->lines);
	for (i=0; i<label->lines; i++)
	{
		label->value[i] = (char *)calloc(label->w+1, sizeof(char));
		strncpy(label->value[i], value, label->w);
		value += label->w;
	}
	return label;
}


LABEL *create_label_ex(LABEL *label,   void *parent, char *value, int x, int y, int w, int h)
{
	int len, i;
	
	label->x = x;
	label->y = y;
	label->w = w;
	label->h = h;
	label->parent = (AREAR *)parent;
	
	len = strlen(value);
	label->lines = (len-1) / w + 1;
	label->value =  (char **)malloc(label->lines);
	for (i=0; i<label->lines; i++)
	{
		label->value[i] = (char *)calloc(w+1, sizeof(int));
		strncpy(label->value[i], value, w);
		value += w;
	}
	return label;
}


int create_simple_label( void *parent, char *value, int x, int y)
{
	mvwaddstr(MAIN_SCREEN, offset_y(parent)+y, offset_x(parent)+x, value);
	return 0;
}


int delete_label(LABEL *label)
{
	int i;
	for (i=0; i<label->lines; i++)
		safe_free(label->value[i]);
	safe_free(label->value);
	return 0;
}

int show_label(LABEL *label)
{
	int i, max_line, x0, y0;
	
	x0 = offset_x(label);
	y0 = offset_y(label);
	max_line = MIN(label->h, label->lines);
	
	for (i=0; i<max_line; i++)
	{
		mvwaddstr(MAIN_SCREEN, y0 + i, x0, label->value[i]);
	}
	return max_line;
}
