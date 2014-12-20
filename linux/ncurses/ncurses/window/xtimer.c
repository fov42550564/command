/*
 * =====================================================================================
 *
 *	Filename:  xtimer.c
 *
 * 	Description:  xtimer c file
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

#include "xtimer.h"

int initflag = 0;
TIMER_MANAGER timer_manager = {0};
#define TIMER(id)  timer_manager.timers[id]
 


static void sig_func(int signo)
{ 
	int i;
	for (i=0; i < MAX_TIMER_CNT; i++)
	{
		if(TIMER(i).enable)
		{
			TIMER(i).tick+=timer_manager.unit;
        		if(TIMER(i).tick >= TIMER(i).interval*1000)
			{
            			TIMER(i).tick = 0;
				TIMER(i).timer_proc(TIMER(i).param);
			}
		}
    } 
}



int start_timer(int interval, TIMER_PROC timer_proc, void *param)
{
	int timer_id, i;
	for (timer_id=0; timer_id<MAX_TIMER_CNT; timer_id++)
		if (TIMER(timer_id).alive == FALSE)
			break;
	if (timer_id==MAX_TIMER_CNT)
		return -1;
		
	TIMER(timer_id).enable = TRUE;
	TIMER(timer_id).alive = TRUE;
	TIMER(timer_id).param = param;
	TIMER(timer_id).timer_proc = timer_proc;
	TIMER(timer_id).interval = interval;
	TIMER(timer_id).tick = 0;

	return timer_id;
}

int reset_timer(int timer_id, int interval)
{
	if (timer_id>=0&&timer_id<MAX_TIMER_CNT && TIMER(timer_id).alive)
	{
		TIMER(timer_id).interval = interval;
		return 0;
	}
	return -1;
}


int stop_timer(int timer_id)
{
	if (timer_id>=0&&timer_id<MAX_TIMER_CNT)
	{
		TIMER(timer_id).enable = FALSE;
		return 0;
	}
	return -1;
}


int resume_timer(int timer_id)
{
	if (timer_id>=0&&timer_id<MAX_TIMER_CNT)
	{
		TIMER(timer_id).enable = TRUE;
		return 0;
	}
	return -1;
}


int kill_timer(int timer_id)
{
	if ((timer_id>=0&&timer_id<MAX_TIMER_CNT) && TIMER(timer_id).alive)
	{
		TIMER(timer_id).enable = FALSE;
		TIMER(timer_id).alive = FALSE;
		return 0;
	}
	return -1;
}


int init_mtimer(int unit)
{
	if (initflag)
		return -1;
	initflag = 1;
	memset(&timer_manager,0x00, sizeof(TIMER_MANAGER));
	timer_manager.unit = (unit==0) ? DEFAULT_TIMER_UNIT : unit;

	timer_manager.value.it_interval.tv_usec = timer_manager.unit;
	timer_manager.value.it_interval.tv_sec = 0;
	timer_manager.value.it_value.tv_usec = timer_manager.unit;
	timer_manager.value.it_value.tv_sec = 0;

	if( setitimer(ITIMER_REAL, &(timer_manager.value),  NULL) < 0 )
		return -1; 
		
	if( (timer_manager.old_sigfunc = signal(SIGALRM, sig_func)) == SIG_ERR)
		return -1;
	return 0;
}


int delete_mtimer()
{
	int i, ret = 0;
	if (!initflag)
		return -1;
	initflag = 0;
	if ((signal(SIGALRM, timer_manager.old_sigfunc)) == SIG_ERR)
       	 ret = -1;
	if (setitimer(ITIMER_REAL, &(timer_manager.ovalue), NULL) < 0)
		ret = -1;
	return ret;
}



