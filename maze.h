#pragma once

#include<memory.h>
#include<graphics.h>
#include<time.h>
#include<stdio.h>
#include<cstring>


typedef struct {                     //迷宫单元格类
	int is_visited;					 //单元格是否访问
	int neighbors[4];				 //邻居单元的连通情况,0为该墙未访问，-1为墙，1为连通，生成完毕后，所有的0都变为-1
}maze_cell, **MAZE;


enum vec { VEC_UP, VEC_DOWN, VEC_LEFT, VEC_RIGHT }; //step[n] 得到的是对应方向的向量 
enum gen_mode { NO_GEN, KRUSKAL, PRIM, DFS_GEN, RECUR };  //当前生成迷宫用到的算法标签
enum exp_mode { NO_EXP, DFS_EXP, BFS, ASTAR };			   //当前解决迷宫用到的算法标签

//图片路径名称宏
#define PANNEL_BK  _T("pics\\pannal_bk.png")		   //画板
#define MAZE_PRIM  _T("pics\\maze_prim.png")		   //各种迷宫图片
#define MAZE_KRUS  _T("pics\\maze_krus.png")			
#define MAZE_DFS   _T("pics\\maze_dfs.png")
#define MAZE_RECUR _T("pics\\maze_recur.png")
#define RAW_MAZE   _T("pics\\raw_maze.png")			   //全是墙的迷宫
#define DFS_TAG    _T("pics\\tags\\dfs_tag.png")	   //迷宫标签图片
#define BFS_TAG    _T("pics\\tags\\bfs_tag.png")
#define ASTAR_TAG  _T("pics\\tags\\astar_tag.png")
#define RECRU_TAG  _T("pics\\tags\\recru_tag.png")
#define PRIM_TAG   _T("pics\\tags\\prim_tag.png")
#define KRUS_TAG   _T("pics\\tags\\kruskal_tag.png")

//得到两点之间向量宏
#define POINT_TO_VEC(p1,p2) (vec)((1+p2.y-p1.y+(p2.x-p1.x)*(p2.x-p1.x)*(4+(p2.x-p1.x)))/2)		   


//迷宫相关的全局变量
extern gen_mode GEN_MODE_TAG;		//生成迷宫算法的标签
extern exp_mode EXP_MODE_TAG;		//解决迷宫算法的迷宫

extern IMAGE MAZE_BK;				//全局迷宫背景
extern IMAGE MAZE_PANNEL;			//全局面板背景
extern const POINT MAZE_STEP[4];	//绘制路径时的参数列表
extern const int MAZE_HEIGHT;		//迷宫大小确定
extern const int MAZE_LENGTH;		//防止麻烦
extern const int CELL_SIZE;			//一个方格的像素宽，不能改，raw_maze上的像素宽是固定的改不了的，或者再重画一遍
extern const int MAZE_PADDING_RIGHT;//窗口的右边面板的宽度
extern const POINT MAZE_GOAL;		//终点
extern const POINT MAZE_START;		//起点

extern int PATH_COLOR_DEGREE;		//路径的长度
extern int PATH_COLOR_DEGREE_STATE; //表示路径绘制是在前进还是后退，前进为1，后退是-1


//////////////////////////////////////////////////////////////
//用到的数据结构
//////////////////////////////////////////////////////////////

typedef struct {					 //栈，迷宫大小确定，无需重新分配内存
	POINT *base;
	int   len;
}maze_stack;

void init_maze_stack(maze_stack *STACK);					//初始化栈
void maze_stack_push(maze_stack *MAZE_STACK, POINT p);		//入栈
void maze_stack_pop(maze_stack *MAZE_STACK, POINT* p);		//出栈
void maze_stack_top(maze_stack *MAZE_STACK, POINT* p);		//取栈顶
void destory_maze_stack(maze_stack *MAZE_STACK);			//销毁栈



typedef struct {					//队列，迷宫大小确定，无需重新分配内存
	POINT *base;
	int   rear;
	int   head;
}maze_queue;

