/*
 * =====================================================================================
 *
 *	Filename:  textarea.c
 *
 * 	Description:  textarea c file
 *
 *	Version:  1.0
 *	Created:  12/28/11 23:57:08
 *	Revision:  none
 * 	Compiler:  gcc
 *
 *	Author:  fangyunjiang (fang), 42550564@qq.com
 *	Company:  star-net
 *
 * =====================================================================================
 */

#include "textarea.h"


#define WIN_TOP_Y(edit_area) 			(edit_area->top_section_y)
#define WIN_BOTTOM_Y(edit_area) 			(WIN_TOP_Y(edit_area)+(edit_area)->max_show_lines-1)

#define HEAD_SECTIONS(edit_area) 		((edit_area)->cur_line_pos/(edit_area)->max_chars_cnt)
#define TAIL_SECTIONS(edit_area) 		(CUR_LINE_SECTIONS(edit_area)-HEAD_SECTIONS(edit_area)-1)
#define CURSOR_SECTION_ID(edit_area) 	(HEAD_SECTIONS(edit_area))
#define TOTAL_SECTIONS(edit_area)		(LAST_LINE(edit_area)->y+LAST_LINE(edit_area)->sections)

#define CUR_LINE(edit_area) 				((edit_area)->lines[(edit_area)->cur_line_id])
#define CUR_LINE_SECTIONS(edit_area) 	((edit_area)->lines[(edit_area)->cur_line_id]->sections)
#define LAST_LINE(edit_area)				((edit_area)->lines[(edit_area)->lines_count-1])

#define CUR_BEFORE_CHARS(edit_area)		((edit_area)->cur_line_pos)
#define CUR_AFTER_CHARS(edit_area)		(CUR_LINE(edit_area)->len-edit_area->cur_line_pos -1)

#define CURSOR_X(edit_area)  				((edit_area)->cur_line_pos%(edit_area)->max_chars_cnt)
#define CURSOR_Y(edit_area) 				(CUR_LINE_TOP_Y(edit_area) +HEAD_SECTIONS(edit_area))

#define CURSOR_UP_WIN_LINES(edit_area)		(CURSOR_Y(edit_area)-WIN_TOP_Y(edit_area))
#define CURSOR_DOWN_WIN_LINES(edit_area)	(WIN_BOTTOM_Y(edit_area)-CURSOR_Y(edit_area))

#define CURSOR_UP_LINES(edit_area)			(edit_area->cur_line_id)
#define CURSOR_DOWN_LINES(edit_area)		(edit_area->lines_count-edit_area->cur_line_id-1)

#define CURSOR_UP_SECTIONS(edit_area)		(CURSOR_Y(edit_area) )
#define CURSOR_DOWN_SECTIONS(edit_area)	(LAST_LINE(edit_area)->y+LAST_LINE(edit_area)->sections-CURSOR_Y(edit_area)-1)

#define CUR_LINE_TOP_Y(edit_area) 		((edit_area)->lines[(edit_area)->cur_line_id]->y)
#define CUR_LINE_BOTTOM_Y(edit_area) 	(CUR_LINE_TOP_Y(edit_area)+CUR_LINE_SECTIONS(edit_area)-1)

#define CYRSOR_WIN_X(edit_area) 			(CURSOR_X(edit_area))
#define CYRSOR_WIN_Y(edit_area) 			(CURSOR_Y(edit_area)-WIN_TOP_Y(edit_area))

#define GET_SECTIONS_BY_LEN(edit_area, len)	((len-1)/(edit_area)->max_chars_cnt+1)

#define CURSOR_AT_LEFT_MRGIN(edit_area) 	((edit_area)->cur_line_pos%(edit_area)->max_chars_cnt==0)
#define CURSOR_AT_RIGHT_MRGIN(edit_area) 	(((edit_area)->cur_line_pos+1)%(edit_area)->max_chars_cnt==0)
#define CURSOR_AT_TOP_MRGIN(edit_area) 		(CYRSOR_WIN_Y(edit_area)==0)
#define CURSOR_AT_BOTTOM_MRGIN(edit_area)  	(CYRSOR_WIN_Y(edit_area)==(edit_area)->max_show_lines-1)
#define CURSOR_AT_FIRST_LINE(edit_area) 	((edit_area)->cur_line_id==0)
#define CURSOR_AT_LAST_LINE(edit_area) 		((edit_area)->cur_line_id==(edit_area)->lines_count-1)
#define CURSOR_AT_END_OF_LINE(edit_area)  	((edit_area)->cur_line_pos==CUR_LINE(edit_area)->len-1)
#define CURSOR_AT_BEGIN_OF_LINE(edit_area) ((edit_area)->cur_line_pos==0)

#define SCROLL_UP(edit_area)  			(edit_area)->top_section_y--
#define SCROLL_DOWN(edit_area)  			(edit_area)->top_section_y++
#define SCROLL_UP_N(edit_area,n)  		(edit_area)->top_section_y-=n
#define SCROLL_DOWN_N(edit_area,n)  		(edit_area)->top_section_y+=n

#define STATUS_BAR_X(textarea) 			(1)
#define STATUS_BAR_Y(textarea) 			((textarea)->start_y+(textarea->edit_area)->max_show_lines)

#define  release_a_line(line) do \
{ \
	safe_free(line->chars); \
	safe_free(line); \
} while (0)

#define check_scoll_down(edit_area) do { \
	int des_y, bottom_y, n; \
	des_y = CURSOR_Y(edit_area); \
	bottom_y = WIN_BOTTOM_Y(edit_area); \
	n = MAX(0, des_y-bottom_y); \
	SCROLL_DOWN_N(edit_area, n); \
	if (n > 0) \
		set_update_type(edit_area,UPDATE_ALL_LINE); \
} while (0)

#define check_scoll_up(edit_area) do { \
	int   des_y, top_y, n; \
	des_y = CURSOR_Y(edit_area); \
	top_y = WIN_TOP_Y(edit_area); \
	n = MAX(0, top_y-des_y); \
	SCROLL_UP_N(edit_area, n); \
	if (n > 0) \
		set_update_type(edit_area,UPDATE_ALL_LINE); \
} while (0)

#define SET_UPDATE_ALL(edit_area) do { \
	edit_area->update_lines->start_line = 0; \
	edit_area->update_lines->lines_cnt = edit_area->max_show_lines; \
	get_top_line_id(edit_area); \
} while (0)

#define SET_UPDATE_CUR_LINE(edit_area) do { \
	edit_area->update_lines->start_line = CYRSOR_WIN_Y(edit_area); \
	edit_area->update_lines->lines_cnt = MIN(TAIL_SECTIONS(edit_area)+1,CURSOR_DOWN_WIN_LINES(edit_area)+1); \
	edit_area->update_lines->start_id = edit_area->cur_line_id; \
	edit_area->update_lines->segs_id = CURSOR_SECTION_ID(edit_area); \
} while (0)

#define SET_UPDATE_CUR_BELLOW_LINES(edit_area) do { \
	edit_area->update_lines->start_line = CYRSOR_WIN_Y(edit_area); \
	edit_area->update_lines->lines_cnt = CURSOR_DOWN_WIN_LINES(edit_area)+1; \
	edit_area->update_lines->start_id = edit_area->cur_line_id; \
	edit_area->update_lines->segs_id = CURSOR_SECTION_ID(edit_area); \
} while (0)


#define clear_status_bar(textarea) clear_box(textarea, 1,STATUS_BAR_Y(textarea), textarea->w-2, 1)

#define update_cursor(textarea) set_cusor(textarea, CYRSOR_WIN_X(textarea->edit_area)+ \
	textarea->start_x, CYRSOR_WIN_Y(textarea->edit_area)+textarea->start_y)

