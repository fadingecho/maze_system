#include"pch.h"
#include"maze.h"


IMAGE MAZE_BK;	
IMAGE MAZE_PANNEL;

gen_mode GEN_MODE_TAG;
exp_mode EXP_MODE_TAG;

const int MAZE_HEIGHT = 45;
const int MAZE_LENGTH = 45;
const int CELL_SIZE = 12;
const int MAZE_PADDING_RIGHT = 300;
const POINT MAZE_GOAL = { MAZE_LENGTH-1,MAZE_HEIGHT -1};
const POINT MAZE_START = { 0,0 };
int PATH_COLOR_DEGREE;
int PATH_COLOR_DEGREE_STATE;

const POINT MAZE_STEP[4] = { {0,-1},{0,1},{-1,0},{1,0} };

/////////////////////////////////////////////////////////////////////////////////////////////

//栈操作
void init_maze_stack(maze_stack* STACK) {
	STACK->base = (POINT*)malloc(sizeof(POINT)*MAZE_HEIGHT*MAZE_LENGTH);
	//分配内存失败
	if (!STACK->base) exit(-1);
	STACK->len = 0;
}
void maze_stack_push(maze_stack *MAZE_STACK, POINT p) {
	//若栈溢出
	if (MAZE_STACK->len >= MAZE_HEIGHT * MAZE_LENGTH) exit(-2);
	MAZE_STACK->base[MAZE_STACK->len] = p;
	++MAZE_STACK->len;
}
void maze_stack_pop(maze_stack *MAZE_STACK, POINT* p) {
	//若栈空
	if (MAZE_STACK->len == 0) exit(-3);
	if (p != NULL)*p = MAZE_STACK->base[MAZE_STACK->len - 1];
	--MAZE_STACK->len;
}
void maze_stack_top(maze_stack *MAZE_STACK, POINT* p) {
	//若栈空
	if (MAZE_STACK->len == 0) exit(-66);
	*p = MAZE_STACK->base[MAZE_STACK->len - 1];
}
void destory_maze_stack(maze_stack *MAZE_STACK) {
	free(MAZE_STACK->base);
}

//队列操作(循环队列)
void init_maze_queue(maze_queue* q) {
	q->base = (POINT*)malloc(sizeof(POINT)*MAZE_HEIGHT*MAZE_LENGTH);
	//分配内存失败
	if (!q->base) exit(-5);
	q->rear = q->head = 0;
}

void maze_enqueue(maze_queue *q, POINT p) {
	//若队满
	if ((q->rear + 1) % (MAZE_HEIGHT*MAZE_LENGTH) == q->head) exit(-6);
	q->base[q->rear] = p;
	q->rear = (q->rear + 1) % (MAZE_HEIGHT*MAZE_LENGTH);
}

void maze_dequeue(maze_queue *q, POINT *p) {
	//若队空
	if (queue_maze_empty(*q)) exit(-7);
	*p = q->base[q->head];
	q->head = (q->head + 1) % (MAZE_LENGTH*MAZE_HEIGHT);
}

void maze_queue_get_head(maze_queue q, POINT *p) {
	//若队空
	if (queue_maze_empty(q)) exit(-8);
	*p = q.base[q.head];
}

void destory_maze_queue(maze_queue *q) {
	if (q == NULL) free(q->base);
}

bool queue_maze_empty(maze_queue q) {
	if (q.head == q.rear) return 1;
	return 0;
}

void init_maze_edge_list(maze_edge_list *list) {
	list->base = (maze_edge*)malloc(sizeof(maze_edge)*(MAZE_HEIGHT*MAZE_LENGTH * 2 - MAZE_LENGTH - MAZE_HEIGHT));
	if (!list->base) exit(-9);
	list->len = 0;
}
void maze_edge_list_push(maze_edge_list *list, maze_edge e) {
	if (!list) exit(-1);
	list->base[list->len++] = e;
}
void maze_edge_list_pop(maze_edge_list *list, maze_edge *e) {
	if (list->len == 0) exit(-10);
	int position = rand() % list->len;
	*e = list->base[position];
	for (int i = position; i < list->len - 1; i++) {
		list->base[i] = list->base[i + 1];
	}
	--list->len;
}
bool maze_edge_list_empty(maze_edge_list list) {
	if (list.len == 0) return true;
	return false;
}
void destory_maze_edge_list(maze_edge_list list) {
	if (list.base) free(list.base);
}

//初始化一个并查集
void init_maze_ufset(maze_ufset *set) {
	//分配行指针
	(*set) = (ufset_node**)malloc(sizeof(ufset_node*)*MAZE_LENGTH);
	if (!*set) exit(-1);
	for (int i = 0; i < MAZE_LENGTH; i++) {
		//分配列指针
		(*set)[i] = (ufset_node*)malloc(sizeof(ufset_node)*MAZE_HEIGHT);
		if (!(*set)[i]) exit(-1);
		for (int j = 0; j < MAZE_HEIGHT; j++) {
			(*set)[i][j].parent = { i,j };
			(*set)[i][j].rank = 0;
		}
	}
}
//递归查找根节点
POINT get_maze_ufset_root(const maze_ufset set, POINT p) {
	if (set[p.x][p.y].parent.x == p.x && set[p.x][p.y].parent.y == p.y) return p;
	else return get_maze_ufset_root(set, set[p.x][p.y].parent);
}
//合并两个等价类
void joint_maze_ufset(const maze_ufset set, POINT r1, POINT r2) {
	if (r1.x == r2.x && r1.y == r2.y) return;
	if (set[r1.x][r1.y].rank > set[r2.x][r2.y].rank) {
		set[r2.x][r2.y].parent = r1;
		set[r1.x][r1.y].rank += set[r2.x][r2.y].rank;
	}
	else {
		set[r1.x][r1.y].parent = r2;
		set[r2.x][r2.y].rank += set[r1.x][r1.y].rank;
	}
}

void destory_maze_ufset(maze_ufset set) {
	if (set == NULL) exit(-1);
	for (int i = 0; i < MAZE_LENGTH; i++) {
		if (!set[i])exit(-1);
		free(set[i]);
	}
	free(set);
}
void init_maze_heap(maze_min_heap *mh) {
	mh->count = 0;
	mh->MaxSize = MAZE_HEIGHT * MAZE_LENGTH;
	mh->data = (MAZE_CELLA **)malloc(sizeof(MAZE_CELLA*)*mh->MaxSize);
	if (mh->data == NULL) exit(-1);
}

