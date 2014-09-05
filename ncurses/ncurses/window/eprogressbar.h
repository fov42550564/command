/*
 * =====================================================================================
 *
 *	Filename:  eprogressbar.h
 *
 * 	Description:  the eprogressbar of list header
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

#ifndef _EPROGRESSBAR_H_2012_
#define _EPROGRESSBAR_H_2012_

#include "win.h"

typedef struct _EPROGRESSBAR
{
	ALL_HAVE_ATRRIBUTE
	int speed;

	pthread_t tid;
}EPROGRESSBAR, *PEPROGRESSBAR;

EPROGRESSBAR *create_eprogressbar(EPROGRESSBAR* eprogressbar, void *parent);
int show_eprogressbar(EPROGRESSBAR* eprogressbar);
int notice_eprogressbar_stop(EPROGRESSBAR* eprogressbar);
int delete_eprogressbar(EPROGRESSBAR* eprogressbar);

#endif /*_EPROGRESSBAR_H_2012_*/

