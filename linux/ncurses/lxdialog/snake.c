/*
 * 	file:snake.c
 *	discription:show a snake game
 * author: fangyunjiang
 * date:2011-12-24
 */

#include "dialog.h"
#include "GameSnakeAI_F.h"

#define HEAD_CHAR '@'
#define BODY_CHAR 'o'
#define TAIL_CHAR '.'
#define FOOD_CHAR '$'
#define BLOCK_CHAR '*'

#define MSGBOX_HEIGHT 6
#define MSGBOX_WIDTH  14

#define LEVEL_WIN_HEIGHT 3

static Snake snake;						/*蛇*/
static Section food;					/*食物*/
static int flag = 0;					/*标记*/
static WINDOW *g_win;					/*窗体*/
static WINDOW *g_level_win;			/*级别窗体*/
static int g_height = 0;
static int g_width = 0;
static int g_level = 1;
static int fexit = 0;

static int _createTimer(int level);
static int _clearTimer();

/*画向上的蛇头*/
static int _drawUpHead(WINDOW *win, int x, int y)
{
	mvwaddch(win, y, x,  HEAD_CHAR);
	return 1;
}

/*画向下的头*/
static int _drawDownHead(WINDOW *win, int x, int y)
{
	mvwaddch(win, y, x,  HEAD_CHAR);
	return 1;
}

/*画向右的蛇头*/
static int _drawRightHead(WINDOW *win, int x, int y)
{
	mvwaddch(win, y, x,  HEAD_CHAR);
	return 1;
}

/*画向左的蛇头*/
static int _drawLeftHead(WINDOW *win, int x, int y)
{
	mvwaddch(win, y, x,  HEAD_CHAR);
	return 1;
}

/*画横向的蛇身*/
static int _drawHBody(WINDOW *win, int x, int y)
{	
	mvwaddch(win, y, x,  BODY_CHAR);
	return 1;
}


/*画纵向的蛇身*/
static int _drawVBody(WINDOW *win, int x, int y)
{
	mvwaddch(win, y, x,  BODY_CHAR);
	return 1;
}


/*画下左转的蛇身*/
static int _drawDLTurn(WINDOW *win, int x, int y)
{
	mvwaddch(win, y, x,  BODY_CHAR);
	return 1;
}

/*画下右转的蛇身*/
static int _drawDRTurn(WINDOW *win, int x, int y)
{
	mvwaddch(win, y, x,  BODY_CHAR);
	return 1;
}

/*画上左转的蛇身*/
static int _drawULTurn(WINDOW *win, int x, int y)
{
	mvwaddch(win, y, x,  BODY_CHAR);
	return 1;
}

/*画上右转的蛇身*/
static int _drawURTurn(WINDOW *win, int x, int y)
{
	mvwaddch(win, y, x,  BODY_CHAR);
	return 1;
}

/*画向上的蛇尾*/
static int _drawUpTail(WINDOW *win, int x, int y)
{
	mvwaddch(win, y, x,  TAIL_CHAR);
	return 1;
}

/*画向下的蛇尾*/
static int _drawDownTail(WINDOW *win, int x, int y)
{
	mvwaddch(win, y, x,  TAIL_CHAR);
	return 1;
}

/*画向左的蛇尾*/
static int _drawLeftTail(WINDOW *win, int x, int y)
{
	mvwaddch(win, y, x,  TAIL_CHAR);
	return 1;
}


/*画向右的蛇尾*/
static int _drawRightTail(WINDOW *win, int x, int y)
{
	mvwaddch(win, y, x,  TAIL_CHAR);
	return 1;
}


/*画食物*/
static int _drawFood(WINDOW *win, int x, int y)
{
	mvwaddch(win, y, x,  FOOD_CHAR);
	return 1;
}

/*画墙*/
static int _drawWall(int x, int y)
{
	draw_box (stdscr, y-1, x-1, g_height+2, g_width+2, dialog_attr, border_attr);
	refresh();
	return 1;
}

/*画障碍*/
static int _drawBlock(WINDOW *win, unsigned int x, unsigned int y)
{
	mvwaddch(win, y, x,  BLOCK_CHAR);
	return 1;
}

/*画所有障碍*/
static void _drawAllBlocks(WINDOW *win)
{
	unsigned int _x;
	unsigned int _y;
	for (_x=0; _x<g_height;_x++)
		for(_y=0; _y<g_width; _y++)
			if (snake.flag[_x][_y] == 2)
				_drawBlock(win, _y, _x);
	
}