void destory_maze_heap(maze_min_heap *mh) {
	if (mh == NULL) exit(-1);
	free(mh->data);
}

void  maze_heap_adjust_up(maze_min_heap *mh, int k) {
	while (1 < k && *(mh->data[k / 2]) > *(mh->data[k])) {
		MAZE_CELLA *temp = mh->data[k / 2];
		mh->data[k / 2] = mh->data[k];
		mh->data[k] = temp;
		k /= 2;
	}
}

void maze_heap_adjust_down(maze_min_heap *mh, int k) {
	while (k * 2 <= mh->count) {
		int j = k * 2;
		//找当前层最小的元素
		if (j + 1 <= mh->count&&*(mh->data[j + 1]) < *(mh->data[j]))
			++j;
		if (*(mh->data[k]) > *(mh->data[j])) {
			MAZE_CELLA *temp = mh->data[k];
			mh->data[k] = mh->data[j];
			mh->data[j] = temp;
		}
		k = j;
	}
}

void maze_heap_insert(maze_min_heap *mh, MAZE_CELLA *ma) {
	mh->data[++(mh->count)] = ma;
	maze_heap_adjust_up(mh, mh->count);
}

void maze_heap_erase(maze_min_heap *mh) {
	MAZE_CELLA* res = mh->data[1];
	mh->data[1] = mh->data[(mh->count)--];
	maze_heap_adjust_down(mh, 1);
	return;
}

bool maze_heap_search(maze_min_heap *mh, MAZE_CELLA ma) {
	for (int i = 1; i <= mh->count; i++) {
		if (ma == *(mh->data[i]))
			return 1;
	}
	return 0;
}

////////////////////////////////////////////////////////////////////////////////////////

void path_length_inc() {

	++PATH_COLOR_DEGREE;
	if (PATH_COLOR_DEGREE == PATH_COLOR_CYCLE + 1 && PATH_COLOR_DEGREE_STATE == 1) {
		PATH_COLOR_DEGREE = 1;
		PATH_COLOR_DEGREE_STATE = -1;
	}
	if (PATH_COLOR_DEGREE == PATH_COLOR_CYCLE && PATH_COLOR_DEGREE_STATE == -1) {
		PATH_COLOR_DEGREE = 0;
		PATH_COLOR_DEGREE_STATE = 1;
	}
}
void path_length_dec() {

	--PATH_COLOR_DEGREE;
	if (PATH_COLOR_DEGREE == -1 && PATH_COLOR_DEGREE_STATE == 1) {
		PATH_COLOR_DEGREE = PATH_COLOR_CYCLE - 1;
		PATH_COLOR_DEGREE_STATE = -1;
	}
	if (PATH_COLOR_DEGREE == 0 && PATH_COLOR_DEGREE_STATE == -1) {
		PATH_COLOR_DEGREE = PATH_COLOR_CYCLE;
		PATH_COLOR_DEGREE_STATE = 1;
	}
}

void MAZE_DELAY(int fps) {		//根据帧率延迟

	clock_t last, present;		//初始化时钟
	last = clock();
	double interval = (double)1 / fps;

	present = clock();			//通过延时来控制刷新率
	while ((present - last) < (clock_t)CLOCKS_PER_SEC*(interval)) present = clock();
	last = present;
}

