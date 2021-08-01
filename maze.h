#pragma once

#include<memory.h>
#include<graphics.h>
#include<time.h>
#include<stdio.h>
#include<cstring>


typedef struct {                     //�Թ���Ԫ����
	int is_visited;					 //��Ԫ���Ƿ����
	int neighbors[4];				 //�ھӵ�Ԫ����ͨ���,0Ϊ��ǽδ���ʣ�-1Ϊǽ��1Ϊ��ͨ��������Ϻ����е�0����Ϊ-1
}maze_cell, **MAZE;


enum vec { VEC_UP, VEC_DOWN, VEC_LEFT, VEC_RIGHT }; //step[n] �õ����Ƕ�Ӧ��������� 
enum gen_mode { NO_GEN, KRUSKAL, PRIM, DFS_GEN, RECUR };  //��ǰ�����Թ��õ����㷨��ǩ
enum exp_mode { NO_EXP, DFS_EXP, BFS, ASTAR };			   //��ǰ����Թ��õ����㷨��ǩ

//ͼƬ·�����ƺ�
#define PANNEL_BK  _T("pics\\pannal_bk.png")		   //����
#define MAZE_PRIM  _T("pics\\maze_prim.png")		   //�����Թ�ͼƬ
#define MAZE_KRUS  _T("pics\\maze_krus.png")			
#define MAZE_DFS   _T("pics\\maze_dfs.png")
#define MAZE_RECUR _T("pics\\maze_recur.png")
#define RAW_MAZE   _T("pics\\raw_maze.png")			   //ȫ��ǽ���Թ�
#define DFS_TAG    _T("pics\\tags\\dfs_tag.png")	   //�Թ���ǩͼƬ
#define BFS_TAG    _T("pics\\tags\\bfs_tag.png")
#define ASTAR_TAG  _T("pics\\tags\\astar_tag.png")
#define RECRU_TAG  _T("pics\\tags\\recru_tag.png")
#define PRIM_TAG   _T("pics\\tags\\prim_tag.png")
#define KRUS_TAG   _T("pics\\tags\\kruskal_tag.png")

//�õ�����֮��������
#define POINT_TO_VEC(p1,p2) (vec)((1+p2.y-p1.y+(p2.x-p1.x)*(p2.x-p1.x)*(4+(p2.x-p1.x)))/2)		   


//�Թ���ص�ȫ�ֱ���
extern gen_mode GEN_MODE_TAG;		//�����Թ��㷨�ı�ǩ
extern exp_mode EXP_MODE_TAG;		//����Թ��㷨���Թ�

extern IMAGE MAZE_BK;				//ȫ���Թ�����
extern IMAGE MAZE_PANNEL;			//ȫ����屳��
extern const POINT MAZE_STEP[4];	//����·��ʱ�Ĳ����б�
extern const int MAZE_HEIGHT;		//�Թ���Сȷ��
extern const int MAZE_LENGTH;		//��ֹ�鷳
extern const int CELL_SIZE;			//һ����������ؿ����ܸģ�raw_maze�ϵ����ؿ��ǹ̶��ĸĲ��˵ģ��������ػ�һ��
extern const int MAZE_PADDING_RIGHT;//���ڵ��ұ����Ŀ��
extern const POINT MAZE_GOAL;		//�յ�
extern const POINT MAZE_START;		//���

extern int PATH_COLOR_DEGREE;		//·���ĳ���
extern int PATH_COLOR_DEGREE_STATE; //��ʾ·����������ǰ�����Ǻ��ˣ�ǰ��Ϊ1��������-1


//////////////////////////////////////////////////////////////
//�õ������ݽṹ
//////////////////////////////////////////////////////////////

typedef struct {					 //ջ���Թ���Сȷ�����������·����ڴ�
	POINT *base;
	int   len;
}maze_stack;

void init_maze_stack(maze_stack *STACK);					//��ʼ��ջ
void maze_stack_push(maze_stack *MAZE_STACK, POINT p);		//��ջ
void maze_stack_pop(maze_stack *MAZE_STACK, POINT* p);		//��ջ
void maze_stack_top(maze_stack *MAZE_STACK, POINT* p);		//ȡջ��
void destory_maze_stack(maze_stack *MAZE_STACK);			//����ջ



typedef struct {					//���У��Թ���Сȷ�����������·����ڴ�
	POINT *base;
	int   rear;
	int   head;
}maze_queue;

