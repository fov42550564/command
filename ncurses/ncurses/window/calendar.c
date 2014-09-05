/*
 * =====================================================================================
 *
 *	Filename:  calendar.c
 *
 * 	Description:  calendar c file
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

#include <time.h>
#include "calendar.h"


static const int DAYS_OF_MONTH_IN_NONLEEPYEAR[13]	= {0,31,59,90,120,151,181,212,243,273,304,334,365};
static const int DAYS_OF_MONTH_IN_LEEPYEAR[13]		= {0,31,60,91,121,152,182,213,244,274,305,335,366};

#ifdef LUNAR_ON
static const char *TIANGAN_NAME[]	= {"甲","乙","丙","丁","戊","己","庚","辛","壬","癸"};					/*天干名称*/ 
static const char *DIZHI_NAME[]	= {"子","丑","寅","卯","辰","巳","午", "未","申","酉","戌","亥"};		/*地支名称*/
static const char *SHUXIANG_NAME[] = {"鼠","牛","虎","兔","龙","蛇", "马","羊","猴","鸡","狗","猪"}; 		/*属相名称*/ 

static const char *WEEK_NAME[] = {"日", "一", "二", "三", "四", "五", "六"};
static const char *LUNAR_MONTH_NAME[] = {"正","二","三","四","五","六", "七","八","九","十","冬","腊"};
static const char *LUNAR_DAY_NAME[] = {
	"初一","初二","初三","初四","初五", "初六","初七","初八","初九","初十", 
		"十一","十二","十三","十四","十五", "十六","十七","十八","十九","二十", 
		"廿一","廿二","廿三","廿四","廿五", "廿六","廿七","廿八","廿九","三十"};
#else
static const char *WEEK_NAME[] = {"SUN", "MON", "TUE", "WEN", "THU", "FRI", "STA"};
#endif

//1970-1-1 08:00:00

/************************************************************************/
/*                             static functions                         */
/************************************************************************/

//判断是否是闰年
static bool _isLeapYear(int y)
{
	return !(y % 4) && (y % 100) || !(y % 400); 
}

//计算闰年数，不包括本年
static int _gainLeapYear(int y)
{
	return (y - 1) / 4 - (y - 1) / 100 + (y - 1) / 400;
}


//计算从sy-01-01 --  ty-01-01 之间的闰年数， 不包括本年
static int _gainDifLeapYear(int sy, int ty)
{
	return _gainLeapYear(ty) - _gainLeapYear(sy);
}


//计算某一天在一年中的第几天
static int _gainYearDay(int y, int m, int d)
{
	return _isLeapYear(y) ? DAYS_OF_MONTH_IN_LEEPYEAR[m] + d : DAYS_OF_MONTH_IN_NONLEEPYEAR[m] + d; 
}


//计算某一天是星期几
static int _gainWeekDay(int y, int m, int d)
{
	return (y - 1 + (y - 1) / 4 - (y - 1) / 100 + (y - 1) / 400 + _gainYearDay(y, m, d)) % 7;
}


//计算这个周是一年中第几周		本年度新的一周从周日开始
static int _gainYearWeek(int y, int m, int d)
{
	int swday, twday, tyday, yweek;
	swday = _gainWeekDay(y, 0, 1);
	twday = _gainWeekDay(y, m, d);
	tyday = _gainYearDay(y, m, d);

#ifdef WEEK_START_SUNDAY
	yweek = (tyday<=twday) ? 0 : ((swday > 0) ? (tyday - (7 - swday) - 1) / 7 + 1 : (tyday - 1) / 7 + 1);
#else
	swday = (swday + 6) % 7;
	twday = (twday + 6) % 7;
	yweek =  (tyday<=twday) ? 0 : ((swday > 0) ? (tyday - (7 - swday) - 1) / 7 + 1 : (tyday - 1) / 7 + 1);
#endif

	return yweek;
}

//计算一年中的第几个小时
static long _gainYearHour(int y, int m, int d, int h)
{
	return (_gainYearDay(y, m, d) - 1) * 24 + h;
}


//计算一年中的第几分钟
static long _gainYearMinute(int y, int m, int d, int h, int mt)
{
	return _gainYearHour(y, m, d, h) * 60 + mt;
}



//计算一年中的第几秒钟
static long _gainYearSecond(int y, int m, int d, int h, int mt, int s)
{
	return _gainYearMinute(y, m, d, h, s) * 60 + s;
}