/*画片段*/
static int _drawSection(WINDOW *win, Section *part)
{
	switch (part->type)
	{
	case HEAD:
		switch (part->dir)
		{
		case UP:
			_drawUpHead(win, part->y, part->x);
			break;
		case DOWN:
			_drawDownHead(win, part->y, part->x);
			break;
		case 1:
			_drawLeftHead(win, part->y, part->x);
			break;
		case RIGHT:
			_drawRightHead(win, part->y, part->x);
			break;
		default:
			break;
		}	
		break;
	case BODY:
		if (part->turn == NOTURN)		
		{			
			if (part->dir == LEFT || part->dir == RIGHT)			
			{					
				_drawHBody(win, part->y, part->x);			
			}			
			else			
			{				
				_drawVBody(win, part->y, part->x);			
			}		
		}
		else
		{
			switch (part->turn)
			{
			case ULTURN:
				_drawDLTurn(win, part->y, part->x);
				break;
			case URTURN:
				_drawDRTurn(win, part->y, part->x);
				break;
			case DLTURN:
				_drawULTurn(win, part->y, part->x);
				break;
			case DRTURN:
				_drawURTurn(win, part->y, part->x);
				break;
			default:
				break;
			}
		}
		break;
	case TAIL:
		switch (part->dir)
		{
		case UP:
			_drawUpTail(win, part->y, part->x);
			break;
		case DOWN:
			_drawDownTail(win, part->y, part->x);
			break;
		case LEFT:
			_drawLeftTail(win, part->y, part->x);
			break;
		case RIGHT:
			_drawRightTail(win, part->y, part->x);
			break;
		default:
			break;
		}
		break;
	case FOOD:
		_drawFood(win, part->y, part->x);
		break;
	default:
		break;
	}

	return 1;
}



/*画蛇和食物*/
static int _drawSnakeAndFood(WINDOW *win)
{
	Section *tmp = snake.head;
	_drawSection(win, tmp);
	do 	
	{		
		tmp = tmp->next;		
		_drawSection(win, tmp);			
	} while (tmp != snake.tail);

	_drawSection(win, &food);
	return 1;
}

/*画分数*/
static int _drawScore()
{
	clear_box(g_level_win, 1, 1, 1, g_width-3);
	mvwprintw(g_level_win, 1, 3, "Score: ");
	//wattron(g_level_win, A_BLINK);
	wprintw(g_level_win, "%d" , snake.score);
	//wattroff(g_level_win, A_BLINK);
	wprintw(g_level_win,  "     Level: ", snake.level);
	//wattron(g_level_win, A_BLINK);
	wprintw(g_level_win, "%d" , snake.level);
	//wattroff(g_level_win, A_BLINK);
	wrefresh(g_level_win);
	return 1;
}


/*画整个地图*/
static int _drawMap(WINDOW *win)
{
	clear_box(win, 0, 0, g_height, g_width);
	_drawSnakeAndFood(win);
	_drawAllBlocks(win);
	 _drawScore();
	return 1;
}

/*重画整个地图*/
int repaint(WINDOW *win)
{
	clear_box(win, 0, 0, g_height, g_width);
	_drawSnakeAndFood(win);
	_drawAllBlocks(win);
	 _drawScore();
	wrefresh(win);
	return 1;
}

static int _clearTimer();
/*结束画面*/
int _showEndWin(WINDOW *win)
{
	int x, y;
	
	y= g_height/2;
	x = g_width/2;
	fexit = 1;
	flag=0;
	
	 _clearTimer();
	over_draw_box(win,  y-MSGBOX_HEIGHT/2, x-MSGBOX_WIDTH/2, MSGBOX_HEIGHT, MSGBOX_WIDTH, 
		dialog_attr, border_attr);
	mvwaddstr(win,  y-1, x-5, "Game Over");
	print_button (win, "  Ok  ", y+1, x - 4, TRUE);
	wrefresh(win);
	return 1;
};

/*暂停画面*/
int _showPauseWin(WINDOW *win)
{
	int x, y;
	
	y= g_height/2;
	x = g_width/2;

	flag=0;
	
	over_draw_box(win,  y-MSGBOX_HEIGHT/2, x-MSGBOX_WIDTH/2, MSGBOX_HEIGHT, MSGBOX_WIDTH, 
		dialog_attr, border_attr);
	mvwaddstr(win,  y-1, x-5, "Continue");
	print_button (win, "  Ok  ", y+1, x - 4, TRUE);
	wrefresh(win);
	return 1;
};