void init_maze_queue(maze_queue *MAZE_QUEUE);				//��ʼ������
bool queue_maze_empty(maze_queue MAZE_QUEUE);				//�жϿն���
void maze_enqueue(maze_queue *MAZE_QUEUE, POINT p);			//���
void maze_dequeue(maze_queue *MAZE_QUEUE, POINT *p);		//����
void maze_queue_get_head(maze_queue MAZE_QUEUE, POINT *p);	//ȡ��ͷ
void destory_maze_queue(maze_queue *MAZE_QUEUE);			//���ٶ���


typedef struct MAZE_CELLA {
	//�ýṹ����A*�㷨
	int dis_from;									//����㵽�õ�ľ���
	int dis_to;										//�Ӹõ㵽�յ�Ĺ��ƾ���
	int ideal_dis;									//����֮��

	POINT point;									//����
	maze_cell maze_copy;							//�õ���Թ���Ϣ
	struct MAZE_CELLA *parent;						//���ڵ��ָ��

	bool operator == (const  MAZE_CELLA a)const {	//�ȽϹ��ƾ���
		return (this->point.x == a.point.x&&this->point.y == a.point.y);
	}
	bool operator >(const  MAZE_CELLA a)const {
		return (this->ideal_dis > a.ideal_dis);
	}
	bool operator <(const  MAZE_CELLA a)const {
		return (this->ideal_dis < a.ideal_dis);
	}

}maze_cell_a, **MAZE_TA;

typedef struct {					//С���ѣ������ܾ���С��������
	MAZE_CELLA **data;				//data��1��ʼ����
	int count;
	int MaxSize;
}maze_min_heap;

void init_maze_heap(maze_min_heap *mh);						//��ʼ�������
void maze_heap_adjust_up(maze_min_heap *mh, int k);			//�Ӷ���ʼ������
void maze_heap_adjust_down(maze_min_heap *mh, int k);		//�ӵ׿�ʼ������
void maze_heap_insert(maze_min_heap *mh, MAZE_CELLA *ma);	//����ڵ㵽��
void maze_heap_erase(maze_min_heap *mh);					//ȥ�����ڵ�
bool maze_heap_search(maze_min_heap *mh, MAZE_CELLA ma);	//�ڵ��Ƿ��ڶ���
void destory_maze_heap(maze_min_heap *mh);					//ɾ�������



typedef struct {					//������
	POINT p1, p2;
}maze_edge;

typedef struct {					//��űߵ����Ա���Сȷ������Ҫ���ȡֵ
	maze_edge *base;
	int len;
}maze_edge_list;

void init_maze_edge_list(maze_edge_list *list);						//��ʼ�����Ա�
void maze_edge_list_push(maze_edge_list *list, maze_edge e);		//����Ԫ�ص����Ա�
void maze_edge_list_pop(maze_edge_list *list, maze_edge *e);		//����������Ա���Ԫ��
bool maze_edge_list_empty(maze_edge_list list);						//�ж��Ƿ�Ϊ�ձ�
void destory_maze_edge_list(maze_edge_list list);					//����һ�����Ա�



typedef struct {				   //���鼯������kruskal�㷨
	POINT parent;				   //˫�׽��
	int rank;					   //��Ϊ�����ʱ�Ķ�
}ufset_node, **maze_ufset;

void  init_maze_ufset(maze_ufset *set);								//��ʼ��һ�����鼯
POINT get_maze_ufset_root(const maze_ufset set, POINT p);		    //�ݹ���Ҹ��ڵ�
void  joint_maze_ufset(const maze_ufset set, POINT r1, POINT r2);	//�ϲ������ȼ���
void  destory_maze_ufset(maze_ufset set);						    //����һ�����鼯

//////////////////////////////////////////////////////////////
//�����Թ����
//////////////////////////////////////////////////////////////

#define	BKCOLOR			WHITE			//����ɫ
#define	BLOCKCOLOR		0x0000AA		//С�������ɫ
#define VISITEDCOLOR	LIGHTGRAY		//��ʾ������Χ����ɫ
#define PATH_COLOR_CYCLE	360         //��ɫ�仯������
//#define TRACECOLOR (BGR(RGB(255, 100, (127 + 127*(0-PATH_COLOR_DEGREE_STATE) + PATH_COLOR_DEGREE_STATE*(PATH_COLOR_DEGREE*255 / DEGREE_CYCLE))))) 
#define PATHCOLOR (HSLtoRGB((float)PATH_COLOR_DEGREE,0.5,0.6f)) 
										//·������ɫ
void path_length_inc();					//��·������ʱ������·����ɫ
void path_length_dec();					//��·������ʱ������·����ɫ