//绘制所有单元格
void draw_raw_maze() {
	SetWorkingImage(NULL);
	loadimage(&MAZE_BK, RAW_MAZE);
	putimage(0, 0, &MAZE_BK);
	FlushBatchDraw();
}
//绘制指定色到墙壁上，打通墙壁或者在走迷宫时显示已搜索范围
void draw_maze_path(POINT cur, vec vec, draw_maze_mode m) {

	SetWorkingImage(&MAZE_BK);
	if (m == CONNECT_MODE)setlinecolor(BKCOLOR);
	else if (m == PATH_MODE)setlinecolor(PATHCOLOR);
	else setlinecolor(VISITEDCOLOR);
	switch (vec) {
	case VEC_DOWN:
		line(cur.x*CELL_SIZE + 1, (cur.y + 1)*CELL_SIZE, (cur.x + 1)*CELL_SIZE - 1, (cur.y + 1)*CELL_SIZE);
		break;
	case VEC_UP:
		line(cur.x*CELL_SIZE + 1, cur.y*CELL_SIZE, (cur.x + 1)*CELL_SIZE - 1, (cur.y)*CELL_SIZE);
		break;
	case VEC_RIGHT:
		line((cur.x + 1)*CELL_SIZE, cur.y*CELL_SIZE + 1, (cur.x + 1)*CELL_SIZE, (cur.y + 1)*CELL_SIZE - 1);
		break;
	case VEC_LEFT:
		line(cur.x*CELL_SIZE, cur.y*CELL_SIZE + 1, (cur.x)*CELL_SIZE, (cur.y + 1)*CELL_SIZE - 1);
		break;
	}
	SetWorkingImage(NULL);
}
//绘制当前点位置
void draw_maze_position(POINT cur) {

	setfillcolor(VISITEDCOLOR);
	draw_maze_cell(cur);
	draw_maze_bk();
	draw_maze_block(cur);
	FlushBatchDraw();
}
//显示搜索位置
void draw_maze_visited(POINT cur, vec v, draw_maze_mode m) {
	SetWorkingImage(&MAZE_BK);
	//绘制背景
	if (m == PATH_MODE)setfillcolor(PATHCOLOR);
	else if (m == VISITED_MODE) setfillcolor(VISITEDCOLOR);
	else setfillcolor(BKCOLOR);
	SetWorkingImage(NULL);

	draw_maze_cell(cur);
	draw_maze_path(cur, v, m);
	draw_maze_bk();
	draw_maze_block(cur);
	FlushBatchDraw();
}
void draw_maze_bk() {
	SetWorkingImage(NULL);
	putimage(0, 0, &MAZE_BK);
	FlushBatchDraw();
}
void draw_maze_block(POINT p) {
	SetWorkingImage(NULL);
	//绘制方块
	setfillcolor(BLOCKCOLOR);
	solidrectangle(p.x * CELL_SIZE + 2, p.y * CELL_SIZE + 2,
		p.x * CELL_SIZE + CELL_SIZE * 1 - 2, p.y * CELL_SIZE + CELL_SIZE * 1 - 2);
}
void draw_maze_cell(POINT cur) {
	SetWorkingImage(&MAZE_BK);
	solidrectangle(cur.x * CELL_SIZE + 1, cur.y * CELL_SIZE + 1,
		cur.x * CELL_SIZE + CELL_SIZE * 1 - 1, cur.y * CELL_SIZE + CELL_SIZE * 1 - 1);
	SetWorkingImage(NULL);
}
//根据边来画路径
void draw_connected_wall(maze_edge e) {
	//printf("(%d %d) (%d %d)\n", e.p1.x, e.p1.y, e.p2.x, e.p2.y);
	SetWorkingImage(&MAZE_BK);
	setlinecolor(BKCOLOR);
	vec v = POINT_TO_VEC((e.p1), (e.p2));
	POINT cur = e.p1;
	switch (v) {
	case VEC_DOWN:
		line(cur.x*CELL_SIZE + 1, (cur.y + 1)*CELL_SIZE, (cur.x + 1)*CELL_SIZE - 1, (cur.y + 1)*CELL_SIZE);
		break;
	case VEC_UP:
		line(cur.x*CELL_SIZE + 1, cur.y*CELL_SIZE, (cur.x + 1)*CELL_SIZE - 1, (cur.y)*CELL_SIZE);
		break;
	case VEC_RIGHT:
		line((cur.x + 1)*CELL_SIZE, cur.y*CELL_SIZE + 1, (cur.x + 1)*CELL_SIZE, (cur.y + 1)*CELL_SIZE - 1);
		break;
	case VEC_LEFT:
		line(cur.x*CELL_SIZE, cur.y*CELL_SIZE + 1, (cur.x)*CELL_SIZE, (cur.y + 1)*CELL_SIZE - 1);
		break;
	}
	SetWorkingImage(NULL);
	//system("pause");
}
void draw_gen_maze_tag() {
	//更新面板
	loadimage(&MAZE_PANNEL, PANNEL_BK);
	IMAGE tag;
	switch (GEN_MODE_TAG) {
	case NO_GEN:
		exit(-1);
		break;
	case KRUSKAL:
		loadimage(&tag, KRUS_TAG);
		break;
	case PRIM:
		loadimage(&tag, PRIM_TAG);
		break;
	case DFS_GEN:
		loadimage(&tag, DFS_TAG);
		break;
	case RECUR:
		loadimage(&tag, RECRU_TAG);
		break;
	default:
		break;
	}

	SetWorkingImage(&MAZE_PANNEL);
	putimage(160, 100, &tag);
	SetWorkingImage(NULL);
	putimage(MAZE_HEIGHT*CELL_SIZE, 0, &MAZE_PANNEL);
	FlushBatchDraw();
	return;
}
void draw_solve_maze_tag() {
	IMAGE tag;
	switch (EXP_MODE_TAG) {
	case NO_EXP:
		exit(-1);
		break;
	case DFS_EXP:
		loadimage(&tag, DFS_TAG);
		break;
	case BFS:
		loadimage(&tag, BFS_TAG);
		break;
	case ASTAR:
		loadimage(&tag, ASTAR_TAG);
		break;
	default:
		break;
	}

	SetWorkingImage(&MAZE_PANNEL);
	putimage(160, 145, &tag);
	SetWorkingImage(NULL);
	putimage(MAZE_HEIGHT*CELL_SIZE, 0, &MAZE_PANNEL);
	FlushBatchDraw();
	return;
}




void save_solved_maze() {
	char *d,*name; 
 	name = d = (char*)malloc(sizeof(char) * 60);

	//写入路径
	const char *index = "pics\\solved_maze\\";
	while (*d++ = *index++);
	--d;

	//写入生成信息
	const char * gen;
	switch (GEN_MODE_TAG)
	{
	case NO_GEN:
		exit(-1);
		break;
	case KRUSKAL:
		gen = "kruskalmaze";
		while (*d++ = *gen++);
		break;
	case PRIM:
		gen = "primmaze";
		while (*d++ = *gen++);
		break;
	case DFS_GEN:
		gen = "dfsmaze";
		while (*d++ = *gen++);
		break;
	case RECUR:
		gen = "recrumaze";
		while (*d++ = *gen++);
		break;
	default:
		break;
	}
	--d;

	//写入寻路信息
	const char * words = "_solved_by_";
	while (*d++ = *words++);
	--d;
	const char * exp;
	switch (EXP_MODE_TAG)
	{
	case NO_EXP:
		break;
	case DFS_EXP:
		exp = "dfs";
		while (*d++ = *exp++);
		break;
	case BFS:
		exp = "bfs";
		while (*d++ = *exp++);
		break;
	case ASTAR:
		exp = "astar";
		while (*d++ = *exp++);
		break;
	default:
		break;
	}
	--d;

	//写入后缀名
	const char* ex = ".png";
	while (*d++ = *ex++);
	*d = '\0';

	//将char*转换为wchar_t*，num为宽字符所需要的字节数
	//用库函数saveimage来存储
	int num = MultiByteToWideChar(0, 0, name, -1, NULL, 0);
	wchar_t *wide = new wchar_t[num];
	MultiByteToWideChar(0, 0, name, -1, wide, num);
	
	saveimage(wide, &MAZE_BK);
	free(name);
}

//////////////////////////////////////////////////////////////////////////////////////////////////

int is_moveable(MAZE MAZE, POINT * p, vec v)//判断某个方向是否可以移动 
{
	int flag = 1;

	switch (v) {							//边界判定
	case VEC_UP:
		if (MAZE[p->x][p->y].neighbors[VEC_UP] == -1)
			return 0;
		break;
	case VEC_DOWN:
		if (MAZE[p->x][p->y].neighbors[VEC_DOWN] == -1)
			return 0;
		break;
	case VEC_LEFT:
		if (MAZE[p->x][p->y].neighbors[VEC_LEFT] == -1)
			return 0;
		break;
	case VEC_RIGHT:
		if (MAZE[p->x][p->y].neighbors[VEC_RIGHT] == -1)
			return 0;
		break;
	default:
		break;
	}

	if (MAZE[p->x + MAZE_STEP[v].x][p->y + MAZE_STEP[v].y].is_visited == 1)
		return 0;						//不能走到其他已经访问的路上
	return 1;
}