#define set_mode(edit_area, _mode) 	edit_area->mode = _mode
#define set_update_type(edit_area, type) 	edit_area->update_type = type


typedef void (*CMD_LINE_CALLBACK)(TEXTAREA *);


static int get_top_line_id(EDITAREA *edit_area)
{
	int start_id, segs_id, y0, yt;

	y0 = CUR_LINE_TOP_Y(edit_area);
	yt = WIN_TOP_Y(edit_area);
	start_id = edit_area->cur_line_id;
	
	while (y0 > yt)
		y0 = edit_area->lines[--start_id]->y;
	segs_id= yt - edit_area->lines[start_id]->y;
	
	 edit_area->update_lines->start_id = start_id; 
	 edit_area->update_lines->segs_id =segs_id; 

	 return start_id;
}

static int get_bottom_line_id(EDITAREA *edit_area)
{
	int start_id, segs_id, y0, yt;

	y0 = CUR_LINE_TOP_Y(edit_area);
	yt = WIN_BOTTOM_Y(edit_area);
	start_id = edit_area->cur_line_id;
	
	while (y0<=yt && start_id<edit_area->lines_count-1)
		y0 = edit_area->lines[++start_id]->y;
		
	if (start_id < edit_area->lines_count-1)
		start_id--;
	segs_id= yt - y0;
	
	 edit_area->update_lines->start_id = start_id; 
	 edit_area->update_lines->segs_id =segs_id; 

	  return start_id;
}


static int move_line_end(EDITAREA *edit_area)
{
	edit_area->cur_line_pos =CUR_LINE(edit_area)->len-1;
	check_scoll_down(edit_area);
	edit_area->is_end = 1;
	return 0;
}


static int move_line_begin(EDITAREA *edit_area)
{
	edit_area->remain_pos = edit_area->cur_line_pos = 0;
	check_scoll_up(edit_area);
	edit_area->is_end = 0;
	return 0;
}

/*curser move left*/
static int move_left_char(EDITAREA *edit_area)
{
	int ret = -1;
	if (edit_area->cur_line_pos > 0)
	{
		if (CURSOR_AT_TOP_MRGIN(edit_area) && CURSOR_AT_LEFT_MRGIN(edit_area))
		{
			SCROLL_UP(edit_area);
			set_update_type(edit_area,UPDATE_ALL_LINE);
		}
	
		edit_area->remain_pos = --edit_area->cur_line_pos;
		edit_area->is_end = 0;
		ret = 0;
	}
	return ret;
}


//don't check the boder 
static int move_right_always(EDITAREA *edit_area)
{
	if (CURSOR_AT_BOTTOM_MRGIN(edit_area) && CURSOR_AT_RIGHT_MRGIN(edit_area))
	{
		SCROLL_DOWN(edit_area);
		set_update_type(edit_area,UPDATE_ALL_LINE);
	}
	edit_area->cur_line_pos++;
	return 0;
}



/*curser move right*/
static int move_right_char(EDITAREA *edit_area)
{
	int ret = -1, is_scoll = 0;
	if (edit_area->cur_line_pos < CUR_LINE(edit_area)->len-1)
	{
		move_right_always(edit_area);
		
		edit_area->remain_pos = edit_area->cur_line_pos;
		edit_area->is_end = 0;
		ret = 0;
	}
	return ret;
}

/*curser move up*/
static int move_up_line(EDITAREA *edit_area)
{
	int ret = -1,  pos;

	if (edit_area->cur_line_id > 0)
	{
		edit_area->cur_line_id--;
		pos =  CUR_LINE(edit_area)->len-1;
		if (pos < 0)
			pos = 0;
		if ( edit_area->is_end)
			edit_area->cur_line_pos = pos;
		else
			edit_area->cur_line_pos = MIN(pos, edit_area->remain_pos);

		check_scoll_up(edit_area);
		
		ret = 0;
	}
	return ret;
}

/*curser move up by section*/
static int move_up_section(EDITAREA *edit_area)
{
	int  x, y, head_segs, seg_id,  win_top_y, up_segs, len;
	y = CURSOR_Y(edit_area);
	if (y > 0)
	{
		win_top_y = WIN_TOP_Y(edit_area);
		x = CURSOR_X(edit_area);
		head_segs = HEAD_SECTIONS(edit_area);
		seg_id = CURSOR_SECTION_ID(edit_area);

		if (head_segs==0)
		{
			edit_area->cur_line_id--;
			up_segs = CUR_LINE_SECTIONS(edit_area);
			len = CUR_LINE(edit_area)->len;
			edit_area->cur_line_pos = MIN((up_segs-1)*edit_area->max_chars_cnt+x, CUR_LINE(edit_area)->len-1);
		}
		else
		{
			edit_area->cur_line_pos-= edit_area->max_chars_cnt;
		}
		if (y==win_top_y)
		{
			SCROLL_UP(edit_area);
			set_update_type(edit_area,UPDATE_ALL_LINE);
		}
	}
}



/*curser move down*/
static int move_down_line(EDITAREA *edit_area)
{
	int ret = -1, pos;
	
	if (edit_area->cur_line_id < edit_area->lines_count-1)
	{
		edit_area->cur_line_id++;
		pos =  CUR_LINE(edit_area)->len-1;
		if (pos < 0)
			pos = 0;
		if ( edit_area->is_end)
			edit_area->cur_line_pos = pos;
		else
			edit_area->cur_line_pos = MIN(pos, edit_area->remain_pos);

		check_scoll_down(edit_area);
		
		ret = 0;
	}
	return ret;
}


static void move_line_to(EDITAREA *edit_area, int n, int pos)
{
	int top_y, bottom_y;
	if (n<0)n=0;
	if (n>edit_area->lines_count-1)n=edit_area->lines_count-1;
	
	top_y = get_top_line_id(edit_area);
	bottom_y = get_bottom_line_id(edit_area);
	
	edit_area->cur_line_id = n;
	if (n <= top_y)
	{
		edit_area->top_section_y = CUR_LINE_TOP_Y(edit_area);
		set_update_type(edit_area,UPDATE_ALL_LINE);
	}
	else if (n >= bottom_y && bottom_y >edit_area->max_show_lines-1)
	{
		edit_area->top_section_y = CUR_LINE(edit_area)->y+ \
			CUR_LINE(edit_area)->sections-edit_area->max_show_lines;
		set_update_type(edit_area,UPDATE_ALL_LINE);
	}
	edit_area->remain_pos = edit_area->cur_line_pos = pos;
}


/*curser move down by section*/
static int move_down_section(EDITAREA *edit_area)
{
	int total_sections, x, y, tail_segs, seg_id,  win_bottom_y;
	y = CURSOR_Y(edit_area);
	total_sections = TOTAL_SECTIONS(edit_area);
	if (y < total_sections-1)
	{
		win_bottom_y = WIN_BOTTOM_Y(edit_area);
		x = CURSOR_X(edit_area);
		tail_segs = TAIL_SECTIONS(edit_area);
		seg_id = CURSOR_SECTION_ID(edit_area);

		if (tail_segs==0)
		{
			edit_area->cur_line_id++;
			edit_area->cur_line_pos = MIN(x, CUR_LINE(edit_area)->len-1);
		}
		else if (tail_segs>1)
		{
			edit_area->cur_line_pos+= edit_area->max_chars_cnt;
		}
		else
		{
			edit_area->cur_line_pos = MIN((seg_id+1)*edit_area->max_chars_cnt+x,  CUR_LINE(edit_area)->len-1);
		}
		if (y==win_bottom_y)
		{
			SCROLL_DOWN(edit_area);
			set_update_type(edit_area,UPDATE_ALL_LINE);
		}
	}
}