//计算从2000-01-01 开始的第几个月
static int _gainFrom2000Month(int y, int m)
{
	return (y - 2000) * 12 + m;
}



//计算从2000-01-01 开始的第几天
static int _gainFrom2000Day(int y, int m, int d)
{
	int lyear, yday;
	lyear = _gainDifLeapYear(2000, y);
	yday = _gainYearDay(y, m, d);

	return (y - 2000) * 365 + lyear + yday;
}


//计算从2000-01-01 开始的第几个周
static int _gainFrom2000Week(int y, int m, int d)
{
	int swday, twday, tyday, yweek;
	swday = _gainWeekDay(2000, 0, 1);
	twday = _gainWeekDay(y, m, d);
	tyday = _gainFrom2000Day(y, m, d);

#ifdef WEEK_START_SUNDAY
	yweek = (tyday<=twday) ? 0 : ((swday > 0) ? (tyday - (7 - swday) - 1) / 7 + 1 : (tyday - 1) / 7 + 1);
#else
	swday = (swday + 6) % 7;
	twday = (twday + 6) % 7;
	yweek = (tyday<=twday) ? 0 : ((swday > 0) ? (tyday - (7 - swday) - 1) / 7 + 1 : (tyday - 1) / 7 + 1);
#endif
	return yweek;
}


//计算从2000-01-01 开始的第几小时
static long _gainFrom2000Hour(int y, int m, int d, int h)
{
	return (_gainFrom2000Day(y, m, d) - 1) * 24 + h;
}


//计算从2000-01-01 开始的第几分
static long _gainFrom2000Minute(int y, int m, int d, int h, int mt)
{
	return _gainFrom2000Hour(y, m, d, h) * 60 + mt;
}


//计算从2000-01-01 开始的第几秒
static long _gainFrom2000Second(int y, int m, int d, int h, int mt, int s)
{
	return _gainFrom2000Minute(y, m, d, h, mt) * 60 + s;
}


//通过从2000-01-01 00:00:00 经过的秒数算出现在的日期
static LY_TIME *_gainLYTime(LY_TIME *lytm, time_t s)
{
	int nmon, nh, nmin, nsec, leapy;
	int sy, sd, ty, td, dy;
	
	nsec = s % (3600 * 24);
	nh = nsec / 3600;
	nmin = (nsec % 3600) / 60;
	nsec %= 60;
	
	sd = s / (3600 * 24) + 1;
	//sy = 2000;
	sy = DEFAULT_START_YEAR;
	while (1)
	{
		if ((_isLeapYear(sy) && sd <= 366) || (!_isLeapYear(sy) && sd <= 365))
		{
			break;
		}
		dy = sd / 366;
		td = sd % 366;
		ty = dy + sy;
		leapy = _gainDifLeapYear(sy, ty);
		td += dy - leapy;
		
		sd = td;
		sy = ty;
	}
	
	sd++;
	if (_isLeapYear(sy))
	{
		for (nmon = 0; nmon < 13; nmon++)
		{
			if (sd <= DAYS_OF_MONTH_IN_LEEPYEAR[nmon])
			{
				sd -= DAYS_OF_MONTH_IN_LEEPYEAR[nmon - 1];
				break;
			}
		}
	}
	else
	{
		for (nmon = 0; nmon < 13; nmon++)
		{
			if (sd <= DAYS_OF_MONTH_IN_NONLEEPYEAR[nmon])
			{
				sd -= DAYS_OF_MONTH_IN_LEEPYEAR[nmon - 1];
				break;
			}
		}
	}
	
	lytm->tm_year = sy;
	lytm->tm_mon = nmon - 1;
	lytm->tm_mday = sd;
	lytm->tm_hour = nh;
	lytm->tm_min = nmin;
	lytm->tm_sec = nsec;
	
	return lytm;
}


static int _gainSubOfDate(int y1, int m1, int d1, int y2, int m2, int d2)
{
	return _gainFrom2000Day(y1, m1, d1) - _gainFrom2000Day(y2, m2, d2);
}




//计算从StartYear-01-01 开始的第几个月
static int _gainFromStartYearMonth(int y, int m)
{
	return (y - DEFAULT_START_YEAR) * 12 + m;
}



//计算从StartYear-01-01 开始的第几天
static int _gainFromStartYearDay(int y, int m, int d)
{
	int lyear, yday;
	lyear = _gainDifLeapYear(DEFAULT_START_YEAR, y);
	yday = _gainYearDay(y, m, d);
	
	return (y - DEFAULT_START_YEAR) * 365 + lyear + yday;
}


