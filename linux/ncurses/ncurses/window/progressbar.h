/*
 * =====================================================================================
 *
 *	Filename:  progressbar.h
 *
 * 	Description:  the progressbar of list header
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

#ifndef _PROGRESSBAR_H_2012_
#define _PROGRESSBAR_H_2012_

#include "win.h"

typedef struct _PROGRESSBAR
{
	ALL_HAVE_ATRRIBUTE
	int speed;
	int pos;
	int timer_id;
}PROGRESSBAR, *PPROGRESSBAR;

PROGRESSBAR *create_progressbar(PROGRESSBAR* bar, void *parent);
int show_progressbar(PROGRESSBAR* bar);
int delete_progressbar(PROGRESSBAR* bar);

#endif /*_PROGRESSBAR_H_2012_*/

