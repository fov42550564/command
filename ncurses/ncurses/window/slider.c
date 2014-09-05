/*
 * =====================================================================================
 *
 *	Filename:  slider.c
 *
 * 	Description:  slider c file
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


#include "slider.h"

static show_slider(SLIDER* slider, int pos)
{
	clear_box(slider, 1, 1, slider->w-2, slider->h-2);
	draw_hline(slider, 1, 1, slider->w-2);
	move_to(slider, pos, 1);
	waddch(MAIN_SCREEN, 'o');
}


SLIDER *create_slider(SLIDER* slider, void *parent)
{
	slider->parent = parent;
	slider->h = 3;
	draw_boder(slider);
	show_slider(slider, slider->pos);
}


int set_pos(SLIDER* slider, int pos)
{
	slider->pos = pos;
	show_slider(slider, slider->pos);
	update();
}


int get_pos(SLIDER* slider)
{
	return slider->pos;
}

int slider_driver(SLIDER* slider, int msg)
{
	if (!slider->focus)
		return RET_ERROR;
	switch (msg)
	{
		case KEY_LEFT:
			if (slider->pos>1)
			{
				show_slider(slider, --slider->pos);
				update();
			}
			break;
		case KEY_RIGHT:
			if (slider->pos<slider->w-2)
			{
				show_slider(slider, ++slider->pos);
				update();
			}
			break;
		default:
			return RET_ERROR;
	}
	return RET_OK;
}


