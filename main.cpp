#include"pch.h"
#include<stdlib.h>
#include<graphics.h>
#include "maze.h"

int main() {

	MAZE MAZE;
	init_maze_system(&MAZE);                             //初始化迷宫

	generate_maze_recur(MAZE);					  //使用递归分割生成迷宫
	explore_maze_astar(MAZE);					  //寻路
	explore_maze_bfs(MAZE);
	explore_maze_dfs(MAZE);

	generate_maze_krus(MAZE);					 //使用Kruskal生成迷宫
	explore_maze_astar(MAZE);					  //寻路
	explore_maze_bfs(MAZE);
	explore_maze_dfs(MAZE);

	generate_maze_prim(MAZE);					 //使用Prim生成迷宫
	explore_maze_astar(MAZE);					  //寻路
	explore_maze_bfs(MAZE);
	explore_maze_dfs(MAZE);

	generate_maze_dfs(MAZE);					//使用DFS生成迷宫
	explore_maze_astar(MAZE);					  //寻路
	explore_maze_bfs(MAZE);
	explore_maze_dfs(MAZE);
	
	end_maze_system(MAZE);						//结束迷宫系统，关闭窗口，回收内存
	
	return 0;
}