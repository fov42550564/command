/*
 * =====================================================================================
 *
 *	Filename:  slider.h
 *
 * 	Description:  the slider of list header
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

#ifndef _SLIDER_H_2012_
#define _SLIDER_H_2012_

#include "win.h"

typedef struct _SLIDER
{
	ALL_HAVE_ATRRIBUTE
	int pos;
	int focus;
}SLIDER, *PSLIDER;

SLIDER *create_slider(SLIDER* slider, void *parent);
int set_pos(SLIDER* slider, int pos);
int get_pos(SLIDER* slider);
int slider_driver(SLIDER* slider, int msg);

#endif /*_SLIDER_H_2012_*/