//计算从StartYear-01-01 开始的第几个周
static int _gainFromStartYearWeek(int y, int m, int d)
{
	int swday, twday, tyday, yweek;
	swday = _gainWeekDay(DEFAULT_START_YEAR, 0, 1);
	twday = _gainWeekDay(y, m, d);
	tyday = _gainFromStartYearDay(y, m, d);
	
#ifdef WEEK_START_SUNDAY
	yweek = (tyday<=twday) ? 0 : ((swday > 0) ? (tyday - (7 - swday) - 1) / 7 + 1 : (tyday - 1) / 7 + 1);
#else
	swday = (swday + 6) % 7;
	twday = (twday + 6) % 7;
	yweek = (tyday<=twday) ? 0 : ((swday > 0) ? (tyday - (7 - swday) - 1) / 7 + 1 : (tyday - 1) / 7 + 1);
#endif
	return yweek;
}


//计算从StartYear-01-01 开始的第几小时
static long _gainFromStartYearHour(int y, int m, int d, int h)
{
	return (_gainFromStartYearDay(y, m, d) - 1) * 24 + h;
}


//计算从StartYear-01-01 开始的第几分
static long _gainFromStartYearMinute(int y, int m, int d, int h, int mt)
{
	return _gainFromStartYearHour(y, m, d, h) * 60 + mt;
}


//计算从StartYear-01-01 开始的第几秒
static long _gainFromStartYearSecond(int y, int m, int d, int h, int mt, int s)
{
	return _gainFromStartYearMinute(y, m, d, h, mt) * 60 + s;
}


//计算日期差
static int _gainMonthMaxDay(int year, int month)
{
	const int MONTH_DAY[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30 ,31};
	if (_isLeapYear(year) && month == 1)
	{
		return MONTH_DAY[month] + 1;
	}
	return MONTH_DAY[month];
}


#ifdef LUNAR_ON
char *_gainLunarYear(char* ly, int year)
{
	int jiazi, tiangan, dizhi;
	jiazi = (year - 4) % 60;
	tiangan = jiazi % 10;
	dizhi = jiazi % 12;
	sprintf(ly, "%s%s年", TIANGAN_NAME[tiangan], DIZHI_NAME[dizhi]);
	return ly;
}


char * _gainShengXiao(char* sx, int year)
{
	sprintf(sx, "%s年", SHUXIANG_NAME[(year - 4) % 60 / 12]);
	return sx;
}


