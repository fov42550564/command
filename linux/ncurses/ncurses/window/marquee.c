/*
 * =====================================================================================
 *
 *	Filename:  marquee.c
 *
 * 	Description:  marquee c file
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
#include "marquee.h"

static pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

MARQUEE *create_marquee(MARQUEE* marquee, void *parent, char *value)
{
	int len = strlen(value);
	
	marquee->parent = (AREAR *)parent;
	marquee->line = (MARQUEE_LINE*)calloc(1, sizeof(MARQUEE_LINE));
	marquee->line->len = len+2*(marquee->w-2);
	marquee->line->chars = (char*)calloc(marquee->line->len, sizeof(char));
	marquee->show_chars = (char*)calloc(marquee->w-1, sizeof(char));
	memset(marquee->line->chars, ' ', marquee->line->len);
	memcpy(marquee->line->chars+marquee->w-2, value, len);
	marquee->line->pos = 0;
	marquee->enable = 1;
	draw_boder(marquee);
	return marquee;
}


int delete_marquee(MARQUEE* marquee)
{
	marquee->enable = 0;
	pthread_join(marquee->tid, NULL);
	safe_free(marquee->line->chars);
	safe_free(marquee->line);
	safe_free(marquee->show_chars);
}


int update_marquee_show_chars(MARQUEE *marquee)
{
	int i, j=0;
	char *tmp;
	if (marquee->line->pos>=marquee->line->len-marquee->w+2)
		marquee->line->pos = 0;
	memcpy(marquee->show_chars, marquee->line->chars+marquee->line->pos, marquee->w-2);
}


void* marquee_callback(void* arg)
{
	int ret,x, y;
	MARQUEE* marquee = (MARQUEE*)arg;
	move_to(marquee,1,1);

	while (marquee->enable)
	{	
		update_marquee_show_chars(marquee);
		ret=pthread_mutex_trylock(&mutex);
		if (EBUSY != ret)
		{
			pthread_mutex_lock(&mutex);
			getyx(MAIN_SCREEN, y, x);
			wattrset(MAIN_SCREEN, 0);
			mvbaddstr(marquee, 1, 1, marquee->show_chars);
			wmove(MAIN_SCREEN, y, x);
			marquee->line->pos++;
			wrefresh(MAIN_SCREEN);
			pthread_mutex_unlock(&mutex);
		}
		usleep(marquee->speed*1000);
	}
}


int show_marquee(MARQUEE* marquee)
{
	if (pthread_create(&marquee->tid, NULL, marquee_callback, marquee)!=0)
		return -1;
	return 0;
}







