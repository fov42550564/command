
/*大小*/
//#define V_SIZE		100
//#define H_SIZE		200

/*方向*/
#define UP			0
#define LEFT		1
#define DOWN		2
#define RIGHT		3

/*转向*/
#define NOTURN		0

#define ULTURN		1
#define DLTURN		2
#define URTURN		3
#define DRTURN		4

#define LUTURN		DRTURN
#define LDTURN		URTURN
#define RUTURN		DLTURN
#define RDTURN		ULTURN

/*类型*/
#define HEAD		0
#define BODY		1
#define TAIL		2
#define FOOD		3




/*蛇的片段*/
typedef  struct _Section
{
	unsigned int x:			8;		/*x坐标*/
	unsigned int y:			8;		/*y坐标*/
	unsigned int type:		2;		/*片段类型*/
	unsigned int dir:		2;		/*方向*/
	unsigned int turn:		3;		/*转向*/

	struct  _Section *next;		/*后一段*/
	struct  _Section *pre;		/*前一段*/
}Section;

typedef struct _Snake
{		
	unsigned int count:		8;			/*长度*/
	Section *head;					/*头*/
	Section *tail;					/*尾巴*/

	int **flag;//[V_SIZE][H_SIZE];		/*标志*/

	unsigned int level;				//升级
	unsigned int score;			//分数

}Snake;



int SnakeMoveUp_F(Snake *snake, Section *food, int oridir);
int SnakeMoveDown_F(Snake *snake, Section *food, int oridir);
int SnakeMoveLeft_F(Snake *snake, Section *food, int oridir);
int SnakeMoveRight_F(Snake *snake, Section *food, int oridir);
void CreateSnake(Snake *snake, int h, int w, int level, unsigned int type);


void KillSnake(Snake *snake);
void CreateFood(Section *food, Snake *snake);
