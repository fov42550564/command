/*
 * =====================================================================================
 *
 *	Filename:  xtimer.h
 *
 * 	Description:  the xtimer of list header
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

#ifndef _XTIMER_H_2012_
#define _XTIMER_H_2012_

#include <sys/time.h>
#include "win.h"

#define MAX_TIMER_CNT 		10
#define DEFAULT_TIMER_UNIT 	100*1000


typedef void (* TIMER_MANAGER_PROC)(int);
typedef int (* TIMER_PROC)(void *);

typedef struct _MTIMER
{
	int interval;  	/*unit*/
	int tick;		/*unit*/
	bool enable;
	bool alive;
	void *param;
	TIMER_PROC timer_proc;
}MTIMER, *PMTIMER;


typedef struct _TIMER_MANAGER
{
	int unit;        /*microseconds*/
	MTIMER timers[MAX_TIMER_CNT];

	void (* old_sigfunc)(int);
	void (* new_sigfunc)(int);
	struct itimerval value;
	struct itimerval ovalue;
}TIMER_MANAGER, *PTIMER_MANAGER;


int start_timer(int interval, TIMER_PROC timer_proc, void *argv);
int reset_timer(int timer_id, int interval);
int stop_timer(int timer_id);
int resume_timer(int timer_id);
int kill_timer(int timer_id);
int init_mtimer(int unit);
int delete_mtimer();



#endif /*_XTIMER_H_2012_*/