static int delete_a_char(EDITAREA *edit_area)
{
	int i, ret=0, line_id;
	EDITLINE *line = CUR_LINE(edit_area);
	
	if (line->len>0)
	{
		for (i=edit_area->cur_line_pos; i<line->len-1; i++)
			line->chars[i]=line->chars[i+1];
		
		line->len--;
		if (edit_area->cur_line_pos==line->len && line->len>0)
			edit_area->cur_line_pos--;
		if (edit_area->update_type == DO_NOT_UPDATE)
			set_update_type(edit_area,UPDATE_A_LINE);
		if (line->len>0 && line->len%edit_area->max_chars_cnt==0)
		{
			line->sections--;
			for (line_id = edit_area->cur_line_id+1;line_id<edit_area->lines_count; line_id++)
				edit_area->lines[line_id]->y--;
			set_update_type(edit_area,UPDATE_LINE_BELOW);
		}
		ret = -1;
	}
	edit_area->remain_pos = edit_area->cur_line_pos;
	edit_area->is_end = 0;
	return ret;
}


static EDITLINE *relloac_chars(EDITLINE * line, int len)
{
	int max_len;
	char  * chars = line->chars;

	max_len =  ((len-1)/DEFAULT_LINE_LENTGH+1)*DEFAULT_LINE_LENTGH;
	line->chars = (char *)calloc(max_len, sizeof(char));
	memcpy(line->chars, chars, line->max_len);
	line->max_len = max_len;
	safe_free(chars);
	return line;
}


static int insert_a_char(EDITAREA *edit_area, char ch)
{
	int i, line_id;
	EDITLINE *line = CUR_LINE(edit_area);
	line->len++;
	if (line->len > line->max_len)
		relloac_chars(line, 2*DEFAULT_LINE_LENTGH);
	for (i=line->len-1; i>edit_area->cur_line_pos; i--)
		line->chars[i] = line->chars[i-1];
	line->chars[i] = ch;
	if (edit_area->update_type == DO_NOT_UPDATE)
			set_update_type(edit_area,UPDATE_A_LINE);
	if (line->len>0 && line->len%edit_area->max_chars_cnt==0)
			set_update_type(edit_area,UPDATE_ALL_LINE);
	else if (line->len>1 && line->len%edit_area->max_chars_cnt==1)
	{
		line->sections++;
		for (line_id=edit_area->cur_line_id+1;line_id<edit_area->lines_count; line_id++)
			edit_area->lines[line_id]->y++;
		set_update_type(edit_area,UPDATE_LINE_BELOW);
	}
	move_right_always(edit_area);
	edit_area->remain_pos = edit_area->cur_line_pos;
	edit_area->is_end = 0;
	return 0;
}


static int insert_a_string(EDITAREA *edit_area, char*str, int len)
{
	int i, line_id, segs;
	EDITLINE *line = CUR_LINE(edit_area);
	segs = line->sections;
	line->len+=len;
	if (line->len > line->max_len)
		relloac_chars(line, line->len);
	for (i=line->len-1; i>edit_area->cur_line_pos+len-1; i--)
		line->chars[i] = line->chars[i-len];
	for (i=0; i<len; i++)
		line->chars[edit_area->cur_line_pos+i] = str[i];
	edit_area->cur_line_pos+=len;
	line->sections = GET_SECTIONS_BY_LEN(edit_area, line->len);
	
	segs = line->sections-segs;
	if (segs > 0)
	{
		for (line_id=edit_area->cur_line_id+1;line_id<edit_area->lines_count; line_id++)
			edit_area->lines[line_id]->y += segs;
		set_update_type(edit_area,UPDATE_ALL_LINE);
	}
	else
		set_update_type(edit_area,UPDATE_ALL_LINE);

	check_scoll_down(edit_area);

	edit_area->remain_pos = edit_area->cur_line_pos;
	edit_area->is_end = 0;
	return 0;
}



static int replace_a_char(EDITAREA *edit_area, char ch)
{
	int i, line_id;
	EDITLINE *line = CUR_LINE(edit_area);

	line->chars[edit_area->cur_line_pos] = ch;
	set_update_type(edit_area,UPDATE_A_LINE);
	edit_area->remain_pos = edit_area->cur_line_pos;
	edit_area->is_end = 0;
	return 0;
}



static int delete_a_line(EDITAREA *edit_area)
{
	EDITLINE *line = CUR_LINE(edit_area);
	int line_id = edit_area->cur_line_id;

	if (edit_area->lines_count==1)
	{
		line->len = 0;
		line->sections= 1;
	}
	else
	{
		for (;line_id<edit_area->lines_count-1; line_id++)
		{
			edit_area->lines[line_id] = edit_area->lines[line_id+1];
			edit_area->lines[line_id]->y -= line->sections;
		}
		edit_area->lines[line_id] = NULL;
		if (CURSOR_AT_LAST_LINE(edit_area))
			move_up_line(edit_area);
		edit_area->lines_count--;
		release_a_line(line);
	}
	set_update_type(edit_area,UPDATE_LINE_BELOW);
	edit_area->remain_pos = edit_area->cur_line_pos = 0;
	edit_area->is_end=0;
	return 0;
}

/*
static int delete_lines(EDITAREA *edit_area, int del_n)
{
	EDITLINE **del_lines;
	int line_id = edit_area->cur_line_id;
	int id, sections, n;

	n = MIN(del_n, edit_area->lines_count-edit_area->cur_line_id);
	
	if (line_id==0 && edit_area->lines_count<=n)
	{
		edit_area->lines[0]->len = 0;
		edit_area->lines[0]->sections= 1;
		for (id=1, id<n; id++)
			release_a_line(edit_area->lines[id]);
		edit_area->lines_count = 1;
	}
	else
	{
		sections = 0;
		del_lines = (EDITLINE **)calloc(n, sizeof(EDITLINE*));
		
		for (id=0, id<n; id++)
		{
			del_lines[id] = edit_area->lines[id+line_id];
			sections += del_lines[id]->sections;
		}
		for (id=line_id+n; id<edit_area->lines_count; id++)
		{
			edit_area->lines[id]->y -= sections;
			edit_area->lines[id-n] = edit_area->lines[id];
		}
		for (id=edit_area->lines_count-1; id>=edit_area->lines_count-n; id--)
			edit_area->lines[id] = NULL;
		if (CURSOR_AT_LAST_LINE(edit_area))
		{
			//move_up_line(edit_area);
			if (CURSOR_AT_TOP_MRGIN(edit_area))
				SCROLL_UP(edit_area);
			edit_area->cur_line_id--;
		}
		edit_area->lines_count -= n;
		for (id=0, id<n; id++)
			release_a_line(del_lines[id]);
	}
	edit_area->remain_pos = edit_area->cur_line_pos = 0;
	edit_area->is_end=0;
	return 0;
}*/


static EDITAREA *relloac_lines(EDITAREA *edit_area, int cnt)
{
	int i;
	EDITLINE **lines = edit_area->lines;
	
	edit_area->lines = (EDITLINE **)calloc(edit_area->max_lines_count+cnt, sizeof(EDITLINE*));
	for (i=0; i<edit_area->max_lines_count; i++)
		edit_area->lines[i] = lines[i];
	edit_area->max_lines_count += cnt;
	safe_free(lines);
	return edit_area;
}

static EDITLINE * create_a_line(EDITAREA *edit_area, int len)
{
	int max_len;
	EDITLINE *line;

	max_len = ((len-1)/DEFAULT_LINE_LENTGH+1)* DEFAULT_LINE_LENTGH;
	line = (EDITLINE *)calloc(1, sizeof(EDITLINE));
	line->chars = (char *)calloc(max_len, sizeof(char));
	line->max_len = max_len;
	line->sections = 1;
	line->len = 0;

	return line;
}


