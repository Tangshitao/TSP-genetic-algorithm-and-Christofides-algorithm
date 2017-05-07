#include"minimum_spanning_tree_solver.h"
#include<functional>
#include<unordered_set>
#include<stack>
#include<set>
MinimumSpanningTreeSolver::MinimumSpanningTreeSolver(Graph graph_) :Solver(graph_)
{
	
}
void MinimumSpanningTreeSolver::solve()
{
	minimum_spanning_tree();

	std::vector<Vertex>route;

	perfect_matching();
	euler_tour();
		
}

void MinimumSpanningTreeSolver::minimum_spanning_tree()
{
	point_to.clear();
	to.resize(graph.size());
	for (int i = 0; i < graph.size(); i++)
	{
		for (int j = 0; j < i; j++)
		{
			edges.push(std::make_pair(-graph[i].distance(graph[j]), std::make_pair(i, j)));
		}
		point_to.push_back(i);
	}
	int tot = 1;
	double res = 0;
	while (edges.size())
	{
		std::pair<int, int> edge = edges.top().second;
		edges.pop();
		int v = edge.first, u = edge.second;
		int vt = find(v), ut = find(u);
		if (vt != ut)
		{
			point_to[vt] = ut;
			to[v].push_back(u);
			to[u].push_back(v);
			res += graph[v].distance(graph[u]);
			tot++;
		}
	}
}

int MinimumSpanningTreeSolver::find(int v)
{
	return point_to[v]=(point_to[v] == v ? v : find(point_to[v]));
}

void MinimumSpanningTreeSolver::perfect_matching()
{
	std::vector<int>oods;
	std::vector<bool>mark;
	//find all odd degree vertexes
	for (int i = 0; i <(int)to.size(); i++)
	{
		if (to[i].size() & 1)
		{
			oods.push_back(i);
			mark.push_back(true);
		}
	}

	for (int i = 0; i <(int)oods.size(); i++)
	{
		if (mark[i])
		{

			mark[i] = false;
			double min_ = MAX_SCORE;
			int closest;
			for (int j = i + 1; j <(int)oods.size(); j++)
			{
				if (mark[j])
				{
					double d = graph[oods[i]].distance(graph[oods[j]]);
					if (d < min_)
					{
						min_ = d;
						closest = j;
					}
				}
			}
			mark[closest] = false;
			to[oods[i]].push_back(oods[closest]);
			to[oods[closest]].push_back(oods[i]);
		}
	}
}

void MinimumSpanningTreeSolver::euler_tour()
{
	std::map<std::pair<int,int>, int>edge_number;
	std::function<void(int)> dfs_euler;
	std::vector<std::vector<int>>&to_ = to;
	std::stack<int>tour;
	
	for (int i = 0; i <(int)to.size(); i++)
	{
		for (int j = 0; j <(int)to[i].size(); j++)
			edge_number[std::make_pair(i, to[i][j])]++;
	}

	dfs_euler = [&edge_number,&tour,&to_,&dfs_euler](int v)
	{
		for (int i = 0; i <(int)to_[v].size(); i++)
		{
			if (edge_number[std::make_pair(v,to_[v][i])]!=0)
			{
				edge_number[std::make_pair(v, to_[v][i])]--;
				edge_number[std::make_pair(to_[v][i],v)]--;
				dfs_euler(to_[v][i]);
			}
		}
		tour.push(v);
	};

	dfs_euler(0);

	//construct the solution
	std::unordered_set<int>mark;
	while (tour.size())
	{
		int v = tour.top();
		tour.pop();
		if (mark.find(v) == mark.end())
		{
			solution.push_back(graph[v]);
			mark.insert(v);
		}
	}

	score = solution[0].distance(solution[solution.size() - 1]);
	for (int i = 1; i <(int)solution.size(); i++)
	{
		score += solution[i].distance(solution[i - 1]);
	}
}