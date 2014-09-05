
/*��С*/
//#define V_SIZE		100
//#define H_SIZE		200

/*����*/
#define UP			0
#define LEFT		1
#define DOWN		2
#define RIGHT		3

/*ת��*/
#define NOTURN		0

#define ULTURN		1
#define DLTURN		2
#define URTURN		3
#define DRTURN		4

#define LUTURN		DRTURN
#define LDTURN		URTURN
#define RUTURN		DLTURN
#define RDTURN		ULTURN

/*����*/
#define HEAD		0
#define BODY		1
#define TAIL		2
#define FOOD		3




/*�ߵ�Ƭ��*/
typedef  struct _Section
{
	unsigned int x:			8;		/*x����*/
	unsigned int y:			8;		/*y����*/
	unsigned int type:		2;		/*Ƭ������*/
	unsigned int dir:		2;		/*����*/
	unsigned int turn:		3;		/*ת��*/

	struct  _Section *next;		/*��һ��*/
	struct  _Section *pre;		/*ǰһ��*/
}Section;

typedef struct _Snake
{		
	unsigned int count:		8;			/*����*/
	Section *head;					/*ͷ*/
	Section *tail;					/*β��*/

	int **flag;//[V_SIZE][H_SIZE];		/*��־*/

	unsigned int level;				//����
	unsigned int score;			//����

}Snake;



int SnakeMoveUp_F(Snake *snake, Section *food, int oridir);
int SnakeMoveDown_F(Snake *snake, Section *food, int oridir);
int SnakeMoveLeft_F(Snake *snake, Section *food, int oridir);
int SnakeMoveRight_F(Snake *snake, Section *food, int oridir);
void CreateSnake(Snake *snake, int h, int w, int level, unsigned int type);


void KillSnake(Snake *snake);
void CreateFood(Section *food, Snake *snake);
