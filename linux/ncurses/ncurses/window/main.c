
#include "common.h"
#include "win.h"
#include "label.h"
#include "button.h"
#include "textbox.h"


void print_keys(WIN *win, int msg, void *param)
{
	mvbprintw(win, 5, 5, "%d   ", msg);
}

int test_win()
{
	WIN win = {10,2,40,20};
	WIN win1 = {2,2,20,10};

	init_all();
	create_win(&win, NULL, "title");
	create_win(&win1, &win, 0);
	update();
	win_driver(&win, print_keys, 0);
	exit_all();
	return 1;
}



int win_proc_button_textbox(WIN *win, int msg, void *param)
{
	int ret;
	char name[10]={0};
	char age[10]={0};
	int *_param = (int *)param;
	
	textbox_driver((void *)_param[0], msg);
	textbox_driver((void *)_param[1], msg);
	ret = button_driver((void *)_param[2], msg);
	if (ret == RET_CLICK)
	{
		get_textbox_value((void *)_param[0], name, 9);
		get_textbox_value((void *)_param[1], age, 9);
		wattrset(MAIN_SCREEN, 0);
		mvwprintw(MAIN_SCREEN, 23, 13, "name:%s  age:%s", name, age);
		wrefresh(MAIN_SCREEN);
		return RET_EXIT;
	}
	return 0;
}


#include "marquee.h"

int test_button_textbox()
{
	BUTTON btn;
	WIN win = {10,2,40,20};
	TEXTBOX namebox = {20,3,10,3,0,0};
	TEXTBOX agebox = {20,8,10,3,0,1};
	MARQUEE marquee = {2,1,30,3, 0,200};
	void * param[] ={&namebox, &agebox,  &btn};
	
	init_all();
	create_win(&win, NULL, 0);
	
	create_marquee(&marquee, &win, "woshifangyunjiang");
	create_simple_label(&win, "name:",  8, 4);
	create_simple_label(&win, "age:", 8, 9);
	
	create_textbox(&namebox, &win, 0);
	create_textbox(&agebox, &win, 0);
	create_button(&btn, &win, " OK ", 15, 16);
	show_button(&btn);
	
	add_focus_child(&win, CT_TEXTBOX, &namebox, -1);
	add_focus_child(&win, CT_TEXTBOX, &agebox, -1);
	add_focus_child(&win, CT_BUTTON, &btn, -1);
	set_focus(&win, 0, TRUE);
	update();
	show_marquee(&marquee);
	win_driver(&win, win_proc_button_textbox, param);
	delete_button(&btn);
	delete_textbox(&namebox);
	delete_textbox(&agebox);
	delete_marquee(&marquee);
	exit_all();
	return 1;
}


#include "list.h"
int win_proc_list(WIN *win, int msg, void *param)
{
	int *_param = (int *)param;
	list_driver((void *)_param[0], msg);
	return 0;
}

int test_list()
{
	LIST list = {3,3,20,10, 0,1};
	WIN win = {10,2,40,20};
	char *items[] = {"1", "2", "3", "4", "5", "6", "7", "8", "9", "10"};
	void * param[] ={&list, 0};
	
	init_all();
	create_win(&win, NULL, 0);
	create_list(&list, &win, items, 10);
	show_list(&list);
	update();
	list.focus = 1;
	win_driver(&win, win_proc_list, param);
	delete_list(&list);
	exit_all();
	return 1;
}

#include "msgbox.h"
int win_proc_msgbox(WIN *win, int msg, void *param)
{
	int *_param = (int *)param;
	int ret = msgbox_driver((void *)_param[0], msg);
	if (ret == RET_YES)
		return RET_EXIT;
	return 0;
}


int test_msgbox()
{
	WIN win = {10, 2,60,30};
	MSGBOX msgbox = {10,2,40,12, 0, 1};
	void * param[] ={&msgbox, 0};
	
	init_all();
	create_win(&win, NULL, 0);
	create_msgbox(&msgbox, &win, "wo shi fang yun jiang, ni ne? ni jiao shen me ming zi", "name");
	show_msgbox(&msgbox);
	update();
	msgbox.focus = 1;
	win_driver(&win, win_proc_msgbox, param);
	delete_msgbox(&msgbox);
	exit_all();
	return 1;
}

#include "progressbar.h"

int test_progressbar()
{
	WIN win = {10,10,40,20};
	PROGRESSBAR bar = {3,3,34,2,0,100};
	PROGRESSBAR bar1 = {3,7,34,2,0,100};

	init_all();
	create_win(&win, NULL, 0);
	create_progressbar(&bar, &win);
	create_progressbar(&bar1, &win);
	show_progressbar(&bar);
	show_progressbar(&bar1);
	win_driver(&win, 0,0);
	delete_progressbar(&bar);
	delete_progressbar(&bar1);
	exit_all();
	return 1;
}