int set_a_line_string(EDITAREA *edit_area, int line_id, char*str, int len)
{
	int id, segs;

	segs = GET_SECTIONS_BY_LEN(edit_area, len);
	if (line_id >=edit_area->max_lines_count)
		relloac_lines(edit_area, DEFAULT_LINES_COUNT);

	for (id = edit_area->lines_count-1; id>line_id; id--)
		edit_area->lines[id]->y+= segs;
	
	if (edit_area->lines[line_id] == NULL)
	{
		edit_area->lines[line_id] = create_a_line(edit_area, len);
		edit_area->lines_count++;
		if (line_id==0)
			edit_area->lines[line_id]->y = 0;
		else
			edit_area->lines[line_id]->y = edit_area->lines[line_id-1]->y+ \
				edit_area->lines[line_id-1]->sections;
	}
	else if (edit_area->lines[line_id]->max_len < len)
		relloac_chars(edit_area->lines[line_id], 2*DEFAULT_LINE_LENTGH);

	edit_area->lines[line_id]->len = len;
	edit_area->lines[line_id]->sections ;
	memcpy(edit_area->lines[line_id]->chars, str, len);
	return 0;
}


static int insert_a_line(EDITAREA *edit_area, int line_id)
{
	int id, is_scoll = 0;

	if (line_id >=edit_area->max_lines_count)
		relloac_lines(edit_area, DEFAULT_LINES_COUNT);
	for (id = edit_area->lines_count; id>line_id; id--)
	{
		edit_area->lines[id] = edit_area->lines[id-1];
		edit_area->lines[id]->y++;
	}
	edit_area->lines[line_id] = create_a_line(edit_area, DEFAULT_LINE_LENTGH);
	if (line_id==0)
		edit_area->lines[line_id]->y = 0;
	else
		edit_area->lines[line_id]->y = edit_area->lines[line_id-1]->y+ \
			edit_area->lines[line_id-1]->sections;
	
	edit_area->lines_count++;
	edit_area->cur_line_id = line_id;
	edit_area->remain_pos = edit_area->cur_line_pos = 0;
	edit_area->is_end=0;
	return 0;
}


static void key_h_hander(EDITAREA *edit_area)
{
	move_left_char(edit_area);
}

static void key_j_hander(EDITAREA *edit_area)
{
	move_down_line(edit_area);
}

static void key_k_hander(EDITAREA *edit_area)
{
	move_up_line(edit_area);
}

static void key_l_hander(EDITAREA *edit_area)
{
	move_right_char(edit_area);
}

static void key_i_hander(EDITAREA *edit_area)
{
	set_mode(edit_area, MODE_INSERT);
}

static void key_I_hander(EDITAREA *edit_area)
{
	move_line_begin(edit_area);
	set_mode(edit_area, MODE_INSERT);
}

static void key_a_hander(EDITAREA *edit_area)
{
	move_right_always(edit_area);
	set_mode(edit_area, MODE_INSERT);
}

static void key_A_hander(EDITAREA *edit_area)
{
	move_line_end(edit_area);
	move_right_always(edit_area);
	set_mode(edit_area, MODE_INSERT);
	edit_area->is_end = 0;
}

//question
static void key_s_hander(EDITAREA *edit_area)
{
	if (edit_area->times == 0)edit_area->times = 1;
	edit_area->times = MIN(CUR_AFTER_CHARS(edit_area)+1, edit_area->times);
	while (edit_area->times--)
		delete_a_char(edit_area);
	if (CURSOR_AT_END_OF_LINE(edit_area))
		move_right_always(edit_area);
	set_mode(edit_area, MODE_INSERT);
	edit_area->times = 0;
}

//question
static void key_S_hander(EDITAREA *edit_area)
{
	int remain_lines, flag = 0;
	
	if (edit_area->times == 0)edit_area->times = 1;
	remain_lines = CURSOR_DOWN_LINES(edit_area)+1;

	if (remain_lines <= edit_area->times)
	{
		 flag = 1;
		 edit_area->times = remain_lines;
	}
	while (edit_area->times--)
		delete_a_line(edit_area);
	if (flag==1)
		insert_a_line(edit_area, edit_area->cur_line_id+1);
	else
		insert_a_line(edit_area, edit_area->cur_line_id);
	set_update_type(edit_area,UPDATE_LINE_BELOW);
	set_mode(edit_area, MODE_INSERT);
	edit_area->times = 0;
}

static void key_r_hander(EDITAREA *edit_area)
{
	set_mode(edit_area, MODE_REPLACE);
}

static void key_R_hander(EDITAREA *edit_area)
{
	set_mode(edit_area, MODE_REPLACE_INSERT);
	edit_area->replace_chars_cnt = CUR_AFTER_CHARS(edit_area)+1;
}

static void key_x_hander(EDITAREA *edit_area)
{
	if (edit_area->times == 0)edit_area->times = 1;
	
	edit_area->times = MIN(CUR_AFTER_CHARS(edit_area)+1, edit_area->times);
	while (edit_area->times--)
		delete_a_char(edit_area);
	edit_area->times = 0;
}

static void key_X_hander(EDITAREA *edit_area)
{
	if (edit_area->times == 0)edit_area->times = 1;
	edit_area->times = MIN(CUR_BEFORE_CHARS(edit_area), edit_area->times);
	while (edit_area->times--)
	{
		move_left_char(edit_area);
		delete_a_char(edit_area);
	}
	edit_area->times = 0;
}

static void key_d_hander(EDITAREA *edit_area)
{
	set_mode(edit_area, MODE_DELETE);
}

static void key_D_hander(EDITAREA *edit_area)
{
	int remain_lines, flag = 0, remain_pos;

	remain_pos = edit_area->cur_line_pos;
	remain_lines = CURSOR_DOWN_LINES(edit_area)+1;
	if (edit_area->times == 0)edit_area->times = 1;
	if (remain_lines <= edit_area->times)
	{
		 flag = 1;
		 edit_area->times = remain_lines;
	}
	if (edit_area->times>1)
		move_down_line(edit_area);
	while (--edit_area->times)
		delete_a_line(edit_area);
	if (flag==0)
		move_up_line(edit_area);
	edit_area->cur_line_pos = remain_pos;
	edit_area->times = CUR_AFTER_CHARS(edit_area)+1;
	while (edit_area->times--)
		delete_a_char(edit_area);
	set_update_type(edit_area,UPDATE_LINE_BELOW);
	set_mode(edit_area, MODE_COMMAND);
	edit_area->times = 0;

	
}

static void key_o_hander(EDITAREA *edit_area)
{
	insert_a_line(edit_area, edit_area->cur_line_id+1);
	set_update_type(edit_area,UPDATE_LINE_BELOW);
	check_scoll_down(edit_area);
	set_mode(edit_area, MODE_INSERT);
}

static void key_O_hander(EDITAREA *edit_area)
{
	insert_a_line(edit_area, edit_area->cur_line_id);
	set_update_type(edit_area,UPDATE_LINE_BELOW);
	check_scoll_up(edit_area);
	set_mode(edit_area, MODE_INSERT);
}

static void key_g_hander(EDITAREA *edit_area)
{
	set_mode(edit_area, MODE_G_KEY);
}

static void key_G_hander(EDITAREA *edit_area)
{
	edit_area->cur_line_id = edit_area->lines_count-1;
	edit_area->top_section_y = CUR_LINE(edit_area)->y+CUR_LINE(edit_area)->sections-edit_area->max_show_lines;
	edit_area->remain_pos = edit_area->cur_line_pos = 0;
	edit_area->is_end = 0;
	edit_area->times = 0;
	set_update_type(edit_area,UPDATE_ALL_LINE);
}

static void key_y_hander(EDITAREA *edit_area)
{
	set_mode(edit_area, MODE_COPY);
}

