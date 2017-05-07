#pragma once
#include"solver.h"
#include<map>
#include<queue>
/*
Implement the Christofides algorithm
*/
class MinimumSpanningTreeSolver :public Solver
{
private:
	std::priority_queue<std::pair<double,std::pair<int,int>>>edges;	
	std::vector<int>point_to;	//merge-find set
	std::vector<std::vector<int>>to;	//edges of the minimum spanning tree
	void minimum_spanning_tree();	//get the minimum spanning tree
	int find(int);	//find operation in merge-find set
	void perfect_matching();	//find a perfect matching, but not the minimum
	void euler_tour();	//find a euler tour
public:
	void solve();
	MinimumSpanningTreeSolver(Graph);
	
};