void init_maze_queue(maze_queue *MAZE_QUEUE);				//初始化队列
bool queue_maze_empty(maze_queue MAZE_QUEUE);				//判断空队列
void maze_enqueue(maze_queue *MAZE_QUEUE, POINT p);			//入队
void maze_dequeue(maze_queue *MAZE_QUEUE, POINT *p);		//出队
void maze_queue_get_head(maze_queue MAZE_QUEUE, POINT *p);	//取队头
void destory_maze_queue(maze_queue *MAZE_QUEUE);			//销毁队列


typedef struct MAZE_CELLA {
	//该结构用于A*算法
	int dis_from;									//从起点到该点的距离
	int dis_to;										//从该点到终点的估计距离
	int ideal_dis;									//两者之和

	POINT point;									//坐标
	maze_cell maze_copy;							//该点的迷宫信息
	struct MAZE_CELLA *parent;						//父节点的指针

	bool operator == (const  MAZE_CELLA a)const {	//比较估计距离
		return (this->point.x == a.point.x&&this->point.y == a.point.y);
	}
	bool operator >(const  MAZE_CELLA a)const {
		return (this->ideal_dis > a.ideal_dis);
	}
	bool operator <(const  MAZE_CELLA a)const {
		return (this->ideal_dis < a.ideal_dis);
	}

}maze_cell_a, **MAZE_TA;

typedef struct {					//小根堆，估计总距离小的在上面
	MAZE_CELLA **data;				//data从1开始计数
	int count;
	int MaxSize;
}maze_min_heap;

void init_maze_heap(maze_min_heap *mh);						//初始化二叉堆
void maze_heap_adjust_up(maze_min_heap *mh, int k);			//从顶开始调整堆
void maze_heap_adjust_down(maze_min_heap *mh, int k);		//从底开始调整堆
void maze_heap_insert(maze_min_heap *mh, MAZE_CELLA *ma);	//插入节点到堆
void maze_heap_erase(maze_min_heap *mh);					//去掉根节点
bool maze_heap_search(maze_min_heap *mh, MAZE_CELLA ma);	//节点是否在堆中
void destory_maze_heap(maze_min_heap *mh);					//删除二叉堆



typedef struct {					//边类型
	POINT p1, p2;
}maze_edge;

typedef struct {					//存放边的线性表，大小确定，需要随机取值
	maze_edge *base;
	int len;
}maze_edge_list;

void init_maze_edge_list(maze_edge_list *list);						//初始化线性表
void maze_edge_list_push(maze_edge_list *list, maze_edge e);		//插入元素到线性表
void maze_edge_list_pop(maze_edge_list *list, maze_edge *e);		//随机弹出线性表中元素
bool maze_edge_list_empty(maze_edge_list list);						//判断是否为空表
void destory_maze_edge_list(maze_edge_list list);					//销毁一个线性表



typedef struct {				   //并查集，用于kruskal算法
	POINT parent;				   //双亲结点
	int rank;					   //作为根结点时的度
}ufset_node, **maze_ufset;

void  init_maze_ufset(maze_ufset *set);								//初始化一个并查集
POINT get_maze_ufset_root(const maze_ufset set, POINT p);		    //递归查找根节点
void  joint_maze_ufset(const maze_ufset set, POINT r1, POINT r2);	//合并两个等价类
void  destory_maze_ufset(maze_ufset set);						    //销毁一个并查集

//////////////////////////////////////////////////////////////
//绘制迷宫相关
//////////////////////////////////////////////////////////////

#define	BKCOLOR			WHITE			//背景色
#define	BLOCKCOLOR		0x0000AA		//小方块的颜色
#define VISITEDCOLOR	LIGHTGRAY		//表示搜索范围的颜色
#define PATH_COLOR_CYCLE	360         //颜色变化的周期
//#define TRACECOLOR (BGR(RGB(255, 100, (127 + 127*(0-PATH_COLOR_DEGREE_STATE) + PATH_COLOR_DEGREE_STATE*(PATH_COLOR_DEGREE*255 / DEGREE_CYCLE))))) 
#define PATHCOLOR (HSLtoRGB((float)PATH_COLOR_DEGREE,0.5,0.6f)) 
										//路径的颜色
void path_length_inc();					//当路径增长时，控制路径颜色
void path_length_dec();					//当路径减短时，控制路径颜色