/*上移*/
static int _moveUp(WINDOW *win)
{
	int dir = snake.head->dir;
	if (SnakeMoveUp_F(&snake, &food, dir))
	{
		repaint(win);
	}
	else
	{
		_showEndWin(win);
	}
	return 1;
}

/*下移*/
static int _moveDown(WINDOW *win)
{
	int dir = snake.head->dir;
	if (SnakeMoveDown_F(&snake, &food, dir))
	{
		repaint(win);
	}
	else
	{
		_showEndWin(win);
	}
	return 1;
}


/*上移*/
static int _moveLeft(WINDOW *win)
{
	int dir = snake.head->dir;
	if (SnakeMoveLeft_F(&snake, &food, dir))
	{
		repaint(win);
	}
	else
	{
		_showEndWin(win);
	}
	return 1;
}


/*上移*/
static int _moveRight(WINDOW *win)
{
	int dir = snake.head->dir;
	if (SnakeMoveRight_F(&snake, &food, dir))
	{
		repaint(win);
	}
	else
	{
		_showEndWin(win);
	}
	return 1;
}

/*蛇的移动*/
static int _snakeGoGoGo(WINDOW *win)
{
	int dir = snake.head->dir;
	switch (dir)
	{
	case UP:
		_moveUp(win);
		break;
	case DOWN:
		_moveDown(win);
		break;
	case LEFT:
		_moveLeft(win);
		break;
	case RIGHT:
		_moveRight(win);
		break;
	default:
		break;
	}
	return 1;
}


/*计时器过程函数*/
static void my_alarm_handler(int sig)
{
	if (flag)
		_snakeGoGoGo(g_win);
	if (g_level<snake.level)
	{
		g_level=snake.level;
		_createTimer(g_level);
	}
}


/*创建并启动计时器*/
static int _createTimer(int level)
{
	struct itimerval t;
	t.it_interval.tv_usec = (10-level)*100000;
	t.it_interval.tv_sec = 0;
	t.it_value.tv_usec = (10-level)*100000;
	t.it_value.tv_sec = 0;
	if( setitimer( ITIMER_REAL, &t, NULL) < 0 )
		return 1;
	signal( SIGALRM, my_alarm_handler );
	return 0;
}


/*清除计时器*/
static int _clearTimer()
{
	struct itimerval t;
	t.it_interval.tv_usec = 0;
	t.it_interval.tv_sec = 0;
	t.it_value.tv_usec = 0;
	t.it_value.tv_sec = 0;
	setitimer( ITIMER_REAL, &t, NULL);
	return 1;
}



/*初始化*/
static int _initSnake(WINDOW *win)
{
	CreateSnake(&snake, g_height ,g_width, g_level, 1);
	food.type = FOOD;
	CreateFood(&food, &snake);
	flag = 1;
	_drawMap(win);
	_createTimer(g_level);
	return 1;
}


/*清理*/
static int _endSnake()
{
	KillSnake(&snake);
	return 1;
}


/**/
int dialog_snake ( int height, int width, int level)
{
	int i,j, ret=1,  x, y, key = 0;
	int count, c, on=1;
	WINDOW *dialog;
	 
	/* center dialog box on screen */
	x = (COLS - width) / 2;
	y = (LINES - height) / 2;
	g_height = height;
	g_width = width;
	g_level = level;
	    
	dialog = newwin (height, width, y, x);
	g_level_win = newwin (LEVEL_WIN_HEIGHT, width+2, y+height+1, x-1);
	g_win = dialog;
	keypad (dialog, TRUE);
	
	draw_box(g_level_win,0,0,LEVEL_WIN_HEIGHT,width+2, dialog_attr, border_attr);
	
	_drawWall(x, y);
	_initSnake(dialog);
	
	while(on && (c = wgetch(dialog)) != KEY_F(1))
	{     
		switch (c)
		{
		case KEY_UP:
			if (flag)_moveUp(dialog);
			break;

		case KEY_DOWN:
			if (flag)_moveDown(dialog);	
			break;

		case KEY_LEFT:
			if (flag)_moveLeft(dialog);
			break;

		case KEY_RIGHT:
			if (flag)_moveRight(dialog);
			break;
			
		case '\n':
			if (!flag)
			{
				if (fexit)
					on=0;
				else
					flag = 1;
			}
			break;

		case ' ': //暂停
				if (flag)
					_showPauseWin(dialog);
			break; 
		default:
			break;
		}
	}

	_endSnake();
	delwin (dialog);
	endwin();
    return ret;
}

