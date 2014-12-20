/*
 * =====================================================================================
 *
 *	Filename:  eprogressbar.c
 *
 * 	Description:  eprogressbar c file
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


#include<errno.h>
#include <pthread.h> 
#include "eprogressbar.h"

static pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

EPROGRESSBAR *create_eprogressbar(EPROGRESSBAR* eprogressbar, void *parent)
{
	eprogressbar->parent = (AREAR *)parent;
	draw_boder(eprogressbar);
	
	return eprogressbar;
}


int delete_eprogressbar(EPROGRESSBAR* eprogressbar)
{
	pthread_join(eprogressbar->tid, NULL);
}


int notice_eprogressbar_stop(EPROGRESSBAR* eprogressbar)
{
	eprogressbar->speed = 10;
	return 0;
}


void* eprogressbar_callback(void* arg)
{
	int i=1, ret;
	EPROGRESSBAR* eprogressbar = (EPROGRESSBAR*)arg;
	move_to(eprogressbar,1,1);

	while (i<eprogressbar->w-2)
	{	
		ret=pthread_mutex_trylock(&mutex);
		if (EBUSY != ret)
		{
			pthread_mutex_lock(&mutex);
			wattrset(MAIN_SCREEN, A_REVERSE); 
			mvbaddch(eprogressbar, i++, 1, ' ');
			wrefresh(MAIN_SCREEN);
			pthread_mutex_unlock(&mutex);
		}
		usleep(eprogressbar->speed*1000);
	}
}


int show_eprogressbar(EPROGRESSBAR* eprogressbar)
{
	if (pthread_create(&eprogressbar->tid, NULL, eprogressbar_callback, eprogressbar)!=0)
		return -1;
	return 0;
}


