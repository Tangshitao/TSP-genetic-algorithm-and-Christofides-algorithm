#include"brute_force_solver.h"
BruteForceSolver::BruteForceSolver(Graph graph_) :Solver(graph_) 
{
	int siz = graph_.size();
	if (siz <= 15)
	{	
		dp.resize(siz*siz*(1 << siz));
		pre.resize(siz*siz*(1 << siz));
		std::fill(pre.begin(), pre.end(), std::make_pair(-1, -1));
	}
}

int BruteForceSolver::get_index(int u, int v, int offset)
{
	int siz = graph.size();
	return u*siz*(1 << siz) + v*(1 << siz) + offset;
}

void BruteForceSolver::solve()
{
	if (dp.size()==0)
	{
		return;
	}
	int siz = graph.size();
	for (int i = 0; i < (1 << siz); i++)
	{
		//i represents which vertex is in current status
		std::vector<int>temp;
		for (int k = 0, j = i; j; j >>= 1,k++)
		{
			if (j & 1)temp.push_back(k);
		}
		if (temp.size() == 1)
		{
			continue;
		}
		if (temp.size() == 2)
		{
			int index = get_index(temp[0], temp[1], i);
			dp[index] =dp[get_index(temp[1],temp[0],i)]= graph[temp[0]].distance(graph[temp[1]]);
			
			continue;
		}
		for (int j = 0; j <(int) temp.size();j++)
		{
			//j is the start point
			for (int k = 0; k <(int)temp.size(); k++)
			{
				//k is the end point
				Vertex u = graph[temp[j]], v = graph[temp[k]];
				int previous =i^( 1 << temp[j]);
				if (j != k)
					previous ^=1<< temp[k];
				int index = get_index(temp[j], temp[k], i);
				dp[index] = 1e9;
				for (int l = 0; l <(int) temp.size(); l++)
				{
					for (int m = 0; m < (int)temp.size(); m++)
					{
						if ((l!=m||(temp.size()==3&&j!=k))&&(l != j)&&(m != k)&&(l!=k)&&(m!=j))
						{
							Vertex uu = graph[temp[l]], vv = graph[temp[m]];
							int index_temp = get_index(temp[l], temp[m], previous);
							double dis = dp[index_temp] + u.distance(uu) + v.distance(vv);
							if (dis < dp[index])
							{
								dp[index] = dis;
								pre[index] = std::make_pair(temp[l], temp[m]);
							}
						}
					}
				}
			}
		}
	}

	//construct the solution
	score = dp[get_index(0, 0, (1 << siz)-1)];
	std::vector<int>half;
	int status = (1 << siz) - 1, start = 0, end = 0;
	solution.push_back(graph[0]);
	while(start!=-1)
	{
		std::pair<int, int>p = pre[get_index(start, end, status)];
		status = status ^ (1 << start);
		if (start != 0)
			status ^= 1 << end;
		start = p.first;
		end = p.second;
		if (start == -1)
			break;
		if(start!=0)
			solution.push_back(graph[start]);
		if(start!=end)
		half.push_back(end);
	}
	for (int i = half.size() - 1; i >= 0; i--)
	{
		solution.push_back(graph[half[i]]);
	}
}

std::vector<Vertex> BruteForceSolver::get_solution(Vertex start, Vertex end)
{
	int start_i=-1, end_i=-1;
	for (int i = 0; i < graph.size(); i++)
	{
		if (start.name == graph[i].name)
			start_i = i;
		if (end.name == graph[i].name)
			end_i=i;
	}
	std::vector<Vertex>res;

	std::vector<int>half;
	int status = (1 << graph.size()) - 1;
	while (start_i != -1&&end_i!=-1)
	{
		res.push_back(graph[start_i]);
		if (start_i != end_i)
			half.push_back(end_i);
		std::pair<int, int>p = pre[get_index(start_i, end_i, status)];
		status = status ^ (1 << start_i);
		status ^= 1 << end_i;
		start_i = p.first;
		end_i = p.second;
		
	}
	for (int i = half.size() - 1; i >= 0; i--)
	{
		res.push_back(graph[half[i]]);
	}
	return res;
}