static void key_p_hander(EDITAREA *edit_area)
{
	int i, len;
	char *ptr0, *ptr1;
	CLIPBOARD *clipboard = edit_area->clipboard;

	if (clipboard->lines_cnt == 0)
		insert_a_string(edit_area, clipboard->buf, clipboard->len);
	else
	{
		ptr0 = clipboard->buf;
		for (i=0; i<clipboard->lines_cnt; i++)
		{
			key_o_hander(edit_area);
			ptr1 = strchr(ptr0, '\n');
			if (ptr1!=NULL)
				insert_a_string(edit_area, ptr0, (int)(ptr1-ptr0));
			ptr0 = ptr1+1;
		}
	}
	set_mode(edit_area, MODE_COMMAND);
}


static void key_n_hander(EDITAREA *edit_area)
{
	int i, ln = -1, pos;
	for (i=0; i<edit_area->search_list_len; i++)
	{
		ln = HIWORD(edit_area->search_list[i]);
		pos =  LOWORD(edit_area->search_list[i]);
		if (ln>edit_area->cur_line_id ||(ln==edit_area->cur_line_id && pos>edit_area->cur_line_pos))
			break;
	}
	if (ln!=-1)
	{
		if ( i >= edit_area->search_list_len)
		{
			ln = HIWORD(edit_area->search_list[0]);
			pos =  LOWORD(edit_area->search_list[0]);
		}
		
		move_line_to(edit_area, ln, pos);
	}
	set_mode(edit_area, MODE_COMMAND);
}

static void key_N_hander(EDITAREA *edit_area)
{
	int i, ln = -1, pos;

	for (i=0; i<edit_area->search_list_len; i++)
	{
		ln = HIWORD(edit_area->search_list[i]);
		pos =  LOWORD(edit_area->search_list[i]);
		if (ln>edit_area->cur_line_id ||(ln==edit_area->cur_line_id && pos>=edit_area->cur_line_pos))
			break;
	}
	if (ln!=-1)
	{
		i--;
		if ( i < 0)
		{
			ln = HIWORD(edit_area->search_list[edit_area->search_list_len-1]);
			pos =  LOWORD(edit_area->search_list[edit_area->search_list_len-1]);
		}
		else
		{
			ln = HIWORD(edit_area->search_list[i]);
			pos =  LOWORD(edit_area->search_list[i]);
		}
		move_line_to(edit_area, ln, pos);
	}
	set_mode(edit_area, MODE_COMMAND);
}


static void key_zero_hander(EDITAREA *edit_area)
{
	if (edit_area->times == 0)
		move_line_begin(edit_area);
}

static void key_dollor_hander(EDITAREA *edit_area)
{
	move_line_end(edit_area);
}

static void key_last_line_command_hander(TEXTAREA *textarea)
{
	set_mode(textarea->edit_area, MODE_LAST_LINE_COMMAND);
	clear_status_bar(textarea);
	mvbaddch(textarea, 2, STATUS_BAR_Y(textarea), ':');
}

static void key_search_hander(TEXTAREA *textarea)
{
	set_mode(textarea->edit_area, MODE_SEARCH);
	clear_status_bar(textarea);
	mvbaddch(textarea, 2, STATUS_BAR_Y(textarea), '/');
}

KEY_HANDER_TABLE key_hander_table[] =
{
	{'h',key_h_hander},
	{KEY_LEFT,key_h_hander},
	{VK_BACKSPACE,key_h_hander},
	{'j',key_j_hander},
	{KEY_DOWN,key_j_hander},
	{'k',key_k_hander},
	{KEY_UP,key_k_hander},
	{'l',key_l_hander},
	{KEY_RIGHT,key_l_hander},
	{'i',key_i_hander},
	{'I',key_I_hander},
	{'a',key_a_hander},
	{'A',key_A_hander},
	{'s',key_s_hander},
	{'S',key_S_hander},
	{'r',key_r_hander},
	{'R',key_R_hander},
	{'x',key_x_hander},
	{'X',key_X_hander},
	{'d',key_d_hander},
	{'D',key_D_hander},
	{'o',key_o_hander},
	{'O',key_O_hander},
	{'g',key_g_hander},
	{'G',key_G_hander},
	{'y',key_y_hander},
	{'p',key_p_hander},
	{'n',key_n_hander},
	{'N',key_N_hander},
	{'0',key_zero_hander},
	{'$',key_dollor_hander},
};


static void _show_status(TEXTAREA *textarea)
{
	int i, y=textarea->parent->h;
	EDITAREA *edit_area = textarea->edit_area;
	UPDATELINES *up = edit_area->update_lines;
	mvbprintw(textarea->parent, 1, y+1,  \
		"lines=%d,pos=%d,up=%d,segs=%d,cur=%d,top=%d", edit_area->lines_count, edit_area->cur_line_pos,
		edit_area->update_type, CUR_LINE(edit_area)->sections, edit_area->cur_line_id, \
		edit_area->top_section_y);

	mvbprintw(textarea->parent, 1,y+2,  \
		"sl=%d,lc=%d,si=%d,segid=%d", up->start_line, up->lines_cnt, up->start_id, up->segs_id);

	for (i=0; i<edit_area->lines_count; i++)
		mvbprintw(textarea->parent, 1,y+3+i, "%d-%d: %s", edit_area->lines[i]->y, \
		edit_area->lines[i]->sections,edit_area->lines[i]->chars);
}


static void show_status(TEXTAREA *textarea)
{
	EDITAREA *edit_area = textarea->edit_area;
	int i, y = STATUS_BAR_Y(textarea);
	clear_status_bar(textarea);
	if (edit_area->mode == MODE_COMMAND)
		mvbaddstr(textarea, 3, y,  "-command-");
	else if (edit_area->mode == MODE_INSERT)
		mvbaddstr(textarea, 3, y,  "-inert-");
	else if (edit_area->mode == MODE_REPLACE||edit_area->mode == MODE_REPLACE_INSERT)
		mvbaddstr(textarea, 3, y,  "-replace-");
	else if (edit_area->mode == MODE_G_KEY)
		mvbaddstr(textarea, 3, y,  "-g-command-");
	else if (edit_area->mode == MODE_DELETE)
		mvbaddstr(textarea, 3, y,  "-delete-");
	else if (edit_area->mode == MODE_COPY)
		mvbaddstr(textarea, 3, y,  "-copy-");
	mvbprintw(textarea, textarea->w-14, y, "-x:%d y:%d-", CYRSOR_WIN_X(edit_area), CYRSOR_WIN_Y(edit_area));
	update_cursor(textarea);
}


static void show_status_ex(TEXTAREA *textarea, char *info)
{
	EDITAREA *edit_area = textarea->edit_area;
	int y = textarea->start_y+edit_area->max_show_lines;
	clear_box(textarea, 1,y, textarea->w-2, 1);
	mvbprintw(textarea, 1, y, "%s", info);
	update_cursor(textarea);
}

