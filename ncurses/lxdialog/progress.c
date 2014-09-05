/*
 *  progress.c -- implements the message box and info box
 *
 *  ORIGINAL AUTHOR: Savio Lam (lam836@cs.cuhk.hk)
 *  MODIFIED FOR LINUX KERNEL CONFIG BY: William Roadcap (roadcapw@cfw.com)
 *
 *  This program is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU General Public License
 *  as published by the Free Software Foundation; either version 2
 *  of the License, or (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#include "dialog.h"
int flag = 1;

void eixt_handler(int sig) 
{ 
    flag = 0; 
} 


/*
 * Display a progress
 * 
 */
int dialog_progress (int width, int speed)
{
    int i,j, ret=1,  x, y, key = 0;
    int height = 3;
    WINDOW *dialog;
    struct sigaction act, oldact; 
	
    act.sa_handler = eixt_handler; 
    sigaddset(&act.sa_mask, SIGUSR1);
    act.sa_flags = SA_RESETHAND | SA_NODEFER;
 
    sigaction(SIGUSR1, &act, &oldact); 

    /* center dialog box on screen */
    x = (COLS - width) / 2;
    y = (LINES - height) / 2;

    dialog = newwin (height, width, y, x);
    keypad (dialog, TRUE);

    draw_box (dialog, 0, 0, height, width, dialog_attr, border_attr);
   	wattrset(dialog, A_REVERSE); 
	for (i=1; i<width; i++)
	{
    	mvwaddch(dialog, 1, i, ' ');
	}

    i=1;
   	wattrset(dialog, A_NORMAL); 
    while (i<width)
    {
    	mvwaddch(dialog, 1, i++, ' ');
	if (!flag)
	{
		for (j=i;j<width-1; j++)
		{
			wattrset(dialog, A_REVERSE); 
			mvwaddch(dialog,1, j, ' ');
			wrefresh(dialog);
			usleep(10000);
		}	
		ret = 0;
		break;
	}
	wrefresh(dialog);
	usleep(1000*speed);
    }

    wrefresh (dialog);
    delwin (dialog);
    endwin();
    return ret;
}
