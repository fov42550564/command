/*
 * =====================================================================================
 *
 *	Filename:  win.c
 *
 * 	Description:  win c file
 *
 *	Version:  1.0
 *	Created:  12/28/11 20:29:34
 *	Revision:  none
 * 	Compiler:  gcc
 *
 *	Author:  fangyunjiang (fang), 42550564@qq.com
 *	Company:  star-net
 *
 * =====================================================================================
 */
 #include "win.h"
 #include "button.h"
 #include "textbox.h"
 #include "list.h"


WIN *create_win(WIN *win, void *parent, char *title)
{
	int x0, y0, len, n,i;
	
	win->parent = (AREAR *)parent;
	win->active_child_id = 0;
	win->last_child_id = -1;
	
	draw_boder(win);
	if (title)
	{
		x0 = offset_x(win);
		y0 = offset_y(win);
		len = strlen(title);
		n = (win->w - len)/2;
		wmove(MAIN_SCREEN, y0, x0);
		wattrset (MAIN_SCREEN, A_REVERSE);
		for (i=0; i<n; i++)
			waddch(MAIN_SCREEN, ' ');
		waddstr(MAIN_SCREEN, title);
		for (i=0; i<n+1; i++)
			waddch(MAIN_SCREEN, ' ');
	}
	return win;
}

 WIN *create_win_ex(WIN *win, void *parent, int x, int y, int w, int h)
 {
	win->x = x;
	win->y = y;
	win->w = w;
	win->h = h;
	win->parent = (AREAR *)parent;
 	win->active_child_id = 0;
	win->last_child_id = -1;
	
	draw_boder(win);
	
	return win;
 }


int win_driver(WIN *win, void *proc, void *param)
{
	int ch;
	//if (!win->focus)
	//	return RET_ERROR;
	
	while ((ch=wgetch(MAIN_SCREEN))!=VK_EXIT)
	{
		if (ch==VK_TAB && win->last_child_id>=0)
		{
			set_focus(win, win->active_child_id, FALSE);
			win->active_child_id++;
			if (win->active_child_id>win->last_child_id)
				win->active_child_id = 0;
			set_focus(win, win->active_child_id, TRUE);
		}
		else if (NULL== proc);
		else if (RET_EXIT == ((WIN_CALL_PROC)proc)(win, ch, param))
			break;
	}
	return 0;
}



int set_focus(WIN *win, int id, bool focus)
{
	win->active_child_id = id;
	switch (win->focus_childs[id].ctype)
	{
		case CT_WINDOW:
			set_cusoroff();
			((WIN *)(win->focus_childs[id].ptr))->focus = focus;
			break;
		case CT_BUTTON:
			set_cusoroff();
			((BUTTON*)(win->focus_childs[id].ptr))->focus = focus;
			show_button((BUTTON*)(win->focus_childs[id].ptr));
			break;
		case CT_TEXTBOX:
			set_cusoron();
			((TEXTBOX*)(win->focus_childs[id].ptr))->focus = focus;
			form_driver(((TEXTBOX*)(win->focus_childs[id].ptr))->form, REQ_END_LINE);
			break;
		case CT_LIST:
			set_cusoroff();
			((LIST*)(win->focus_childs[id].ptr))->focus = focus;
		default:
			break;
	}
	return 0;
}



int add_focus_child(WIN *win, CHILD_TYPE ctype, void *ptr, int index)
{
	int i;
	if ( win->last_child_id >= MAX_CHILD_COUNT-1)
		return RET_ERROR;
		
	if (index < 0)
		index = win->last_child_id+1;

	for (i=win->last_child_id; i>=index; i--)
	{
		win->focus_childs[i+1].ctype = win->focus_childs[i].ctype;
		win->focus_childs[i+1].ptr = win->focus_childs[i].ptr;
	}
	
	win->focus_childs[index].ctype = ctype;
	win->focus_childs[index].ptr = ptr;
	
	win->last_child_id++;
	return RET_OK;
}


int del_focus_child(WIN *win, int index)
{
	int i;
	if (win->last_child_id==-1)
		return RET_ERROR;
	if (index < 0)
		index = win->last_child_id;

	for (i=index; i<win->last_child_id; i++)
	{
		win->focus_childs[i].ctype = win->focus_childs[i+1].ctype;
		win->focus_childs[i].ptr = win->focus_childs[i+1].ptr;
	}
	
	win->last_child_id--;
	return RET_OK;
}