int get_moveable_choice(MAZE MAZE, POINT *p) {//得到可移动方向的数量，并记录方向 

	int num = 0;

	for (int i = VEC_UP; i <= VEC_RIGHT; i++) {
		if ((MAZE[p->x][p->y].neighbors[i] != -1)
			&& (MAZE[p->x + MAZE_STEP[i].x][p->y + MAZE_STEP[i].y].is_visited == 0)) {
			++num;
		}
	}

	return num;
}

//随机返回该cell一个可行的前进方向
vec get_a_rand_direction(MAZE MAZE, POINT *p) {
	int rest[4] = { -1 }, len = 0, i = 0;
	for (i = 0; i < 4; i++) {
		if (is_moveable(MAZE, p, (vec)i)) {
			rest[len] = i;
			len++;
		}
	}
	if (len == 0) exit(-1);
	return (vec)rest[rand() % len];
}

//初始化迷宫
void init_maze_system(MAZE *MAZE) {

	*MAZE = (maze_cell**)malloc(sizeof(maze_cell*)*MAZE_LENGTH);//为迷宫分配内存
	if (*MAZE == NULL) exit(-1);
	for (int i = 0; i < MAZE_LENGTH; i++) {
		(*MAZE)[i] = (maze_cell*)malloc(sizeof(maze_cell)*MAZE_HEIGHT);
		if ((*MAZE)[i] == NULL) exit(-1);
		memset((*MAZE)[i], 0, sizeof(maze_cell)*MAZE_HEIGHT);
	}

	//////////////////////////////////////////
	for (int i = 0; i < MAZE_HEIGHT; i++) {					//将迷宫的边界外一格置为-1
		(*MAZE)[0][i].neighbors[VEC_LEFT] = -1;				//0代表该邻居未遍历
		(*MAZE)[MAZE_LENGTH - 1][i].neighbors[VEC_RIGHT] = -1;
	}
	for (int i = 0; i < MAZE_LENGTH; i++) {
		(*MAZE)[i][0].neighbors[VEC_UP] = -1;
		(*MAZE)[i][MAZE_HEIGHT - 1].neighbors[VEC_DOWN] = -1;
	}
	//////////////////////////////////////////
	GEN_MODE_TAG = NO_GEN;
	EXP_MODE_TAG = NO_EXP;

	srand((unsigned)time(NULL));
	initgraph(MAZE_LENGTH*CELL_SIZE + 1 + MAZE_PADDING_RIGHT, MAZE_HEIGHT*CELL_SIZE + 1);
	Resize(&MAZE_BK, MAZE_LENGTH*CELL_SIZE+1, MAZE_HEIGHT*CELL_SIZE+1);
	BeginBatchDraw();

	loadimage(&MAZE_PANNEL, PANNEL_BK);
	putimage(MAZE_LENGTH*CELL_SIZE + 1, 0, &MAZE_PANNEL);
	FlushBatchDraw();

	MAZE_DELAY(1);
	return;
}

void destory_maze(MAZE MAZE) {
	if (MAZE == NULL) {
		exit(-1);
	}
	for (int i = 0; i < MAZE_LENGTH; i++) {
		free((MAZE)[i]);
	}
	free(MAZE);
	MAZE = NULL;
}

void clear_maze(MAZE MAZE) {

	for (int i = 0; i < MAZE_LENGTH; i++) {
		memset(MAZE[i], 0, sizeof(maze_cell)*MAZE_HEIGHT);
	}

	//将迷宫的边界外一格置为-1
	//0代表该邻居未遍历
	for (int i = 0; i < MAZE_HEIGHT; i++) {
		MAZE[0][i].neighbors[VEC_LEFT] = -1;
		MAZE[MAZE_LENGTH - 1][i].neighbors[VEC_RIGHT] = -1;
	}
	for (int i = 0; i < MAZE_LENGTH; i++) {
		MAZE[i][0].neighbors[VEC_UP] = -1;
		MAZE[i][MAZE_HEIGHT - 1].neighbors[VEC_DOWN] = -1;
	}
}

void load_raw_maze() {
	switch (GEN_MODE_TAG)
	{
	case NO_GEN:
		exit(-1);
		break;
	case KRUSKAL:
		loadimage(&MAZE_BK, MAZE_KRUS);
		break;
	case PRIM:
		loadimage(&MAZE_BK, MAZE_PRIM);
		break;
	case DFS_GEN:
		loadimage(&MAZE_BK, MAZE_DFS);
		break;
	case RECUR:
		loadimage(&MAZE_BK, MAZE_RECUR);
		break;
	default:
		break;
	}
	putimage(0, 0, &MAZE_BK);
	FlushBatchDraw();
}

void end_maze_system(MAZE MAZE) {
	IMAGE button;
	loadimage(&button, _T("pics//end_button.png"));
	SetWorkingImage(&MAZE_PANNEL);
	putimage(125, 455, &button);
	SetWorkingImage(NULL);
	putimage(MAZE_HEIGHT*CELL_SIZE, 0, &MAZE_PANNEL);
	FlushBatchDraw();
	while (true) {
		MOUSEMSG m;
		m = GetMouseMsg();
		if(m.mkLButton)
			if (m.x < 185 + MAZE_HEIGHT * CELL_SIZE && m.x >125 + MAZE_HEIGHT * CELL_SIZE  && m.y < 495 && m.y > 455) {
				destory_maze(MAZE);
				closegraph();
			}
	}
}

