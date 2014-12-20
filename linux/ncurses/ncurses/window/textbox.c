/*
 * =====================================================================================
 *
 *	Filename:  textbox.c
 *
 * 	Description:  textbox c file
 *
 *	Version:  1.0
 *	Created:  12/28/11 23:57:08
 *	Revision:  none
 * 	Compiler:  gcc
 *
 *	Author:  fangyunjiang (fang), 42550564@qq.com
 *	Company:  star-net
 *
 * =====================================================================================
 */

#include "textbox.h"

#define TEXTBOX_HEIGHT 3

TEXTBOX *create_textbox(TEXTBOX *textbox, void *parent, char *value)
{
	int rows, cols, x0, y0;
	
	textbox->h = TEXTBOX_HEIGHT;
	FIELD *field[2] = {NULL};
	textbox->parent = (AREAR *)parent;
	x0 = offset_x(textbox);
	y0 = offset_y(textbox);
	
	textbox->field = field[0] = new_field(1, textbox->w-2, 0, 0, 0, 0);
	if (textbox->type == TEXTBOX_LINE_STYLE)
		set_field_back(field[0], A_UNDERLINE);	
	field_opts_off(field[0], O_AUTOSKIP);
	textbox->form = new_form(field);
	
	scale_form(textbox->form, &rows, &cols);
	set_form_win(textbox->form, MAIN_SCREEN);
	set_form_sub(textbox->form, derwin(MAIN_SCREEN, rows, cols, y0+1, x0+1));
	post_form(textbox->form);
	if (textbox->type == TEXTBOX_BOX_STYLE)
		draw_boder(textbox);

	if (value)
		set_textbox_value(textbox, value);

	return textbox;
}

int delete_textbox(TEXTBOX *textbox)
{
	unpost_form(textbox->form);
	//free_form(textbox->form);
	free_field(textbox->field);
	return 0;
}

int set_textbox_value(TEXTBOX *textbox, char *value)
{
	char *val = NULL;
	int len;
	
	len = strlen(value);
	if (textbox->w-2 >= len)
		set_field_buffer(textbox->field, 0, value);
	else
	{
		len = textbox->w-2;
		val = strdup(value);
		val[len] = '\0';
		set_field_buffer(textbox->field, 0, val);
		safe_free(val);
	}
	form_driver(textbox->form, REQ_END_LINE);
	return len;
}


int get_textbox_value(TEXTBOX *textbox, char *value, int len)
{
	int n;
	char *val;
	val = field_buffer(textbox->field, 0);
	n = strlen(val);
	if (len-1<n)
		n = len-1;
	strncpy(value, val, n);
	value[n] ='\0';
	return n;
}


int textbox_driver(TEXTBOX *textbox, int msg)
{
	if (!textbox->focus)
		return RET_ERROR;
	
	switch (msg)
	{
		case VK_BACKSPACE:
			form_driver(textbox->form, REQ_DEL_PREV);
			break;
		case KEY_LEFT:
			form_driver(textbox->form, REQ_PREV_CHAR);
			break;
		case KEY_RIGHT:
			form_driver(textbox->form, REQ_NEXT_CHAR);
			break;
		default:
			form_driver(textbox->form, msg);
			form_driver(textbox->form, REQ_VALIDATION); //update screen data to buffer data
			break;
	}
	return RET_OK;
}