enum draw_maze_mode { PATH_MODE, VISITED_MODE, CONNECT_MODE }; //PATH_MODE��·��ģʽ��, VISITED_MODE���ѷ���ģʽ����ʾ�ÿ��ѷ��ʣ�, CONNECT_MODE����ͨģʽ���������Թ�ʱ��ͨǽ�ڣ�

#define MAZE_FRAME 60
void MAZE_DELAY(int fps);				   //����֡���ӳ�

void draw_raw_maze();									  //����δ���ɵ��Թ�
void draw_maze_path(POINT cur, vec vec, draw_maze_mode m);//��curλ�õ�vec�����ǽ����mģʽ����
void draw_maze_cell(POINT cur);							  //����һ����Ԫ����ɫ�������õĺ�������
void draw_connected_wall(maze_edge edge);		          //ȥ��ǽe,���������Թ�
void draw_maze_block(POINT cur);					      //������ʾ��ǰλ�õ�С���飬����չʾ

void draw_maze_visited(POINT cur, vec vec, draw_maze_mode m); //��mģʽ���Ƶ�ǰλ�ã��Լ���vec�����ǽ�ϣ���ʾ��curλ����vec�����ƶ���
void draw_maze_position(POINT cur);							  //�ñ�ʾ���ʵ���ɫ���ǵ�ǰ��λ�ã�����ǽ��

void draw_maze_bk();									  //���Թ����Ƶ��Թ�չʾ������

void draw_gen_maze_tag();								  //���������㷨�ı�ǩ
void draw_solve_maze_tag();								  //���ƽ���Թ��ı�ǩ
void save_solved_maze();								  //�������Թ���ͼƬ
//////////////////////////////////////////////////////////////
//�Թ��������
//////////////////////////////////////////////////////////////


int is_moveable(MAZE MAZE, POINT *p, vec v);		 //�ж�ĳ�������Ƿ�����ƶ� 
int get_moveable_choice(MAZE MAZE, POINT *p);		 //�õ����ƶ����������������¼���� 
vec get_a_rand_direction(MAZE MAZE, POINT *p);		 //���ظõ�Ԫ��һ�������ߵķ���

void init_maze_system(MAZE *MAZE);					 //��ʼ���Թ�,��ÿ�������Թ�֮ǰ����
void clear_maze(MAZE MAZE);							 //����Թ�������
void destory_maze(MAZE MAZE);						 //ɾ��һ���Թ�
void load_raw_maze();								 //�������ɺõ��Թ���������
void end_maze_system(MAZE MAZE);					 //������ʾϵͳ��ͨ����ť�˳�


void   generate_maze_dfs(MAZE MAZE);										 //DFS�����Թ�
void   move_back_generate_dfs(MAZE MAZE, POINT *cur, maze_stack *MAZE_STACK);//��DFS����ʱ�˵�����Ŀ��ƶ�λ�ã���ԭ�� 
void   move_cur_generate_dfs(MAZE MAZE, POINT *cur, maze_stack *MAZE_STACK); //�ƶ���ǰλ�ã�DFS�����Թ���



void generate_maze_krus(MAZE MAZE);					//kruskal�����Թ�



void generate_maze_prim(MAZE MAZE);					//prim�����Թ�



void generate_maze_recur(MAZE MAZE);					              //�ݹ�ָ������Թ�
void devide_maze(MAZE MAZE, int top, int bottom, int left, int right);//�ָ��Թ�




void explore_maze_bfs(MAZE MAZE);														 //BFS���Թ�
void move_cur_explore_bfs(MAZE MAZE, POINT *cur, maze_queue *MAZE_QUEUE, POINT** father);//�ƶ���ǰλ�ã�BFS�����Թ���



void explore_maze_dfs(MAZE MAZE);											//DFS���Թ�
void move_back_explore_dfs(POINT *cur, maze_stack *MAZE_STACK);             //��DFS���Թ�ʱ�˵�����Ŀ��ƶ�λ�ã���ԭ�� 
void move_cur_explore_dfs(MAZE MAZE, POINT *cur, maze_stack *MAZE_STACK);   //�ƶ���ǰλ�ã�DFS���Թ���



void explore_maze_astar(MAZE MAZE);														  //A*���Թ�,ʹ��ŷ����þ���
void init_mazea(MAZE MAZE, MAZE_TA MAZEA);												  //��ʼ��A*�㷨�õ�MAZEA
void move_cur_astar(MAZE_TA MAZEA, POINT* cur, maze_min_heap *open, maze_min_heap *close);//�ƶ���ǰλ�ã�A*���Թ���
void delete_mazea(MAZE_TA MAZEA);														  //ɾ����������MAZEA