enum draw_maze_mode { PATH_MODE, VISITED_MODE, CONNECT_MODE }; //PATH_MODE（路径模式）, VISITED_MODE（已访问模式，表示该块已访问）, CONNECT_MODE（连通模式，在生成迷宫时打通墙壁）

#define MAZE_FRAME 60
void MAZE_DELAY(int fps);				   //根据帧率延迟

void draw_raw_maze();									  //绘制未生成的迷宫
void draw_maze_path(POINT cur, vec vec, draw_maze_mode m);//在cur位置的vec方向的墙上以m模式绘制
void draw_maze_cell(POINT cur);							  //绘制一个单元格，颜色由所调用的函数决定
void draw_connected_wall(maze_edge edge);		          //去除墙e,用于生成迷宫
void draw_maze_block(POINT cur);					      //绘制显示当前位置的小方块，便于展示

void draw_maze_visited(POINT cur, vec vec, draw_maze_mode m); //以m模式绘制当前位置，以及在vec方向的墙上，表示从cur位置向vec方向移动，
void draw_maze_position(POINT cur);							  //用表示访问的颜色覆盖当前点位置（不含墙）

void draw_maze_bk();									  //将迷宫绘制到迷宫展示界面上

void draw_gen_maze_tag();								  //绘制生成算法的标签
void draw_solve_maze_tag();								  //绘制解决迷宫的标签
void save_solved_maze();								  //保存解决迷宫的图片
//////////////////////////////////////////////////////////////
//迷宫创建相关
//////////////////////////////////////////////////////////////


int is_moveable(MAZE MAZE, POINT *p, vec v);		 //判断某个方向是否可以移动 
int get_moveable_choice(MAZE MAZE, POINT *p);		 //得到可移动方向的数量，并记录方向 
vec get_a_rand_direction(MAZE MAZE, POINT *p);		 //返回该单元格一个可以走的方向

void init_maze_system(MAZE *MAZE);					 //初始化迷宫,在每次生成迷宫之前调用
void clear_maze(MAZE MAZE);							 //清空迷宫中数据
void destory_maze(MAZE MAZE);						 //删除一个迷宫
void load_raw_maze();								 //加载生成好的迷宫到画板上
void end_maze_system(MAZE MAZE);					 //结束演示系统，通过按钮退出


void   generate_maze_dfs(MAZE MAZE);										 //DFS生成迷宫
void   move_back_generate_dfs(MAZE MAZE, POINT *cur, maze_stack *MAZE_STACK);//在DFS生成时退到最近的可移动位置，或原点 
void   move_cur_generate_dfs(MAZE MAZE, POINT *cur, maze_stack *MAZE_STACK); //移动当前位置（DFS生成迷宫）



void generate_maze_krus(MAZE MAZE);					//kruskal生成迷宫



void generate_maze_prim(MAZE MAZE);					//prim生成迷宫



void generate_maze_recur(MAZE MAZE);					              //递归分割生成迷宫
void devide_maze(MAZE MAZE, int top, int bottom, int left, int right);//分割迷宫




void explore_maze_bfs(MAZE MAZE);														 //BFS走迷宫
void move_cur_explore_bfs(MAZE MAZE, POINT *cur, maze_queue *MAZE_QUEUE, POINT** father);//移动当前位置（BFS生成迷宫）



void explore_maze_dfs(MAZE MAZE);											//DFS走迷宫
void move_back_explore_dfs(POINT *cur, maze_stack *MAZE_STACK);             //在DFS走迷宫时退到最近的可移动位置，或原点 
void move_cur_explore_dfs(MAZE MAZE, POINT *cur, maze_stack *MAZE_STACK);   //移动当前位置（DFS走迷宫）



void explore_maze_astar(MAZE MAZE);														  //A*走迷宫,使用欧几里得距离
void init_mazea(MAZE MAZE, MAZE_TA MAZEA);												  //初始化A*算法用到MAZEA
void move_cur_astar(MAZE_TA MAZEA, POINT* cur, maze_min_heap *open, maze_min_heap *close);//移动当前位置（A*走迷宫）
void delete_mazea(MAZE_TA MAZEA);														  //删除辅助数据MAZEA