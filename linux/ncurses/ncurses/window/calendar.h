/*
 * =====================================================================================
 *
 *	Filename:  calendar.h
 *
 * 	Description:  the calendar of list header
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

#ifndef _CALENDAR_H_2012_
#define _CALENDAR_H_2012_

#include "win.h"

//#define LUNAR_ON

#define WEEK_START_SUNDAY
#define FT_NOW (void *)0
#define DEFAULT_START_YEAR   2000

typedef struct  _LY_TIME
{
	int tm_sec;     /* seconds	after the minute - [0,59]	*/
	int tm_min;     /* minutes	after the hour - [0,59]		*/
	int tm_hour;    /* hours	since midnight - [0,23]		*/
	int tm_mday;    /* day of the month - [1,31]			*/
	int tm_mon;     /* months since January - [0,11]		*/
	int tm_year;    /* years since 2000						*/
}LY_TIME;

#ifdef LUNAR_ON
typedef struct _LUNAR_TIME
{
	int lu_year;	/* lunar year								*/
	int lu_mon;		/* lunar month						*/
	int lu_mday;	/* lunar day								*/

	int lu_isBig;	/* this month is big month or not		*/
}LUNAR_TIME;
#endif

typedef struct _LY_CALENDAR
{
	int day[42];	
	int year;
	int month;
#ifdef LUNAR_ON	
	int lunar_day[42];
	int lunar_mon;
	int lunar_year;
#endif
	int today;
}LY_CALENDAR;



typedef struct _CALENDAR
{
	ALL_HAVE_ATRRIBUTE
	int focus;

	int year;
	int month;
	int day;
}CALENDAR, *PCALENDAR;


LY_TIME *getLocalTime(LY_TIME *lytm);
CALENDAR *create_calendar(CALENDAR* calendar, void *parent);
int delete_calendar(CALENDAR* calendar);
int calendar_driver(CALENDAR *calendar, int msg);

#endif /*_CALENDAR_H_2012_*/

