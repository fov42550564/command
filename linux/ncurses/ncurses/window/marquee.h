/*
 * =====================================================================================
 *
 *	Filename:  marquee.h
 *
 * 	Description:  the marquee of list header
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

#ifndef _MARQUEE_H_2012_
#define _MARQUEE_H_2012_

#include "win.h"

typedef CHARS_LINE MARQUEE_LINE;

typedef struct _MARQUEE
{
	ALL_HAVE_ATRRIBUTE
	int speed;
	int type;
	int enable;

	MARQUEE_LINE *line;
	char *show_chars;
	pthread_t tid;
}MARQUEE, *PMARQUEE;

MARQUEE *create_marquee(MARQUEE* marquee, void *parent, char *value);
int delete_marquee(MARQUEE* marquee);
int show_marquee(MARQUEE* marquee);

#endif /*_MARQUEE_H_2012_*/