//DFS生成迷宫
void generate_maze_dfs(MAZE MAZE) {
	GEN_MODE_TAG = DFS_GEN;
	draw_gen_maze_tag();
	draw_raw_maze();
	clear_maze(MAZE);

	//储存来路的栈
	maze_stack MAZE_STACK;
	init_maze_stack(&MAZE_STACK);

	clear_maze(MAZE);

	//当前点
	int i = rand() % MAZE_LENGTH, j = rand() % MAZE_HEIGHT;
	POINT cur = { i,j };

	//起点入栈
	MAZE[i][j].is_visited = 1;
	maze_stack_push(&MAZE_STACK, cur);

	do {
		MAZE_DELAY(500);
		//移动位置
		move_cur_generate_dfs(MAZE, &cur, &MAZE_STACK);
		FlushBatchDraw();

	} while (MAZE_STACK.len != 1);//当回到原点时，说明所有点都走完了 
	//将迷宫邻居中的未遍历的邻居记为-1,是否遍历记为0
	for (int i = 0; i < MAZE_HEIGHT; i++) {
		for (int j = 0; j < MAZE_LENGTH; j++) {
			for (int k = 0; k < 4; k++) {
				MAZE[j][i].neighbors[k] = ((MAZE[j][i].neighbors[k] == 1) ? 1 : -1);
			}
			MAZE[j][i].is_visited = 0;
		}
	}
	//生成迷宫结束，迷宫图片保存，数据记录在MAZE中
	GEN_MODE_TAG = DFS_GEN;
	saveimage(MAZE_DFS, &MAZE_BK);

	//删除辅助数据
	destory_maze_stack(&MAZE_STACK);
	return;
}

void move_back_generate_dfs(MAZE MAZE,POINT *cur,maze_stack *MAZE_STACK) {//在生成时退回可移动位置

	while (!get_moveable_choice(MAZE,&(MAZE_STACK->base[MAZE_STACK->len - 1]))) {
		maze_stack_pop(MAZE_STACK,NULL);
		if (MAZE_STACK->len == 1) {//到原点了
			break;
		}

	}

	maze_stack_top(MAZE_STACK,cur);
}


void move_cur_generate_dfs(MAZE MAZE,POINT *cur,maze_stack *MAZE_STACK) {//在生成时移动

	//记录可以走的方向数目
	int num = get_moveable_choice(MAZE,cur);

	//若无路，则退栈
	if (num == 0) move_back_generate_dfs(MAZE,cur,MAZE_STACK);
	else {
		path_length_inc();

	//如果有选择一个方向移动，并将两边的邻居设置为1
		vec direction = get_a_rand_direction(MAZE,cur);
		draw_maze_path(*cur,direction,CONNECT_MODE);
		draw_maze_bk();
		MAZE[cur->x][cur->y].neighbors[direction] = 1;

		cur->x += MAZE_STEP[direction].x;
		cur->y += MAZE_STEP[direction].y;
		MAZE[cur->x][cur->y].is_visited = 1;

		//互相连通,得到之前的方向
		switch (direction) {
			case 1:
				direction = (vec)0;
				break;
			case 0:
				direction = (vec)1;
				break;
			case 2:
				direction = (vec)3;
				break;
			case 3:
				direction = (vec)2;
				break;
		}
		MAZE[cur->x][cur->y].neighbors[direction] = 1;
		
		//新节点入栈
		maze_stack_push(MAZE_STACK,*cur);
	}
	return;
}

void generate_maze_krus(MAZE MAZE) {

	GEN_MODE_TAG = KRUSKAL;
	draw_raw_maze();
	clear_maze(MAZE);
	draw_gen_maze_tag();

	//建立并查集
	maze_ufset set;
	init_maze_ufset(&set);
	//建立一个含有所有边的表
	maze_edge_list edges;
	init_maze_edge_list(&edges);
	//加入所有边
	maze_edge e;
	for (int i = 0; i < MAZE_HEIGHT; i++) {
		for (int j = 0; j < MAZE_LENGTH - 1; j++) {
			e.p1 = { j,i };
			e.p2 = { j + 1,i };
			maze_edge_list_push(&edges, e);
		}
	}

	for (int i = 0; i < MAZE_LENGTH; i++) {
		for (int j = 0; j < MAZE_HEIGHT - 1; j++) {
			e.p1 = { i,j };
			e.p2 = { i,j + 1 };
			maze_edge_list_push(&edges, e);
		}
	}

	while (!maze_edge_list_empty(edges)) {
		maze_edge_list_pop(&edges, &e);
		POINT r1, r2;
		r1 = get_maze_ufset_root(set, e.p1);
		r2 = get_maze_ufset_root(set, e.p2);
		if (r1.x != r2.x || r1.y != r2.y) {
			joint_maze_ufset(set, r1, r2);
			draw_connected_wall(e);
			draw_maze_bk();
			FlushBatchDraw();
			MAZE_DELAY(500);
			MAZE[e.p1.x][e.p1.y].neighbors[POINT_TO_VEC(e.p1, e.p2)] = 1;
			MAZE[e.p2.x][e.p2.y].neighbors[POINT_TO_VEC(e.p2, e.p1)] = 1;
		}
	}

	//将迷宫邻居中的未遍历的邻居记为-1,是否遍历记为0
	for (int i = 0; i < MAZE_HEIGHT; i++) {
		for (int j = 0; j < MAZE_LENGTH; j++) {
			for (int k = 0; k < 4; k++) {
				MAZE[j][i].neighbors[k] = ((MAZE[j][i].neighbors[k] == 1) ? 1 : -1);
				MAZE[j][i].is_visited = 0;
			}
		}
	}
	//生成迷宫结束，迷宫图片保存，数据记录在MAZE中
	saveimage(MAZE_KRUS, &MAZE_BK);

	//删除辅助数据
	destory_maze_ufset(set);
	destory_maze_edge_list(edges);
	return;
}

