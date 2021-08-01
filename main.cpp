#include"pch.h"
#include<stdlib.h>
#include<graphics.h>
#include "maze.h"

int main() {

	MAZE MAZE;
	init_maze_system(&MAZE);                             //��ʼ���Թ�

	generate_maze_recur(MAZE);					  //ʹ�õݹ�ָ������Թ�
	explore_maze_astar(MAZE);					  //Ѱ·
	explore_maze_bfs(MAZE);
	explore_maze_dfs(MAZE);

	generate_maze_krus(MAZE);					 //ʹ��Kruskal�����Թ�
	explore_maze_astar(MAZE);					  //Ѱ·
	explore_maze_bfs(MAZE);
	explore_maze_dfs(MAZE);

	generate_maze_prim(MAZE);					 //ʹ��Prim�����Թ�
	explore_maze_astar(MAZE);					  //Ѱ·
	explore_maze_bfs(MAZE);
	explore_maze_dfs(MAZE);

	generate_maze_dfs(MAZE);					//ʹ��DFS�����Թ�
	explore_maze_astar(MAZE);					  //Ѱ·
	explore_maze_bfs(MAZE);
	explore_maze_dfs(MAZE);
	
	end_maze_system(MAZE);						//�����Թ�ϵͳ���رմ��ڣ������ڴ�
	
	return 0;
}