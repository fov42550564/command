#include "GameSnakeAI_F.h"
#include <time.h>

static int height = 0;
static int width = 0;


/*移动*/
static void _move(Snake *snake)
{
	Section *tmp, *ptmp;
	tmp = snake->tail;
	snake->flag[snake->tail->x][snake->tail->y] = 0;

	do 
	{
		ptmp = tmp;
		tmp = tmp->pre;
		
		ptmp->x = tmp->x;
		ptmp->y = tmp->y;
		ptmp->turn = tmp->turn;
		ptmp->dir = tmp->dir;	

	} while (tmp != snake->head);


}


/*方向转上*/
static void _turnUp(Snake *snake, int oridir)
{
	switch (oridir)
	{
	case UP:
		snake->head->turn = NOTURN;
		break;
	case DOWN:
		return;
	case LEFT:
		snake->head->turn = LUTURN;
		break;
	case RIGHT:
		snake->head->turn = RUTURN;
		break;
	default:
		break;
	}
	snake->head->dir = UP;
}

/*方向转下*/
static void _turnDown(Snake *snake, int oridir)
{
	switch (oridir)
	{
	case UP:
		return;
	case DOWN:
		snake->head->turn = NOTURN;
		break;
	case LEFT:
		snake->head->turn = LDTURN;
		break;
	case RIGHT:
		snake->head->turn = RDTURN;
		break;
	default:
		break;
	}
	snake->head->dir = DOWN;
}

/*方向转左*/
static void _turnLeft(Snake *snake, int oridir)
{
	switch (oridir)
	{
	case UP:
		snake->head->turn = ULTURN;
		break;
	case DOWN:
		snake->head->turn = DLTURN;
		break;
	case LEFT:
		snake->head->turn = NOTURN;
		break;
	case RIGHT:
		return;
	default:
		break;
	}
	snake->head->dir = LEFT;
}

/*方向转右*/
static void _turnRight(Snake *snake, int oridir)
{
	switch (oridir)
	{
	case UP:
		snake->head->turn = URTURN;
		break;
	case DOWN:
		snake->head->turn = DRTURN;
		break;
	case LEFT:
		return;
	case RIGHT:
		snake->head->turn = NOTURN;
		break;
	default:
		break;
	}
	snake->head->dir = RIGHT;
}


/*吃*/
static void _eat(Snake * snake, Section *food, int dir)
{
	Section *newPart = (Section *)calloc(1, sizeof(Section));
	newPart->type = HEAD;
	newPart->x = food->x;
	newPart->y = food->y;
	newPart->dir = dir;
	newPart->turn = NOTURN;
	newPart->next = snake->head;
	snake->head->pre = newPart;

	snake->head->type = BODY;
	snake->flag[food->x][food->y] = 1;

	snake->head = newPart;

	snake->count++;
	snake->score += snake->level;
	if (snake->count>20+snake->level)
	{
		snake->level++;
		KillSnake(snake);
		CreateSnake(snake, height, width, 0, 0);
	}

	CreateFood(food, snake);
}


/*生成障碍*/
static void _block(Snake *snake)
{
	unsigned int _x;
	unsigned int _y;
	unsigned int _i;
	srand(time(NULL));
	for (_i = 0; _i < (snake->level<<1); _i++)
	{
		_x = rand() % (height-1);
		_y = (rand() + 412569) % width;
		if (_x==height/2)_x++;
		snake->flag[_x][_y] = 2;
	}
	
}


int SnakeMoveUp_F(Snake *snake, Section *food, int oridir)
{
	if (!snake->head->x)
	{
		return 0;
	}
	if (oridir == DOWN)
		return 1;	
	if (snake->flag[snake->head->x - 1][snake->head->y])
	{
		return 0;
	}


	
	_turnUp(snake, oridir);
	if (food->y == snake->head->y && food->x == snake->head->x - 1)
	{
		_eat(snake, food, UP);
	}
	else
	{
		_move(snake);
		snake->head->x--;
		snake->flag[snake->head->x][snake->head->y] = 1;
	}
	return 1;
}