//获取农历
static LUNAR_TIME *_gainLunarTime(LUNAR_TIME *lu_tm, int year, int month, int day)
{
	int i, index, lm_tag, leap_month;
	int zm, zd;			//农历正月初一的公历
	int ly, lm, ld;			//农历
	
	/********************************************************************************************************/
	/*										1901-2100间的农历数据											*/
	/*																										*/
	/*结构解析：例1901																						*/
	/*			十六进制：0x04AE53	|	二进制：0000 0100 1010 1110 0101 0011								*/
	/*			24-21	：0000						|	表示当年的闰月月份，如果为0，表示没有当年没有闰月	*/
	/*			20-8	：0100 1010 1110 0101 0		|	表示对应的月份的大小，如果有闰月，如闰2月			*/
	/*												|	则闰2月排在17位的3月，农历3月则排在16位的4月		*/
	/*			7-6		：10						|	表示春节所在的月份									*/
	/*			5-1		：1 0011					|	表示春节所在的号数									*/
	/*只取1999-2099一百年的数据，取1999年时为了计算2000春节以前的日期										*/
	/********************************************************************************************************/
	const int LUNAR_DATA[] ={
		/*0x04AE53,0x0A5748,0x5526BD,0x0D2650,0x0D9544,0x46AAB9,0x056A4D,0x09AD42,0x24AEB6,0x04AE4A,/*1901-1910 */
		/*0x6A4DBE,0x0A4D52,0x0D2546,0x5D52BA,0x0B544E,0x0D6A43,0x296D37,0x095B4B,0x749BC1,0x049754,/*1911-1920 */
		/*0x0A4B48,0x5B25BC,0x06A550,0x06D445,0x4ADAB8,0x02B64D,0x095742,0x2497B7,0x04974A,0x664B3E,/*1921-1930 */
		/*0x0D4A51,0x0EA546,0x56D4BA,0x05AD4E,0x02B644,0x393738,0x092E4B,0x7C96BF,0x0C9553,0x0D4A48,/*1931-1940 */
		/*0x6DA53B,0x0B554F,0x056A45,0x4AADB9,0x025D4D,0x092D42,0x2C95B6,0x0A954A,0x7B4ABD,0x06CA51,/*1941-1950 */ 
		/*0x0B5546,0x555ABB,0x04DA4E,0x0A5B43,0x352BB8,0x052B4C,0x8A953F,0x0E9552,0x06AA48,0x7AD53C,/*1951-1960 */ 
		/*0x0AB54F,0x04B645,0x4A5739,0x0A574D,0x052642,0x3E9335,0x0D9549,0x75AABE,0x056A51,0x096D46,/*1961-1970 */ 
		/*0x54AEBB,0x04AD4F,0x0A4D43,0x4D26B7,0x0D254B,0x8D52BF,0x0B5452,0x0B6A47,0x696D3C,0x095B50,/*1971-1980 */
		/*0x049B45,0x4A4BB9,0x0A4B4D,0xAB25C2,0x06A554,0x06D449,0x6ADA3D,0x0AB651,0x093746,0x5497BB,/*1981-1990 */
		/*0x04974F,0x064B44,0x36A537,0x0EA54A,0x86B2BF,0x05AC53,0x0AB647,0x5936BC,*/0x092E50,0x0C9645,/*1991-2000 */
		0x4D4AB8,0x0D4A4C,0x0DA541,0x25AAB6,0x056A49,0x7AADBD,0x025D52,0x092D47,0x5C95BA,0x0A954E,/*2001-2010 */
			0x0B4A43,0x4B5537,0x0AD54A,0x955ABF,0x04BA53,0x0A5B48,0x652BBC,0x052B50,0x0A9345,0x474AB9,/*2011-2020 */
			0x06AA4C,0x0AD541,0x24DAB6,0x04B64A,0x69573D,0x0A4E51,0x0D2646,0x5E933A,0x0D534D,0x05AA43,/*2021-2030 */
			0x36B537,0x096D4B,0xB4AEBF,0x04AD53,0x0A4D48,0x6D25BC,0x0D254F,0x0D5244,0x5DAA38,0x0B5A4C,/*2031-2040 */
			0x056D41,0x24ADB6,0x049B4A,0x7A4BBE,0x0A4B51,0x0AA546,0x5B52BA,0x06D24E,0x0ADA42,0x355B37,/*2041-2050 */
			0x09374B,0x8497C1,0x049753,0x064B48,0x66A53C,0x0EA54F,0x06B244,0x4AB638,0x0AAE4C,0x092E42,/*2051-2060 */
			0x3C9735,0x0C9649,0x7D4ABD,0x0D4A51,0x0DA545,0x55AABA,0x056A4E,0x0A6D43,0x452EB7,0x052D4B,/*2061-2070 */
			0x8A95BF,0x0A9553,0x0B4A47,0x6B553B,0x0AD54F,0x055A45,0x4A5D38,0x0A5B4C,0x052B42,0x3A93B6,/*2071-2080 */
			0x069349,0x7729BD,0x06AA51,0x0AD546,0x54DABA,0x04B64E,0x0A5743,0x452738,0x0D264A,0x8E933E,/*2081-2090 */
			0x0D5252,0x0DAA47,0x66B53B,0x056D4F,0x04AE45,0x4A4EB9,0x0A4D4C,0x0D1541,0x2D92B5/*,0x0D5349 /*2091-2100 */
	};


	index = year - 1999;
	ly = year;
	lm = 0;					//正月
	
	leap_month = LUNAR_DATA[index] >> 20;
	zm = ((LUNAR_DATA[index] >> 5) & 0x3) - 1;
	zd = LUNAR_DATA[index] & 0x1f;
	
	ld = _gainSubOfDate(year, month, day, year, zm, zd) + 1;
	if (ld < 1)//在春节之前的日期要算到上一年
	{
		index--;
		ly--;
		leap_month = LUNAR_DATA[index] >> 20;
		zm = ((LUNAR_DATA[index] >> 5) & 0x3) - 1;
		zd = LUNAR_DATA[index] & 0x1f;
		ld = _gainSubOfDate(year, month, day, year - 1, zm, zd) + 1;
	}
	
	for (i = 1; i < 14; i++)
	{
		lm_tag = (LUNAR_DATA[index] >> 7) & (1 << (13 - i));
		lm_tag = !!lm_tag;
		if (ld > 29 + lm_tag)
		{
			ld -= (29 + lm_tag);
			lm++;
		}
		else
		{
			lu_tm->lu_isBig = lm_tag;
			break;
		}
	}
	
	if (leap_month && lm > leap_month )
	{
		lm--;
	}
	
	lu_tm->lu_year = ly;
	lu_tm->lu_mon = lm;
	lu_tm->lu_mday = ld;
	return lu_tm;
}
#endif