void generate_maze_prim(MAZE MAZE) {

	GEN_MODE_TAG = PRIM;
	draw_gen_maze_tag();
	draw_raw_maze();
	clear_maze(MAZE);

	//建立边表
	maze_edge_list edges;
	init_maze_edge_list(&edges);
	//随机选择一个点
	POINT cur = { rand() % MAZE_LENGTH, rand() % MAZE_HEIGHT };
	maze_edge e;
	int cnt = 1;

	do {
		//访问当前点
		MAZE[cur.x][cur.y].is_visited = 1;
		//若这条路是连通而且未访问过的
		for (int i = 0; i < 4; i++) {
			if (MAZE[cur.x][cur.y].neighbors[i] == 0) {
				POINT neighbor = { cur.x + MAZE_STEP[i].x,cur.y + MAZE_STEP[i].y };
				//就把这条边加入表中
				if (MAZE[neighbor.x][neighbor.y].is_visited != 1) {
					e = { cur,neighbor };
					maze_edge_list_push(&edges, e);
				}
			}
		}
		do
		{
			//弹出下一个可以访问的边
			//这里实现不好，但是可以少写函数，建立含节点的表更简单
			maze_edge_list_pop(&edges, &e);

		} while (MAZE[e.p1.x][e.p1.y].is_visited == 1 && MAZE[e.p2.x][e.p2.y].is_visited == 1);
		draw_connected_wall(e);
		draw_maze_bk();
		FlushBatchDraw();
		MAZE_DELAY(500);
		//让两边的邻居相互连通
		MAZE[e.p1.x][e.p1.y].neighbors[POINT_TO_VEC(e.p1, e.p2)] = 1;
		MAZE[e.p2.x][e.p2.y].neighbors[POINT_TO_VEC(e.p2, e.p1)] = 1;

		//移动到下一个未访问的点
		cur = MAZE[e.p1.x][e.p1.y].is_visited == 1 ? e.p2 : e.p1;

		++cnt;//记录访问顶点数
	} while (cnt < MAZE_LENGTH*MAZE_HEIGHT);// 访问所有顶点时退出

	//将迷宫邻居中的未遍历的邻居记为-1,是否遍历记为0
	for (int i = 0; i < MAZE_HEIGHT; i++) {
		for (int j = 0; j < MAZE_LENGTH; j++) {
			for (int k = 0; k < 4; k++) {
				MAZE[j][i].neighbors[k] = ((MAZE[j][i].neighbors[k] == 1) ? 1 : -1);
				MAZE[j][i].is_visited = 0;
			}
		}
	}
	//生成迷宫结束，迷宫图片保存，数据记录在MAZE中
	saveimage(MAZE_PRIM, &MAZE_BK);

	//删除辅助数据
	destory_maze_edge_list(edges);
	return;
}

//递归分割生成迷宫
void generate_maze_recur(MAZE MAZE) {
	GEN_MODE_TAG = RECUR;
	draw_gen_maze_tag();
	draw_raw_maze();
	clear_maze(MAZE);

	//进行分割
	devide_maze(MAZE, 0, MAZE_HEIGHT, 0, MAZE_LENGTH);

	//将迷宫邻居中的未遍历的邻居记为-1,是否遍历记为0
	for (int i = 0; i < MAZE_HEIGHT; i++) {
		for (int j = 0; j < MAZE_LENGTH; j++) {
			for (int k = 0; k < 4; k++) {
				MAZE[j][i].neighbors[k] = ((MAZE[j][i].neighbors[k] == 1) ? 1 : -1);
				MAZE[j][i].is_visited = 0;
			}
		}
	}

	//生成迷宫结束，迷宫图片保存，数据记录在MAZE中
	saveimage(MAZE_RECUR, &MAZE_BK);

	return;
}

////分割迷宫
void devide_maze(MAZE MAZE, int border_top, int border_bottom, int border_left, int border_right) {
	//printf("\n==============\n%d %d %d %d\n", border_top, border_bottom, border_left, border_right);
	////////////////////////////////////////////////////
	//递归边界,注意分割时只在内部分割，统统是十字分割
	////////////////////////////////////////////////////
	int x, y;

	//当这是一个宽或高为1的子区块时，其内部的边统统会被打通，退化为一个单链表
	if (border_bottom - border_top == 1) {
		//所有单元有相同的纵坐标
		y = border_top;
		for (int i = border_left; i < border_right - 1; i++)
		{
			//从左到右依次打通
			MAZE[i][y].neighbors[3] = 1;
			MAZE[i + 1][y].neighbors[2] = 1;

			draw_connected_wall({ {i,y}, {i + 1,y} });
			draw_maze_bk();
			MAZE_DELAY(500);
			FlushBatchDraw();
		}
		return;
	}

	if (border_right - border_left == 1) {
		//所有单元有相同的纵坐标
		x = border_left;
		for (int i = border_top; i < border_bottom - 1; i++)
		{
			//从上到下依次打通
			MAZE[x][i].neighbors[1] = 1;
			MAZE[x][i + 1].neighbors[0] = 1;

			draw_connected_wall({ {x,i}, {x,i + 1} });
			draw_maze_bk();
			MAZE_DELAY(500);
			FlushBatchDraw();
		}
		return;
	}

	////////////////////////////////////////////////////
	//下面是一般情况
	////////////////////////////////////////////////////

	//随机取十字交叉点的坐标
	int crossover_x = rand() % (border_right - border_left - 1)+ 1+border_left,
		crossover_y = rand() % (border_bottom - border_top - 1)+ 1+border_top;
	POINT p1,p2;

	//printf("crossover is %d %d\n", crossover_x, crossover_y);

	// 随机选择一个不打通的方向
	int v = rand() % 4;
	
	if (v != VEC_UP)

	{	// p1在p2右边
		p1 = { crossover_x, border_top + rand() % (crossover_y - border_top) };
		p2 = { p1.x - 1, p1.y };

		MAZE[p1.x][p1.y].neighbors[2] = 1;
		MAZE[p2.x][p2.y].neighbors[3] = 1;

		draw_connected_wall({ p1,p2 });
		draw_maze_bk();
		MAZE_DELAY(300);
		FlushBatchDraw();
	}
	if (v != VEC_DOWN)

	{	// p1在p2右边
		p1 = { crossover_x, crossover_y + rand() % (border_bottom - crossover_y) };
		p2 = { p1.x - 1, p1.y };

		MAZE[p1.x][p1.y].neighbors[2] = 1;
		MAZE[p2.x][p2.y].neighbors[3] = 1;

		draw_connected_wall({ p1,p2 });
		draw_maze_bk();
		MAZE_DELAY(300);
		FlushBatchDraw();
	}
	if (v != VEC_LEFT)

	{	// p1在p2下边
		p1 = {border_left + rand() % (crossover_x - border_left), crossover_y};
		p2 = { p1.x, p1.y - 1};

		MAZE[p1.x][p1.y].neighbors[0] = 1;
		MAZE[p2.x][p2.y].neighbors[1] = 1;

		draw_connected_wall({ p1,p2 });
		draw_maze_bk();
		MAZE_DELAY(300);
		FlushBatchDraw();
	}
	if (v != VEC_RIGHT)

	{	// p1在p2下边
		p1 = {crossover_x + rand() % (border_right - crossover_x), crossover_y };
		p2 = { p1.x, p1.y - 1};

		MAZE[p1.x][p1.y].neighbors[0] = 1;
		MAZE[p2.x][p2.y].neighbors[1] = 1;

		draw_connected_wall({ p1,p2 });
		draw_maze_bk();
		MAZE_DELAY(300);
		FlushBatchDraw();
	}

	//递归四片子区域
	devide_maze(MAZE, border_top, crossover_y, border_left, crossover_x);
	devide_maze(MAZE, border_top, crossover_y, crossover_x, border_right);
	devide_maze(MAZE, crossover_y, border_bottom, crossover_x, border_right);
	devide_maze(MAZE, crossover_y, border_bottom, border_left, crossover_x);

	return;
}

