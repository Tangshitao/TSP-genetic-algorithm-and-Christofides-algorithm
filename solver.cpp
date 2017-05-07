#include"solver.h"
Solver::Solver()
{
}
Solver::Solver(Graph graph_):graph(graph_){}
std::vector<Vertex> Solver::get_solution(){ return solution; }
double Solver::get_score() { return score; }