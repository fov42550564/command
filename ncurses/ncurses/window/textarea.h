/*
 * =====================================================================================
 *
 *	Filename:  textarea.h
 *
 * 	Description:  the file of textarea header
 *
 *	Version:  1.0
 *	Created:  12/28/11 23:56:48
 *	Revision:  none
 * 	Compiler:  gcc
 *
 *	Author:  fangyunjiang (fang), 42550564@qq.com
 *	Company:  star-net
 *
 * =====================================================================================
 */

#ifndef _TEXTAREA_H_2012_
#define _TEXTAREA_H_2012_

#include "win.h"

#define TMP_BUF_FILE  		"buf.txt"
#define TMP_SEARCH_FILE  	"search.txt"

#define DEFAULT_LINE_LENTGH 256
#define DEFAULT_LINES_COUNT 512
#define DEFAULT_CMD_LINES_LEN  64

typedef enum
{
	MODE_COMMAND,
	MODE_INSERT,
	MODE_REPLACE,
	MODE_REPLACE_INSERT,
	MODE_DELETE,
	MODE_COPY,
	MODE_G_KEY, //for gg gu gh gj gk gl
	MODE_LAST_LINE_COMMAND,
	MODE_SEARCH,
}MODE_TYPE_E;

typedef enum
{
	DO_NOT_UPDATE,
	UPDATE_A_LINE,
	UPDATE_LINE_BELOW,
	UPDATE_ALL_LINE,	
}UPDATE_TYPE_E;


/*clip board is a string with '\n' seperate, lines_cnt==0 for singal line*/
typedef struct _CLIPBOARD
{
	int lines_cnt;
	char *buf;
	int len;
}CLIPBOARD, *PCLIPBOARD;

//for : and / command
typedef CHARS_LINE CMD_LINE;

typedef struct _EDITLINE
{
	int max_len;
	char *chars;
	int len;

	int y;
	int sections;
}EDITLINE, *PEDITLINE;


typedef struct _UPDATELINES
{
	int start_line;  		//line index in window
	int lines_cnt;		//show update lines
	int start_id;		//line id
	int segs_id;		//line seg id
}UPDATELINES, *PUPDATELINES;


typedef struct _EDITAREA
{
	int max_lines_count;
	EDITLINE **lines;
	int lines_count;
	
	int cur_line_id;
	int cur_line_pos;
	
	int is_end;
	int remain_pos;

	int top_section_y;
	
	int max_show_lines;
	int max_chars_cnt;
	int mode;

	int times;
	int update_type;

	UPDATELINES *update_lines;
	CLIPBOARD *clipboard;
	CMD_LINE *cmd_line;
	
	int replace_chars_cnt; //just use for R

	int *search_list;     //for search postion
	int search_list_len;
}EDITAREA, *PEDITAREA;


typedef struct _TEXTAREA
{
	ALL_HAVE_ATRRIBUTE
	
	int type;
	bool focus;

	int start_x;
	int start_y;
	EDITAREA * edit_area;
}TEXTAREA;


typedef  void (*KEY_HANDER)(EDITAREA *);

typedef struct _KEY_HANDER_TABLE
{
	int key;
	KEY_HANDER hander;
	
}KEY_HANDER_TABLE, *PKEY_HANDER_TABLE;


TEXTAREA *create_textarea(TEXTAREA *editarea, void *parent);
int delete_textarea(TEXTAREA *editarea);
//int set_textarea_value(TEXTAREA *editarea, char *value);
//int get_textarea_value(TEXTAREA *editarea, char *value, int len);

int textarea_driver(TEXTAREA *editarea, int msg);

#endif /*_TEXTAREA_H_2012_*/