int show_lines(TEXTAREA *textarea)
{
	int j, x, y=2, sections, len, show_blank=0;
	char *str=NULL, *tmp;
	int start_line, lines_cnt, start_id, segs_id;
	EDITAREA *edit_area = textarea->edit_area;
	EDITLINE *line;

	if (edit_area->update_type==DO_NOT_UPDATE)
		goto outer;
	else if (edit_area->update_type==UPDATE_A_LINE)
		SET_UPDATE_CUR_LINE(edit_area);
	else if (edit_area->update_type==UPDATE_LINE_BELOW)
		SET_UPDATE_CUR_BELLOW_LINES(edit_area);
	else if (edit_area->update_type==UPDATE_ALL_LINE)
		SET_UPDATE_ALL(edit_area);
	
	start_line = edit_area->update_lines->start_line;
	lines_cnt = edit_area->update_lines->lines_cnt;
	start_id = edit_area->update_lines->start_id;
	segs_id = edit_area->update_lines->segs_id;

	x = textarea->start_x;
	y = textarea->start_y+start_line;
	clear_box(textarea, 1, y, textarea->w-2, lines_cnt);
	
	sections = edit_area->lines[start_id]->sections - segs_id;
	tmp = edit_area->lines[start_id]->chars+segs_id*edit_area->max_chars_cnt;
	len = edit_area->lines[start_id]->len - segs_id*edit_area->max_chars_cnt;
	str = (char *)calloc(edit_area->max_chars_cnt+1, sizeof(char));
	
	while (lines_cnt>0)
	{
		if (sections == edit_area->lines[start_id]->sections) //show line number
			mvbprintw(textarea, 1, y, "%d", start_id+1);
		
		for (j=0; j<sections-1; j++)
		{
			memcpy(str, tmp, edit_area->max_chars_cnt);
			mvbaddstr(textarea, x, y++, str);
			tmp += edit_area->max_chars_cnt;
			len -= edit_area->max_chars_cnt;
			lines_cnt--;
			if (lines_cnt==0)
				goto outer;
		}
		memcpy(str, tmp, len);
		str[len] = '\0';
		mvbaddstr(textarea, x, y++, str);
		lines_cnt--;
		start_id++;
		if (start_id >= edit_area->lines_count)
		{
			show_blank = 1;
			break;
		}
			
		sections = edit_area->lines[start_id]->sections;
		tmp = edit_area->lines[start_id]->chars;
		len = edit_area->lines[start_id]->len;
	}
outer:	
	if (show_blank)
		while (y<textarea->start_y+edit_area->max_show_lines)
			mvbaddstr(textarea, 1, y++, "~");

	_show_status(textarea);
	show_status(textarea);
	edit_area->update_type=DO_NOT_UPDATE;
	safe_free(str);
	return 0;
}

/*set from fn to fn, line_cnt lines to clipboard*/
char *set_multy_line_clipboard(EDITAREA *edit_area, int fn, int line_cnt)
{
	int i, len = 0;
	CLIPBOARD *clipboard = edit_area->clipboard;
	char *tmp;
	
	clipboard->lines_cnt = line_cnt;

	safe_free(clipboard->buf);
	for (i=fn; i<fn+line_cnt; i++)
		len += edit_area->lines[i]->len+1; /*add a char '\n'*/
	clipboard->buf = (char *)calloc(len, sizeof(char));
	clipboard->len = len;

	tmp = clipboard->buf;
	for (i=fn; i<fn+line_cnt; i++)
	{
		memcpy(tmp, edit_area->lines[i]->chars, edit_area->lines[i]->len);
		tmp[edit_area->lines[i]->len] = '\n';
		tmp += edit_area->lines[i]->len+1;
	}
	
	return  clipboard->buf;
}


/*set from pos, len chars to clipboard*/
char *set_single_line_clipboard(EDITAREA *edit_area, int pos, int len)
{
	int i;
	CLIPBOARD *clipboard = edit_area->clipboard;
	char *tmp;
	
	clipboard->lines_cnt = 0;

	safe_free(clipboard->buf);
	clipboard->buf = (char *)calloc(len, sizeof(char));

	tmp = CUR_LINE(edit_area)->chars + pos;
	memcpy(clipboard->buf, tmp, len);
	clipboard->len = len;
	
	return  clipboard->buf;
}


static void hander_command_keys(TEXTAREA *textarea, int msg)
{
	int  i,  cmd_key_count;
	EDITAREA *edit_area = textarea->edit_area;
	
	cmd_key_count = SIZEOF(key_hander_table);

	
	for (i=0; i<cmd_key_count; i++)
	{
		if (key_hander_table[i].key == msg)
		{
			key_hander_table[i].hander(edit_area);
			break;
		}
	}
	if (msg >= '0' && msg <= '9')
		edit_area->times = edit_area->times *10 + ctoi(msg);

	if (msg == ':')
		key_last_line_command_hander(textarea);
	else if (msg=='/')
		key_search_hander(textarea);
	else
		show_lines(textarea);
}

static void hander_edit_keys(TEXTAREA *textarea, int msg)
{
	int len, up_id;
	char *tmp;
	EDITAREA *edit_area = textarea->edit_area;
	switch (msg)
	{
	case VK_BACKSPACE:
		if (edit_area->cur_line_pos>0)
		{
			move_left_char(edit_area);
			delete_a_char(edit_area);
			if (CURSOR_AT_END_OF_LINE(edit_area))
				move_right_always(edit_area);
		}
		break;
	case VK_ENTER:
		if (edit_area->cur_line_pos==CUR_LINE(edit_area)->len)
			key_o_hander(edit_area);
		else
		{
			len = CUR_AFTER_CHARS(edit_area)+1;
			up_id = edit_area->cur_line_id;
			CUR_LINE(edit_area)->len = edit_area->cur_line_pos;
			tmp = (char *)(CUR_LINE(edit_area)->chars)+edit_area->cur_line_pos;
			key_o_hander(edit_area);
			if (len > CUR_LINE(edit_area)->max_len)
				relloac_chars(CUR_LINE(edit_area), len);
			memcpy(CUR_LINE(edit_area)->chars, tmp, len);
			CUR_LINE(edit_area)->len = len;
			set_update_type(edit_area,UPDATE_ALL_LINE);
		}
		break;
	case KEY_LEFT:
		move_left_char(edit_area);
		break;
	case KEY_RIGHT:
		move_right_char(edit_area);
		if (CURSOR_AT_END_OF_LINE(edit_area))
			move_right_always(edit_area);
		break;
	case KEY_UP:
		move_up_section(edit_area);
		break;
	case KEY_DOWN:
		move_down_section(edit_area);
		break;
	default:
		insert_a_char(textarea->edit_area, msg);
	}
	show_lines(textarea);
}

static void hander_delete_keys(TEXTAREA *textarea, int msg)
{
	EDITAREA *edit_area = textarea->edit_area;
	switch (msg)
	{
	case 'd':
		if (edit_area->times == 0)edit_area->times = 1;
		edit_area->times=MIN(CURSOR_DOWN_LINES(edit_area)+1,edit_area->times);
		set_multy_line_clipboard(edit_area,edit_area->cur_line_id, edit_area->times);
		while (edit_area->times--)
			delete_a_line(edit_area);
		break;
	case '0':
		edit_area->times = CUR_BEFORE_CHARS(edit_area)+1;
		set_single_line_clipboard(edit_area, 0, edit_area->times);
		while (edit_area->times--)
		{
			delete_a_char(edit_area);
			move_left_char(edit_area);
		}
		break;
	case '$':
		edit_area->times = CUR_AFTER_CHARS(edit_area)+1;
		set_single_line_clipboard(edit_area, edit_area->cur_line_pos, edit_area->times );
		while (edit_area->times--)
			delete_a_char(edit_area);
		break;
	case 'G':
		edit_area->times=CURSOR_DOWN_LINES(edit_area)+1;
		set_multy_line_clipboard(edit_area,edit_area->cur_line_id, edit_area->times);
		while (edit_area->times--)
			delete_a_line(edit_area);
		break;
	default:;
	}
	edit_area->times = 0;
	set_mode(edit_area, MODE_COMMAND);
	show_lines(textarea);
}

static void hander_replace_keys(TEXTAREA *textarea, int msg)
{
	int i;
	EDITAREA *edit_area = textarea->edit_area;
	if (edit_area->times == 0)edit_area->times = 1;
	edit_area->times = MIN(CUR_AFTER_CHARS(edit_area)+1, edit_area->times);
	for (i=edit_area->cur_line_pos; i<edit_area->cur_line_pos+edit_area->times; i++)
		CUR_LINE(edit_area)->chars[i] = msg;
	edit_area->cur_line_pos += edit_area->times;
	edit_area->times = 0;
	set_update_type(edit_area,UPDATE_A_LINE);
	set_mode(edit_area, MODE_COMMAND);
	show_lines(textarea);
}