int SnakeMoveDown_F(Snake *snake, Section *food, int oridir)
{
	if (snake->head->x == height - 1)
	{
		return 0;
	}

	if (oridir == UP)
		return 1;	

	if (snake->flag[snake->head->x + 1][snake->head->y])
	{
		return 0;
	}


	_turnDown(snake, oridir);
	if (food->y == snake->head->y && food->x == snake->head->x + 1)
	{
		_eat(snake, food, DOWN);
	}
	else
	{
		_move(snake);
		snake->head->x++;
		snake->flag[snake->head->x][snake->head->y] = 1;
	}
	return 1;
}

int SnakeMoveLeft_F(Snake *snake, Section *food, int oridir)
{
	if (!snake->head->y)
	{
		return 0;
	}

	if (oridir == RIGHT)
		return 1;	

	if (snake->flag[snake->head->x][snake->head->y - 1])
	{
		return 0;
	}


	_turnLeft(snake, oridir);

	if (food->y == snake->head->y - 1 && food->x == snake->head->x)
	{
		_eat(snake, food, LEFT);
	}
	else
	{
		_move(snake);
		snake->head->y--;
		snake->flag[snake->head->x][snake->head->y] = 1;
	}

	return 1;
}

int SnakeMoveRight_F(Snake *snake, Section *food, int oridir)
{
	if (snake->head->y == width - 1)
	{
		return 0;
	}
	if (oridir == LEFT)
		return 1;	
	if (snake->flag[snake->head->x][snake->head->y + 1])
	{
		return 0;
	}

	_turnRight(snake, oridir);

	if (food->y == snake->head->y + 1 && food->x == snake->head->x)
	{
		_eat(snake, food, RIGHT);
	}
	else
	{
		_move(snake);
		snake->head->y++;
		snake->flag[snake->head->x][snake->head->y] = 1;
	}
	return 1;
}


void CreateSnake(Snake *snake, int h, int w, int level, unsigned int type)
{
	int i;
	height = h;
	width = w;
	if (type)
	{
		snake->flag = (int **)calloc(height, sizeof(int *));
		for (i=0; i<height; i++)
			snake->flag[i]=(int*)calloc(width, sizeof(int));
		snake->level = level;
		snake->score = 0;
	}
	for (i=0; i<height; i++)
		memset(snake->flag[i], 0x00, width*sizeof(int));;
	snake->count = 3;
	_block(snake);


	Section *head, *body, *tail;
	head = (Section *)calloc(1, sizeof(Section));
	body = (Section *)calloc(1, sizeof(Section));
	tail = (Section *)calloc(1, sizeof(Section));

	head->dir = LEFT;
	head->next = body;
	head->turn = NOTURN;
	head->type = HEAD;
	head->x = height/2;
	head->y = width/2-1;
	snake->flag[height/2][width/2-1] = 1;

	body->dir = LEFT;
	body->next = tail;
	body->pre = head;
	body->turn = NOTURN;
	body->type = BODY;
	body->x = height/2;
	body->y = width/2;
	snake->flag[height/2][width/2] = 1;

	tail->dir = LEFT;
	tail->pre = body;
	tail->turn = NOTURN;
	tail->type = TAIL;
	tail->x = height/2;
	tail->y = width/2+1;
	snake->flag[height/2][width/2+1] = 1;

	snake->head = head;
	snake->tail = tail;
}

void KillSnake(Snake *snake)
{
	Section *tmp, *ptmp;
	tmp = snake->head;
	do 
	{
		ptmp = tmp;
		tmp = tmp->next;
		free(ptmp);
	} while (tmp != snake->tail);

	snake->head = NULL;
	snake->tail = NULL;

	free(tmp);
}


void CreateFood(Section *food, Snake *snake)
{
	int x, y;
	srand(time(NULL));
	for (;;)
	{
		x = rand() % height;
		y = (rand() + 412569) % width;

		if (!snake->flag[x][y])
		{
			food->x = x;
			food->y = y;
			return ;
		}
	}
}