//获取日历
static LY_CALENDAR *_gainCalendar(LY_CALENDAR *ly_calendar, int year, int mon, int day)
{
	int i, tmp;

	ly_calendar->year = year;
	ly_calendar->month = mon;
	ly_calendar->today = day;
	
	int max_day = _gainMonthMaxDay(year, mon);
	int s_wday = _gainWeekDay(year,mon, 1);
	tmp = s_wday;
	
	memset(ly_calendar->day, 0, 42 * sizeof(int));
	
	for (i = 1; i <= max_day; i++)
	{
		ly_calendar->day[s_wday++] = i;
	}
#ifdef LUNAR_ON
	LUNAR_TIME lu_tm;
	memset(ly_calendar->lunar_day, 0, 42 * sizeof(int));
	_gainLunarTime(&lu_tm, year, mon, 1);
	
	int dif_d1 = 29 + lu_tm.lu_isBig - lu_tm.lu_mday + 1;		//农历月前的天数
	int dif_d2 = max_day - dif_d1;								//农历岳后的天数
	
	if (max_day >= dif_d1)
	{
		for (i = 0; i < dif_d1; i++)
		{
			ly_calendar->lunar_day[tmp++] = lu_tm.lu_mday + i;
		}
		
		_gainLunarTime(&lu_tm, year, mon, ly_calendar->day[tmp + 1]);
		
		ly_calendar->lunar_day[tmp++] = lu_tm.lu_mon + 100;
		for (i = 0; i < dif_d2 - 1; i++)
		{
			ly_calendar->lunar_day[tmp++] = lu_tm.lu_mday + i;
		}
	}
	else
	{
		for (i = 0; i < max_day; i++)
		{
			ly_calendar->lunar_day[tmp++] = lu_tm.lu_mday + i;
		}
		
	}
	ly_calendar->lunar_mon = lu_tm.lu_mon;
	ly_calendar->lunar_year = lu_tm.lu_year;
#endif
	return ly_calendar;
}


LY_TIME *getLocalTime(LY_TIME *lytm)
{
	struct tm *newtime;
        time_t long_time;

        time( &long_time ); 
        newtime = localtime( &long_time );

	lytm->tm_year = newtime->tm_year;
	lytm->tm_mon =newtime->tm_mon;
	lytm->tm_mday =newtime->tm_mday;
	lytm->tm_hour =newtime->tm_hour;
	lytm->tm_min =newtime->tm_min;
	lytm->tm_sec =newtime->tm_sec;
	//const time_t SECONDS_FROM_1970_TO_2000 = 946656000;
	//time_t now = time(0);
	//return _gainLYTime(lytm, now - SECONDS_FROM_1970_TO_2000);
	return lytm;
}


LY_CALENDAR g_ly_calendar = {0};
int g_year;
int g_month;