static void hander_replace_insert_keys(TEXTAREA *textarea, int msg)
{
	EDITAREA *edit_area = textarea->edit_area;
	int remain_chars = CUR_AFTER_CHARS(edit_area)+1;

	set_update_type(edit_area,UPDATE_A_LINE);
	if (edit_area->replace_chars_cnt > 0)
	{
		CUR_LINE(edit_area)->chars[CUR_LINE(edit_area)->len-edit_area->replace_chars_cnt] = msg;
		edit_area->replace_chars_cnt --;
		move_right_always(edit_area);
	}
	else
		set_mode(edit_area, MODE_INSERT);
	show_lines(textarea);
}

static void hander_copy_keys(TEXTAREA *textarea, int msg)
{
	char info[64] = {0};
	EDITAREA *edit_area = textarea->edit_area;
	switch (msg)
	{
	case 'y':
		if (edit_area->times == 0)edit_area->times = 1;
		edit_area->times=MIN(CURSOR_DOWN_LINES(edit_area)+1,edit_area->times);
		set_multy_line_clipboard(edit_area,edit_area->cur_line_id, edit_area->times);
		sprintf(info, "~ %d line(s) copied", edit_area->times);
		break;
	case '0':
		set_single_line_clipboard(edit_area, 0, CUR_BEFORE_CHARS(edit_area)+1);
		sprintf(info, "~ %d char(s) copied", CUR_BEFORE_CHARS(edit_area)+1);
		break;
	case '$':
		set_single_line_clipboard(edit_area, edit_area->cur_line_pos, CUR_AFTER_CHARS(edit_area)+1);
		sprintf(info, "~ %d char(s) copied", CUR_AFTER_CHARS(edit_area)+1);
		break;
	case 'b':
		if (edit_area->times == 0)edit_area->times = 1;
		edit_area->times=MIN(CUR_AFTER_CHARS(edit_area)+1,edit_area->times);
		set_single_line_clipboard(edit_area, edit_area->cur_line_pos, edit_area->times);
		sprintf(info, "~ %d chars(s) copied",  edit_area->times);
		break;
	case 'B':
		if (edit_area->times == 0)edit_area->times = 1;
		edit_area->times=MIN(CUR_BEFORE_CHARS(edit_area)+1,edit_area->times);
		set_single_line_clipboard(edit_area, edit_area->cur_line_pos-edit_area->times+1, edit_area->times);
		sprintf(info, "~ %d chars(s) copied",  edit_area->times);
		break;
	default:;
	}
	edit_area->times = 0;
	set_mode(edit_area, MODE_COMMAND);
	show_status_ex(textarea, info);
}


static void hander_g_command_keys(TEXTAREA *textarea, int msg)
{
	int i;
	EDITAREA *edit_area = textarea->edit_area;

	if (edit_area->times == 0)edit_area->times = 1;
	switch (msg)
	{
	case 'g':
		move_line_to(edit_area, edit_area->times-1, 0);
		break;
	case 'u':
	case 'U':
		edit_area->times = MIN(CUR_AFTER_CHARS(edit_area)+1, edit_area->times);
		for (i=edit_area->cur_line_pos; i<edit_area->cur_line_pos+edit_area->times; i++)
			CUR_LINE(edit_area)->chars[i] = (msg=='U')?toupper(CUR_LINE(edit_area)->chars[i]): \
				tolower(CUR_LINE(edit_area)->chars[i]);
		set_update_type(edit_area,UPDATE_A_LINE);
		break;
	case 'j':
		move_down_section(edit_area);
		break;
	case 'k':
		move_up_section(edit_area);
		break;
	default:;
	}
	edit_area->is_end = 0;
	edit_area->times = 0;
	set_mode(edit_area, MODE_COMMAND);
	show_lines(textarea);
}


static void content_to_file(EDITAREA *edit_area)
{
	FILE *fp = NULL;
	int i;

	fp = fopen(TMP_BUF_FILE, "w+");
	if (fp == NULL)
		return;
	
	for (i=0; i<edit_area->lines_count; i++)
		fprintf(fp, "%s\n", edit_area->lines[i]->chars);
	fclose(fp);
}


static void file_to_content(EDITAREA *edit_area)
{
	FILE *fp = NULL;
	char * line = NULL;
	size_t len = 0;
	ssize_t read;
	int line_id = 0, id;

	fp = fopen(TMP_BUF_FILE, "r");
	if (fp == NULL)
		return;
	
	while ((read = getline(&line, &len, fp)) != -1) 
		set_a_line_string(edit_area, line_id++, line, read-1);

	for (id=line_id; id<edit_area->lines_count; id++)
		release_a_line(edit_area->lines[id]);
	edit_area->lines_count = line_id;
	edit_area->cur_line_id = 0;
	edit_area->cur_line_pos = 0;
	edit_area->top_section_y = 0;
	
	fclose(fp);
	if (line)
		free(line);
}

/*
 *m,ns/x/x/
 *%s/x/x/
 *m,/x/s/x/x/  /x/,ms/x/x/
 *
 */
static void deal_with_command_line(TEXTAREA *textarea)
{
	int sl, ll;
	char cmd[126]={0};
	EDITAREA *edit_area = textarea->edit_area;
	content_to_file(edit_area);
	sprintf(cmd,"sed -i '%s' %s",edit_area->cmd_line, TMP_BUF_FILE);
	system(cmd);
	
	file_to_content(edit_area);
	set_update_type(edit_area,UPDATE_ALL_LINE);
	show_lines(textarea);
}


static void hander_cmd_line_keys(TEXTAREA *textarea, int msg, CMD_LINE_CALLBACK callback)
{
	int i, y;
	char *tmp;
	EDITAREA *edit_area = textarea->edit_area;
	CMD_LINE *cmd_line = edit_area->cmd_line;

	y = STATUS_BAR_Y(textarea);
	switch (msg)
	{
	case VK_ESC:
		set_mode(edit_area, MODE_COMMAND);
		cmd_line->len = 0;
		break;
		
	case VK_ENTER:
		cmd_line->chars[cmd_line->len] = '\0';
		callback(textarea);
		set_mode(edit_area, MODE_COMMAND);
		cmd_line->len = 0;
		break;
	case VK_BACKSPACE:
		if (cmd_line->pos>0)
		{
			for (i=cmd_line->pos; i<cmd_line->len-1; i++)
				cmd_line->chars[i] = cmd_line->chars[i+1];
			cmd_line->len--;
			cmd_line->pos--;
			cmd_line->chars[cmd_line->len] = '\0';
			clear_box(textarea, 3, y, textarea->w-4, 1);
			mvbaddstr(textarea, 3, y, cmd_line->chars);
			move_to(textarea,  3+cmd_line->pos, y);
		}
		break;
	case KEY_LEFT:
		if (cmd_line->pos>0)
			cmd_line->pos--;
		move_to(textarea,  3+cmd_line->pos, y);
		break;
	case KEY_RIGHT:
		if (cmd_line->pos<cmd_line->len)
			cmd_line->pos++;
		move_to(textarea,  3+cmd_line->pos, y);
		break;
	case KEY_UP:
		break;
	case KEY_DOWN:
		break;	
	default:
		if (IS_VISIBLE_CHAR(msg))
		{
			for (i=cmd_line->len; i>cmd_line->pos; i--)
				cmd_line->chars[i] = cmd_line->chars[i-1];
			cmd_line->chars[cmd_line->pos++] = msg;
			cmd_line->len++;
			cmd_line->chars[cmd_line->len] = '\0';
			clear_box(textarea, 3, y, textarea->w-4, 1);
			mvbaddstr(textarea, 3, y, cmd_line->chars);
			move_to(textarea,  3+cmd_line->pos, y);
		}
	}
}


