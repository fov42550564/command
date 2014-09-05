/*
 * =====================================================================================
 *
 *	Filename:  common.h
 *
 * 	Description:  the file of common header
 *
 *	Version:  1.0
 *	Created:  12/28/11 20:32:01
 *	Revision:  none
 * 	Compiler:  gcc
 *
 *	Author:  fangyunjiang (fang), 42550564@qq.com
 *	Company:  star-net
 *
 * =====================================================================================
 */

#ifndef _COMMON_H_2012_
#define _COMMON_H_2012_

#include <curses.h>
#include <stdlib.h>
#include <string.h> 

#ifdef ASCII_DEFINED
#undef ACS_ULCORNER
#undef ACS_LLCORNER
#undef ACS_URCORNER
#undef ACS_LRCORNER
#undef ACS_HLINE
#undef ACS_VLINE
#undef ACS_LTEE
#undef ACS_RTEE
#undef ACS_UARROW
#undef ACS_DARROW
#endif

#ifndef ACS_ULCORNER
#define ACS_ULCORNER '+'
#endif
#ifndef ACS_LLCORNER
#define ACS_LLCORNER '+'
#endif
#ifndef ACS_URCORNER
#define ACS_URCORNER '+'
#endif
#ifndef ACS_LRCORNER
#define ACS_LRCORNER '+'
#endif
#ifndef ACS_HLINE
#define ACS_HLINE '-'
#endif
#ifndef ACS_VLINE
#define ACS_VLINE '|'
#endif
#ifndef ACS_LTEE
#define ACS_LTEE '+'
#endif
#ifndef ACS_RTEE
#define ACS_RTEE '+'
#endif
#ifndef ACS_UARROW
#define ACS_UARROW '^'
#endif
#ifndef ACS_DARROW
#define ACS_DARROW 'v'
#endif

#ifndef PUTTY_DEFINED
#define VK_BACKSPACE 	263
#define VK_ENTER 		10
#define VK_TAB 		9
#define VK_ESC 		27
#define VK_SPACE  		32
#define VK_EXIT		126
#else
#define VK_BACKSPACE 	127
#define VK_ENTER 		10
#define VK_TAB 		9
#define VK_ESC 		27
#define VK_SPACE  		32
#define VK_EXIT		KEY_F(1)
#endif


#define RET_ERROR      	-1
#define RET_OK      		0
#define RET_EXIT      	1
#define RET_CLICK         2
#define RET_YES         	3
#define RET_NO         	4
#define RET_TIMEOUT 	5


#define HIWORD(l)		((short)((((int)(l)) >> 16) & 0xFFFF))
#define LOWORD(l)		((short)(int)(l))
#define MAKELONG(hi, low)	((int)(((short)(low))|(((int)((short)(hi)))<<16)))

#define MIN(x,y) ((x) < (y) ? (x) : (y))
#define MAX(x,y) ((x) > (y) ? (x) : (y))
#define REVERSE(x) x = !(x)
#define SIZEOF(a) (sizeof(a)/sizeof(a[0]))
#define IS_VISIBLE_CHAR(ch) ((ch)>=32 && (ch<=126))

#ifdef offsetof
#undef offsetof
#endif
#define offsetof(type, member) ((size_t)(&((type *)0)->member))
#define container_of(ptr, type, member) ({ \
	const typeof(((type *)0)->member) *_ptr = ptr; \
	(type *)((char *)_ptr-offsetof(type, member));})


#define safe_free(p) do \
{ \
	if (!p) \
	{ \
		free(p); \
		p = NULL; \
	} \
} while(0);


#define XDEBUG(format, arg...)  do \
{ \
	def_prog_mode(); \
   	endwin(); \
   	printf("%s:"format, "[star-net][fang]", ## arg); \
   	reset_prog_mode(); \
   	refresh(); \
}while (0)


#define DEBUG(format, arg...) mvwprintw(MAIN_SCREEN, 40,10, format, ##arg)


#define MAIN_SCREEN  stdscr
#define ALL_HAVE_ATRRIBUTE \
	int x; \
	int y; \
	int w; \
	int h; \
	struct _AREAR*parent; \


typedef struct _CHARS_LINE
{
	char *chars;
	int len;
	int pos;
}CHARS_LINE, *PCHARS_LINE;


typedef struct _AREAR
{
	ALL_HAVE_ATRRIBUTE
}AREAR, *PAREAR;

#define ctoi(x)  (x-'0')
#define offset_x(a)  client_x_to_screen(a)
#define offset_y(a)  client_y_to_screen(a)

#define set_cusor(a, x, y)  wmove(MAIN_SCREEN, (offset_y(a)+y), (offset_x(a)+x))
#define set_cusoron()   printf("\033[?25h")
#define set_cusoroff()   printf("\033[?25l")

int client_x_to_screen(/*AREAR*/void *ptr);
int client_y_to_screen(/*AREAR*/void *ptr);

int move_to(/*AREAR*/void *ptr, int x, int y);

int draw_box(/*AREAR*/void *ptr, int x, int y, int w, int h);
int cover_box(/*AREAR*/void *ptr, int x, int y, int w, int h);
int clear_box(/*AREAR*/void *ptr, int x, int y, int w, int h);
int draw_boder(/*AREAR*/void *ptr);

int draw_hline(/*AREAR*/void *ptr, int x, int y, int len);
int draw_vline(/*AREAR*/void *ptr, int x, int y, int len);
int draw_up_start_obline(/*AREAR*/void *ptr, int x, int y, int len);
int draw_down_start_obine(/*AREAR*/void *ptr, int x, int y, int len);

int mvbaddch(/*AREAR*/void *ptr, int x, int y, int ch);
int mvbaddstr(/*AREAR*/void *ptr, int x, int y, char *str);
int mvbprintw(/*AREAR*/void *ptr, int x, int y, char *fmt, ...);


int update(void);
int init_all(void);
int exit_all(void);


#endif /*_COMMON_H_2012_*/