static int show_calendar(CALENDAR* calendar)
{
	int i, j;
	LY_CALENDAR *cal = &g_ly_calendar;
	_gainCalendar(cal, calendar->year, calendar->month, calendar->day);

	move_to(calendar, 1,1);
#ifdef LUNAR_ON
	char ly[10] = {0};
	char lm[10] = {0};
	char sx[10] = {0};
	wprintw(MAIN_SCREEN, "      %d-%d  %s年(%s)%s月 ", cal->year, cal->month+1, _gainLunarYear(ly, cal->lunar_year), 
		_gainShengXiao(sx,  cal->lunar_year),   LUNAR_MONTH_NAME[cal->lunar_mon]);
#else
	wprintw(MAIN_SCREEN, "          %d   %d  ", cal->year, cal->month+1);
#endif
	draw_hline(calendar, 1,2,calendar->w-2);
	mvbaddch(calendar,0,1, '<');
	mvbaddch(calendar,calendar->w-1,1, '>');
	mvbaddch(calendar,calendar->w/2,0, ACS_UARROW);
	mvbaddch(calendar,calendar->w/2,2, ACS_DARROW);
	move_to(calendar, 1,3);
#ifdef 	LUNAR_ON
	wattrset(MAIN_SCREEN, A_REVERSE); 
	for (i=0; i<7; i++)
	{
		wprintw(MAIN_SCREEN, "  %s  ", WEEK_NAME[i]);
	}
	wattrset(MAIN_SCREEN, 0); 
	for (i = 0; i < 6; i++)
	{
		move_to(calendar, 1, 4+2*i);
		for (j = 0; j < 7; j++)
		{
			if (!cal->day[i * 7 + j])
			{
				waddstr(MAIN_SCREEN, "      ");
			}
			else if(cal->day[i * 7 + j] == cal->today && cal->year==g_year && cal->month==g_month)
			{
				wattrset(MAIN_SCREEN, A_REVERSE); 
				wprintw(MAIN_SCREEN,  "  %s%d  ", (cal->day[i * 7 + j]<10)?" ":"", cal->day[i * 7 + j]);
				wattrset(MAIN_SCREEN, 0); 
			}
			else
			{
				wprintw(MAIN_SCREEN,  "  %s%d  ", (cal->day[i * 7 + j]<10)?" ":"", cal->day[i * 7 + j]);
			}
		}
		move_to(calendar, 1, 4+2*i+1);
		for (j = 0; j < 7; j++)
		{
			if (!cal->day[i * 7 + j])
			{
				waddstr(MAIN_SCREEN, "      ");
			}
			else if(cal->lunar_day[i * 7 + j] <100)
			{
				wprintw(MAIN_SCREEN,  " %s ", LUNAR_DAY_NAME[cal->lunar_day[i * 7 + j]-1]);
			}
			else
			{
				wprintw(MAIN_SCREEN,  " %s月 ", LUNAR_MONTH_NAME[cal->lunar_day[i * 7 + j]-100]);
			}
		}
	}
#else
	wattrset(MAIN_SCREEN, A_REVERSE); 
	for (i=0; i<7; i++)
	{
		wprintw(MAIN_SCREEN, "%s ", WEEK_NAME[i]);
	}
	wattrset(MAIN_SCREEN, 0); 
	for (i = 0; i < 6; i++)
	{
		move_to(calendar, 1, 4+i);
		for (j = 0; j < 7; j++)
		{
			if (!cal->day[i * 7 + j])
			{
				waddstr(MAIN_SCREEN, "    ");
			}
			else if(cal->day[i * 7 + j] == cal->today && cal->year==g_year && cal->month==g_month)
			{
				wattrset(MAIN_SCREEN, A_REVERSE); 
				wprintw(MAIN_SCREEN,  " %s%d ", (cal->day[i * 7 + j]<10)?" ":"", cal->day[i * 7 + j]);
				wattrset(MAIN_SCREEN, 0); 
			}
			else
			{
				wprintw(MAIN_SCREEN,  " %s%d ", (cal->day[i * 7 + j]<10)?" ":"", cal->day[i * 7 + j]);
			}
		}
	}
#endif
}




CALENDAR *create_calendar(CALENDAR* calendar, void *parent)
{
	LY_TIME ly_time;
	
	calendar->parent = (AREAR *)parent;
#ifdef LUNAR_ON
	calendar->w = 44;
	calendar->h = 17;
#else
	calendar->w = 30;
	calendar->h = 11;
#endif
	getLocalTime(&ly_time);
	calendar->year = ly_time.tm_year;
	calendar->month = ly_time.tm_mon;
	calendar->day = ly_time.tm_mday;
	g_year = calendar->year;
	g_month = calendar->month;

	draw_boder(calendar);
	show_calendar(calendar);
	
	return calendar;
}


int calendar_driver(CALENDAR *calendar, int msg)
{
	if (!calendar->focus)
		return RET_ERROR;
	
	switch (msg)
	{
		case KEY_UP:
			calendar->month--;
			if (calendar->month<0)
			{
				calendar->month = 11;
				calendar->year--;
			}
			show_calendar(calendar);
			break;
		case KEY_DOWN:
			calendar->month++;
			if (calendar->month>11)
			{
				calendar->month = 0;
				calendar->year++;
			}
			show_calendar(calendar);
			break;
		case KEY_LEFT:
			calendar->year--;
			show_calendar(calendar);
			break;
		case KEY_RIGHT:
			calendar->year++;
			show_calendar(calendar);
			break;
		default:
			break;
	}
	return RET_OK;
}