//BFS走迷宫
void explore_maze_bfs(MAZE MAZE) {
	if (!MAZE)
	{
		return;
	}

	/*先遍历，记录父节点，再从叶子画到根节点*/
	EXP_MODE_TAG = BFS;
	draw_solve_maze_tag();
	load_raw_maze();//加载图片背景

	POINT **father;
	father = (POINT**)malloc(sizeof(POINT)*MAZE_LENGTH*MAZE_HEIGHT);
	if (!father) exit(-1);
	for (int i = 0; i < MAZE_LENGTH; i++) {
		father[i] = (POINT*)malloc(sizeof(POINT)*MAZE_HEIGHT);
		if (!father[i]) exit(-1);
		memset(father[i], 0, sizeof(POINT)*MAZE_HEIGHT);
	}

	maze_queue queue;
	init_maze_queue(&queue);

	POINT cur = MAZE_START;
	maze_enqueue(&queue, cur);

	load_raw_maze();//加载图片背景

	while (cur.x != MAZE_GOAL.x || cur.y != MAZE_GOAL.y) {
		MAZE_DELAY(200);
		//移动至下一格
		move_cur_explore_bfs(MAZE, &cur, &queue, father);
	}
	for (int i = 0; i < MAZE_HEIGHT; i++) {
		for (int j = 0; j < MAZE_LENGTH; j++) {
			MAZE[j][i].is_visited = 0;
		}
	}

	PATH_COLOR_DEGREE = 0;
	PATH_COLOR_DEGREE_STATE = 1;
	while (cur.x != MAZE_START.x || cur.y != MAZE_START.y) {
		MAZE_DELAY(60);
		//移动至下一格
		int dx = father[cur.x][cur.y].x - cur.x, dy = father[cur.x][cur.y].y - cur.y;
		draw_maze_visited(cur, (vec)((1 + dy + dx * dx * (4 + dx)) / 2), PATH_MODE);
		cur = father[cur.x][cur.y];
		FlushBatchDraw();

		path_length_inc();
	}

	//画最后一格
	setfillcolor(PATHCOLOR);
	draw_maze_cell(cur);
	draw_maze_bk();
	draw_maze_block(cur);
	FlushBatchDraw();
	MAZE_DELAY(1);

	//删除辅助数据
	for (int i = 0; i < MAZE_LENGTH; i++) {
		free(father[i]);
	}
	free(father);
	destory_maze_queue(&queue);

	//保存搜索结果
	save_solved_maze();
}


void move_cur_explore_bfs(MAZE MAZE, POINT *cur, maze_queue *queue, POINT** father) {

	maze_dequeue(queue, cur);

	if (MAZE[cur->x][cur->y].is_visited == 0) {
		MAZE[cur->x][cur->y].is_visited = 1;
		POINT p;
		for (int v = VEC_UP; v <= VEC_RIGHT; v++) {
			if (MAZE[cur->x][cur->y].neighbors[(vec)v] == 1) {
				switch (v)
				{
				case VEC_UP:
					p = { 0 + cur->x,-1+cur->y };
					break;
				case VEC_DOWN:
					p = { 0+cur->x,1 + cur->y };
					break;
				case VEC_LEFT:
					p = { -1+cur->x,0 + cur->y };
					break;
				case VEC_RIGHT:
					p = { 1+cur->x,0 + cur->y };
					break;
				default:
					break;
				}
				if (MAZE[p.x][p.y].is_visited == 0) {
					maze_enqueue(queue, p);
					father[p.x][p.y].x = cur->x;
					father[p.x][p.y].y = cur->y;
				}
				int dx = p.x - cur->x, dy = p.y - cur->y;
				draw_maze_visited(*cur, (vec)((1 + dy + dx * dx * (4 + dx)) / 2), VISITED_MODE);
			}

		}
	}
	
}

//DFS走迷宫
void explore_maze_dfs(MAZE MAZE) {
	if (!MAZE)
	{
		return;
	}

	EXP_MODE_TAG = DFS_EXP;
	draw_solve_maze_tag();
	load_raw_maze();//加载图片背景

	POINT cur = MAZE_START;
	MAZE[cur.x][cur.y].is_visited = 1;

	//起点入栈
	maze_stack stack;
	init_maze_stack(&stack);
	maze_stack_push(&stack, cur);

	PATH_COLOR_DEGREE = 0;
	PATH_COLOR_DEGREE_STATE = 1;

	while (cur.x != MAZE_GOAL.x || cur.y != MAZE_GOAL.y) {
		MAZE_DELAY(100);
		//移动至下一格
		move_cur_explore_dfs(MAZE, &cur, &stack);
	}

	for (int i = 0; i < MAZE_HEIGHT; i++) {
		for (int j = 0; j < MAZE_LENGTH; j++) {
			MAZE[j][i].is_visited = 0;
		}
	}

	//画最后一格
	//绘制背景
	setfillcolor(PATHCOLOR);
	draw_maze_cell(cur);
	draw_maze_bk();
	draw_maze_block(cur);
	FlushBatchDraw();
	MAZE_DELAY(1);
	//删除辅助数据
	destory_maze_stack(&stack);

	//保存搜索结果
	save_solved_maze();
	
}

//在走迷宫时退到最近的可移动位置，或原点 
void move_back_explore_dfs(POINT *cur, maze_stack *MAZE_STACK) {

	//确定迷宫的颜色
	path_length_dec();
	//退回去的点
	POINT last;
	maze_stack_pop(MAZE_STACK, &last);
	maze_stack_top(MAZE_STACK, cur);
	//绘制退栈时的位置
	draw_maze_visited(last, POINT_TO_VEC(last, (*cur)), VISITED_MODE);
	FlushBatchDraw();

}