#include "menubox.h"
char *options[] = {
                        "Choice 1",
                        "Choice 2",
                        "Choice 3",
                        "Choice 4",
			   "Choice 5",
			   "Choice 6",
			   "Choice 7",
			   "Choice 8",
			   "Choice 9",
			   "Choice 10",
			   (char *)NULL,
                       };

int win_proc_menubox(WIN *win, int msg, void *param)
{
	int *_param = (int *)param;
	menubox_driver((void *)_param[0], msg);
	return 0;
}

int test_menubox()
{
	WIN win = {10, 2,60,30};
	MENUBOX menubox = {10,2,25,6, 0, 4, 2};
	void * param[] ={&menubox, 0};
	
	init_all();
	create_win(&win, NULL, 0);
	create_menubox(&menubox, &win, options);
	update();
	menubox.focus = 1;
	win_driver(&win, win_proc_menubox, param);
	delete_menubox(&menubox);
	exit_all();
	return 1;
}



#include "slider.h"
int win_proc_slider(WIN *win, int msg, void *param)
{
	int pos;
	int *_param = (int *)param;
	slider_driver((void *)_param[0], msg);
	pos =get_pos((SLIDER *)_param[0]);
	mvwprintw(MAIN_SCREEN, 12, 3, "value:%d ", pos);
	return 0;
}


int test_slider()
{
	WIN win = {10, 2,60,10};
	SLIDER slider = {10,2,25,3, 0, 1};
	void * param[] ={&slider, 0};
	
	init_all();
	create_win(&win, NULL, 0);
	create_slider(&slider, &win);
	update();
	slider.focus = 1;
	win_driver(&win, win_proc_slider, param);
	exit_all();
	return 1;
}



#include "calendar.h"
int win_proc_calendar(WIN *win, int msg, void *param)
{
	int *_param = (int *)param;
	calendar_driver((void *)_param[0], msg);
	return 0;
}


int test_calendar()
{
	WIN win = {10, 2,60,30};
	CALENDAR calendar = {10,2,50,30, 0, 1};
	void * param[] ={&calendar, 0};
	
	init_all();
	create_win(&win, NULL, 0);
	create_calendar(&calendar, &win);
	update();
	calendar.focus = 1;
	win_driver(&win, win_proc_calendar, param);
	exit_all();
	return 1;
}


#include "textarea.h"
int win_proc_textarea(WIN *win, int msg, void *param)
{
	int *_param = (int *)param;
	textarea_driver((void *)_param[0], msg);
	return 0;
}

int test_textarea()
{
	WIN win = {10, 2,50,25};
	TEXTAREA textarea = {10,2,30,10, 0, 1};
	void * param[] ={&textarea, 0};
	
	init_all();
	create_win(&win, NULL, 0);
	create_textarea(&textarea, &win);
	set_cusoron();
	update();
	textarea.focus = 1;
	win_driver(&win, win_proc_textarea, param);
	delete_textarea(&textarea);
	exit_all();
	return 1;
}

#include "eprogressbar.h"
int test_eprogressbar()
{
	WIN win = {10, 2,50,20};
	EPROGRESSBAR eprogressbar = {2,2,40,3, 0, 500};
	EPROGRESSBAR eprogressbar1 = {2,8,40,3, 0, 200};
	
	init_all();
	create_win(&win, NULL, 0);
	create_eprogressbar(&eprogressbar, &win);
	create_eprogressbar(&eprogressbar1, &win);
	show_eprogressbar(&eprogressbar);
	show_eprogressbar(&eprogressbar1);
	sleep(2);
	notice_eprogressbar_stop(&eprogressbar);
	sleep(2);
	notice_eprogressbar_stop(&eprogressbar1);
	delete_eprogressbar(&eprogressbar);
	delete_eprogressbar(&eprogressbar1);
	exit_all();
	return 1;
}


int test_marquee()
{
	WIN win = {10, 2,50,20};
	MARQUEE marquee = {2,2,40,3, 0, 500};
	MARQUEE marquee1 = {2,8,40,3, 0, 200};
	
	init_all();
	create_win(&win, NULL, 0);
	create_marquee(&marquee, &win, "woshifangyunjiang");
	create_marquee(&marquee1, &win, "nishishui");
	show_marquee(&marquee);
	show_marquee(&marquee1);
	win_driver(&win, NULL, NULL);
	delete_marquee(&marquee);
	delete_marquee(&marquee1);
	exit_all();
	return 1;
}




int main()
{
	/**test_win();/**/
	/**test_button_textbox();/**/
	/**test_list();/**/
	/**test_msgbox();/**/
	/**test_progressbar();/**/
	/**test_menubox();/**/
	/**test_textarea();/**/
	/**test_slider();/**/
	/**test_calendar();/**/
	/**/test_textarea();/**/
	/**test_eprogressbar();/**/
	/**test_marquee();/**/
}