static int get_match_counts(EDITAREA *edit_area, char *regex)
{
	FILE *fp = NULL;
	char * line = NULL;
	size_t len = 0;
	ssize_t read;
	char cmd[512]={0};
	int ln, pos, i = 0;
	
	content_to_file(edit_area);
	sprintf(cmd,"awk '{s0=0;p0=0;s=$0;do{match(s,\"%s\");if(RSTART)print NR,s0+RSTART; \
		p0=RSTART+RLENGTH;s0+=p0-1;s=substr(s,p0);}while(RSTART);}' %s>%s", regex, TMP_BUF_FILE, TMP_SEARCH_FILE);
	system(cmd);

	fp = fopen(TMP_SEARCH_FILE, "r");
	if (fp == NULL)
		return;
	while ((read = getline(&line, &len, fp)) != -1) 
		i++;
	fclose(fp);
	if (line)
		free(line);
	return i;
}


static int get_match_position(EDITAREA *edit_area, int *pos_arr)
{
	FILE *fp = NULL;
	char * line = NULL, *tmp;
	size_t len = 0;
	ssize_t read;
	int ln, pos, i = 0;
	
	fp = fopen(TMP_SEARCH_FILE, "r");
	if (fp == NULL)
		return;
	while ((read = getline(&line, &len, fp)) != -1) 
	{
		tmp = line;
		ln = atoi(tmp)-1;
		tmp = strchr(line, ' ');
		pos = atoi(tmp)-1;
		pos_arr[i++] = MAKELONG(ln,pos);
	}
	fclose(fp);
	if (line)
		free(line);
	return i;
}


static void deal_search_line(TEXTAREA *textarea)
{
	int i, ln = -1, pos;
	EDITAREA *edit_area = textarea->edit_area;
	
	edit_area->search_list_len = get_match_counts(edit_area, edit_area->cmd_line->chars);
	safe_free(edit_area->search_list);
	edit_area->search_list = (int *)calloc(edit_area->search_list_len, sizeof(int));
	get_match_position(edit_area, edit_area->search_list);

	for (i=0; i<edit_area->search_list_len; i++)
	{
		ln = HIWORD(edit_area->search_list[i]);
		pos =  LOWORD(edit_area->search_list[i]);
		if (ln>edit_area->cur_line_id ||(ln==edit_area->cur_line_id && pos>=edit_area->cur_line_pos))
			break;
	}
	if (ln==-1)
	{
		show_status_ex(textarea, "-not find-");
	}
	else
	{
		if ( i >= edit_area->search_list_len)
		{
			ln = HIWORD(edit_area->search_list[0]);
			pos =  LOWORD(edit_area->search_list[0]);
		}
		
		move_line_to(edit_area, ln, pos);
		show_lines(textarea);
	}
}


static void hander_search_keys(TEXTAREA *textarea, int msg)
{
	static int pos = 0;
	EDITAREA *edit_area = textarea->edit_area;
	switch (msg)
	{
	case VK_ESC:
		set_mode(edit_area, MODE_COMMAND);
		pos = 0;
		break;
		
	case VK_ENTER:
		edit_area->cmd_line[pos] = '\0';
		set_mode(edit_area, MODE_COMMAND);
		deal_search_line(textarea);
		pos = 0;
		break;
	default:
		if (IS_VISIBLE_CHAR(msg))
		{
			edit_area->cmd_line[pos++] = msg;
			waddch(MAIN_SCREEN, msg);
		}
	}
}


int textarea_driver(TEXTAREA *textarea, int msg)
{
	EDITAREA *edit_area;
	if (!textarea->focus)
		return RET_ERROR;

	edit_area = textarea->edit_area;
	if (VK_ESC == msg)
	{
		if (edit_area->mode == MODE_INSERT)
		{
			move_left_char(edit_area);
			update_cursor(textarea);
		}
		edit_area->times = 0;
		set_mode(edit_area, MODE_COMMAND);
		show_status(textarea);
	}
	else if (MODE_COMMAND == edit_area->mode)
	{
		hander_command_keys(textarea, msg);
	}
	else  if (MODE_INSERT == edit_area->mode)
	{
		hander_edit_keys(textarea, msg);
	}
	else if (MODE_DELETE == edit_area->mode)
	{
		hander_delete_keys(textarea, msg);
	}
	else if (MODE_REPLACE == edit_area->mode)
	{
		hander_replace_keys(textarea, msg);
	}
	else if (MODE_REPLACE_INSERT == edit_area->mode)
	{
		hander_replace_insert_keys(textarea, msg);
	}
	else if (MODE_COPY == edit_area->mode)
	{
		hander_copy_keys(textarea, msg);
	}
	else if (MODE_G_KEY == edit_area->mode)
	{
		hander_g_command_keys(textarea, msg);
	}
	else if (MODE_LAST_LINE_COMMAND == edit_area->mode)
	{
		hander_cmd_line_keys(textarea, msg, deal_with_command_line);
	}
	else if (MODE_SEARCH == edit_area->mode)
	{
		hander_cmd_line_keys(textarea, msg, deal_search_line);
	}
	return RET_OK;
}


static EDITAREA * create_a_editarea(EDITAREA *edit_area, TEXTAREA *textarea)
{
	int y = 2;
	
	memset(edit_area, 0x00, sizeof(EDITAREA));
	edit_area->max_lines_count= DEFAULT_LINES_COUNT;
	edit_area->max_chars_cnt = textarea->w-textarea->start_x-1; //reverse for line number
	edit_area->max_show_lines = textarea->h-3; //last line is status
	edit_area->update_lines = (UPDATELINES *)calloc(1, sizeof(UPDATELINES));;
	edit_area->clipboard = (CLIPBOARD*)calloc(1, sizeof(CLIPBOARD));
	edit_area->lines = (EDITLINE **)calloc(DEFAULT_LINES_COUNT, sizeof(EDITLINE*));
	edit_area->cmd_line = (CMD_LINE*)calloc(1, sizeof(CMD_LINE));
	edit_area->cmd_line->chars = (char*)calloc(DEFAULT_CMD_LINES_LEN, sizeof(char));
	
	insert_a_line(edit_area, 0);
	set_mode(edit_area, MODE_COMMAND);
	mvbprintw(textarea, 1, 1, "1"); //first line's line number
	while (y<textarea->start_y+edit_area->max_show_lines)
			mvbaddstr(textarea, 1, y++, "~");
	show_status(textarea);
	return edit_area;
}


static int release_a_editarea(EDITAREA *edit_area)
{
	int id ;
	for (id = edit_area->lines_count-1; id>=0; id--)
		release_a_line(edit_area->lines[id]);
	safe_free(edit_area->lines);
	safe_free(edit_area->update_lines);
	safe_free(edit_area->clipboard->buf);
	safe_free(edit_area->clipboard);
	safe_free(edit_area->search_list);
	safe_free(edit_area->cmd_line->chars);
	safe_free(edit_area->cmd_line);
	safe_free(edit_area);
	return 0;
}

TEXTAREA *create_textarea(TEXTAREA *textarea, void *parent)
{
	int rows, cols, x0, y0;
	
	textarea->parent = (AREAR *)parent;
	x0 = offset_x(textarea);
	y0 = offset_y(textarea);
	textarea->start_x = 4; //reverse of line number
	textarea->start_y = 1;
	
	draw_boder(textarea);
	textarea->edit_area = (EDITAREA*)calloc(1, sizeof(EDITAREA));
	create_a_editarea(textarea->edit_area, textarea);
	return textarea;
}


int delete_textarea(TEXTAREA *textarea)
{
	release_a_editarea(textarea->edit_area);
	return 0;
}