void move_cur_explore_dfs(MAZE MAZE, POINT *cur, maze_stack *MAZE_STACK) {//移动当前位置（DFS走迷宫）
	int choices = get_moveable_choice(MAZE, cur);
	if (choices == 0)	move_back_explore_dfs(cur, MAZE_STACK);
	else {

		path_length_inc();
		//随机选取方向
		vec direction = get_a_rand_direction(MAZE, cur);
		//绘制背景
		draw_maze_visited(*cur, direction, PATH_MODE);
		//改变坐标
		cur->x += MAZE_STEP[direction].x;
		cur->y += MAZE_STEP[direction].y;
		maze_stack_push(MAZE_STACK, *cur);
		MAZE[cur->x][cur->y].is_visited = 1;

	}
}

void init_mazea(MAZE MAZE, MAZE_TA MAZEA) {
	for (int i = 0; i < MAZE_HEIGHT; i++) {
		for (int j = 0; j < MAZE_LENGTH; j++) {
			MAZEA[i][j].dis_from = i * i + j * j;
			MAZEA[i][j].dis_to = (MAZE_GOAL.x - j)*(MAZE_GOAL.x - j) + (MAZE_GOAL.y - i)*(MAZE_GOAL.y - i);
			MAZEA[i][j].maze_copy = MAZE[i][j];
			//MAZEA[i][j].ideal_dis = MAZEA[i][j].dis_from + MAZEA[i][j].dis_to;
			MAZEA[i][j].point.x = i;
			MAZEA[i][j].point.y = j;
		}
	}
}

void explore_maze_astar(MAZE MAZE) {
	if (!MAZE)
	{
		return;
	}

	EXP_MODE_TAG = ASTAR;
	draw_solve_maze_tag();

	PATH_COLOR_DEGREE = 0;
	PATH_COLOR_DEGREE_STATE = 1;

	load_raw_maze();//加载图片背景

	MAZE_TA MAZEA;
	MAZEA = (MAZE_CELLA**)malloc(sizeof(MAZE_CELLA*)*MAZE_LENGTH);
	if (MAZE == NULL) exit(-1);
	for (int i = 0; i < MAZE_LENGTH; i++) {
		MAZEA[i] = (MAZE_CELLA*)malloc(sizeof(MAZE_CELLA)*MAZE_HEIGHT);
		if (!MAZEA[i]) exit(-1);
		memset(MAZEA[i], 0, sizeof(MAZE_CELLA)*MAZE_HEIGHT);     
	}
	init_mazea(MAZE, MAZEA);
	//建立两个二叉堆，保证第一个元素为f最小的
	maze_min_heap *open, *close;
	open = (maze_min_heap*)malloc(sizeof(maze_min_heap));
	close = (maze_min_heap*)malloc(sizeof(maze_min_heap));
	init_maze_heap(open);
	init_maze_heap(close);
	POINT cur = MAZE_START;
	maze_heap_insert(open, &MAZEA[cur.x][cur.y]);
	MAZEA[cur.x][cur.y].parent = NULL;
	while (cur.x != MAZE_GOAL.x || cur.y != MAZE_GOAL.y) {
		MAZE_DELAY(200);
		move_cur_astar(MAZEA, &cur,open,close);
	}

	PATH_COLOR_DEGREE = 0;
	PATH_COLOR_DEGREE_STATE = 1;
	while (cur.x != MAZE_START.x || cur.y != MAZE_START.y) {
		MAZE_DELAY(60);
		//移动至下一格
		draw_maze_visited(cur, POINT_TO_VEC(cur, MAZEA[cur.x][cur.y].parent->point), PATH_MODE);
		cur = MAZEA[cur.x][cur.y].parent->point;
		FlushBatchDraw();

		path_length_inc();
	}

	//画最后一格
	//绘制背景
	setfillcolor(PATHCOLOR);
	draw_maze_cell(cur);
	draw_maze_bk();
	draw_maze_block(cur);
	FlushBatchDraw();
	MAZE_DELAY(1);

	//删除辅助内存
	delete_mazea(MAZEA);
	destory_maze_heap(open);
	destory_maze_heap(close);
	//保存搜索结果
	save_solved_maze();

	return;
}
void move_cur_astar(MAZE_TA MAZEA, POINT* cur, maze_min_heap *open, maze_min_heap *close) {
	
	draw_maze_position(*cur);
	//查看四个邻居
	for (int i = 0; i < 4; i++) {
		//判断邻居是否连通并且没有访问过
		if (MAZEA[cur->x][cur->y].maze_copy.neighbors[i] == 1 && !maze_heap_search(close, MAZEA[cur->x + MAZE_STEP[i].x][cur->y + MAZE_STEP[i].y])&& !maze_heap_search(open, MAZEA[cur->x + MAZE_STEP[i].x][cur->y + MAZE_STEP[i].y])) {
			int *dis_from, *dis_to, *ideal_dis; 
			POINT this_neightbor = { cur->x + MAZE_STEP[i].x ,cur->y + MAZE_STEP[i].y };
			dis_from = &(MAZEA[this_neightbor.x][this_neightbor.y].dis_from);
			dis_to = &(MAZEA[this_neightbor.x][this_neightbor.y].dis_to);
			ideal_dis = &(MAZEA[this_neightbor.x][this_neightbor.y].ideal_dis);
			

			MAZEA[this_neightbor.x][this_neightbor.y].parent = &(MAZEA[cur->x][cur->y]);
			
			*dis_from = MAZEA[cur->x][cur->y].dis_from + 1;
			*dis_to = MAZE_GOAL.x - (this_neightbor.x) + MAZE_GOAL.y - (this_neightbor.y);
			*ideal_dis = *dis_from + *dis_to;
			maze_heap_insert(open, &MAZEA[this_neightbor.x][this_neightbor.y]);
			draw_maze_visited(*cur, (vec)i, VISITED_MODE);
		}
	}
	maze_heap_erase(open);
	maze_heap_insert(close, &MAZEA[cur->x][cur->y]);
	*cur = { open->data[1]->point.x, open->data[1]->point.y };
}

void delete_mazea (MAZE_TA MAZEA) {
	if (MAZEA == NULL) {
		exit(-1);
	}
	for (int i = 0; i < MAZE_LENGTH; i++) {
		free((MAZEA)[i]);
	}
	free(MAZEA);
	MAZEA = NULL;
}

