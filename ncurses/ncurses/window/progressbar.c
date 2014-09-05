/*
 * =====================================================================================
 *
 *	Filename:  progressbar.c
 *
 * 	Description:  progressbar c file
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


#include <fcntl.h>
#include "progressbar.h"

#define PROGRESS_BAR_HEIGHT 2

//#define ONLY_TYPE
#ifdef ONLY_TYPE

int flag = 1;

void eixt_handler(int sig) 
{ 
    flag = 0; 
} 


PROGRESSBAR *create_progressbar(PROGRESSBAR* bar, void *parent)
{
	struct sigaction act, oldact; 

	bar->h = PROGRESS_BAR_HEIGHT;
	bar->parent = (AREAR *)parent;
	
	act.sa_handler = eixt_handler; 
	sigaddset(&act.sa_mask, SIGUSR1);
	act.sa_flags = SA_RESETHAND | SA_NODEFER;

	sigaction(SIGUSR1, &act, &oldact); 
	
	return bar;
}


int show_progressbar(PROGRESSBAR* bar)
{
	int i=0, x0, y0;

	x0 = offset_x(bar);
	y0 = offset_y(bar);
	//draw_boder(bar);
	wmove(MAIN_SCREEN, y0+1,  x0);
	set_cusoroff();
	 while (i++<bar->w-1)
	{
		wattrset(MAIN_SCREEN, A_REVERSE); 
	    	waddch(MAIN_SCREEN, ' ');
		wrefresh(MAIN_SCREEN);
		if (flag)
			usleep(1000*bar->speed);
		else
			usleep(10000);
	}
	 set_cusoron();
	 
	return flag ? RET_TIMEOUT : RET_OK;
}


int delete_progressbar(PROGRESSBAR* bar)
{
	return 0;
}

#else

#include "xtimer.h"

PROGRESSBAR *create_progressbar(PROGRESSBAR* bar, void *parent)
{
	bar->h = PROGRESS_BAR_HEIGHT;
	bar->parent = (AREAR *)parent;
	bar->pos = 0;
	init_mtimer(0);

	return bar;
}


static int timer_proc(void *param)
{
	PROGRESSBAR* bar = (PROGRESSBAR*)param;
	int x0, y0;
 
	x0 = offset_x(bar);
	y0 = offset_y(bar);
	
	wattrset(MAIN_SCREEN, A_REVERSE); 
	if (bar->pos++<bar->w)
	{
		mvwaddch(MAIN_SCREEN, y0, x0+bar->pos, ' ');
		update();
	}
	else
	{
		kill_timer(bar->timer_id);
	}
}


int show_progressbar(PROGRESSBAR* bar)
{
	bar->timer_id = start_timer(bar->speed, timer_proc, bar);
	return 0;
}


int delete_progressbar(PROGRESSBAR* bar)
{
	 delete_mtimer();
	return 0;
}
#endif

