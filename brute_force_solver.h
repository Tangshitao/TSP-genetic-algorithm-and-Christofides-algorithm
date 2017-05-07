#include"solver.h"
/*
Implement Karp's algorithm. 
*/
class BruteForceSolver :public Solver 
{
private:
	std::vector<double>dp;			//store the answer
	std::vector<std::pair<int,int>>pre;	//store the prevous vertex to construct the solution
	int get_index(int, int, int);
public:
	BruteForceSolver(Graph);
	void solve();
	std::vector<Vertex> get_solution(Vertex start, Vertex end);	//return the optimized route from start to end
	
};