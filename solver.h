#pragma once
#include"graph.h"
#include<vector>
#include"config.h"
/*
Basic solver class
*/
class Solver
{
protected:
	Graph graph;
	std::vector<Vertex> solution;	//solution
	double score=MAX_SCORE;	//the optimized score
public:
	Solver();
	Solver(Graph);
	virtual void solve() = 0;
	std::vector<Vertex> get_solution();
	double get_score();
};