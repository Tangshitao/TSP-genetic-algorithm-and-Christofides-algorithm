#include"genetic_solver.h"
#include"minimum_spanning_tree_solver.h"
#include"brute_force_solver.h"
#include <random>
#include<thread>
#include <assert.h> 
#include <stdexcept>
#include<unordered_map>
#include <chrono>
#include <ctime>
GeneticSolver::Gene::Gene(){}
GeneticSolver::Gene::Gene(const Gene&ge)
{
	code = ge.code;
	score = ge.score;
}

GeneticSolver::Gene::Gene(std::vector<int>code_)
{
	code = code_;
}

void  GeneticSolver::Gene::calculate_score(Graph&g)
{
	score = g[code[0]].distance(g[code[code.size() - 1]]);
	for (int i = 1; i <(int) code.size(); i++)
	{
		score += g[code[i]].distance(g[code[i - 1]]);
	}
}

double GeneticSolver::Gene::get_score()
{
	return score;
}

GeneticSolver::Gene& GeneticSolver::Gene::operator=(const Gene&another)
{
	code = another.code;
	score = another.score;
	return *this;
}

bool GeneticSolver::Gene::operator<(const Gene&another)const
{
	return score < another.score;
}

int& GeneticSolver::Gene::operator[](int i)
{
	return code[i];
}

std::vector<int> GeneticSolver::Gene::get_code()
{
	return code;
}

void GeneticSolver::Gene::set_score(double score_)
{
	score = score_;
}

GeneticSolver::GeneticSolver(Graph graph_):Solver(graph_)
{
}

void GeneticSolver::random_generate_gene(Gene&a)
{
	std::vector<int>base((int)graph.size());

	for (int i = 0; i < (int)graph.size(); i++)
	{
		base[i] = i;
	}
	

	static std::random_device rd;
	static std::mt19937 g(rd());
	shuffle(base.begin(), base.end(), g);
	a = Gene(base);
	a.calculate_score(graph);
}

void GeneticSolver::init()
{
	gene_number =std::min(MAX_GENE_NUMBER, graph.size());
	gene_pool.resize(gene_number);

	//add a gene formed by Christofides algorithm to get a good initilization
	MinimumSpanningTreeSolver msts(graph);
	msts.solve();
	std::unordered_map<std::string, int>ump;
	for (int i = 0; i < graph.size(); i++)
	{
		ump[graph[i].name] = i;
	}
	std::vector<Vertex>best=msts.get_solution();
	std::vector<int>best_code;
	for (int i = 0; i <(int) best.size(); i++)
	{
		best_code.push_back(ump[best[i].name]);
	}
	gene_pool[0] = Gene(best_code);
	gene_pool[0].set_score(msts.get_score());

	//randonly generate genes
	for (int i = 1; i < gene_number; i++)
	{
		random_generate_gene(gene_pool[i]);
	}

	select_best_and_worst();
}

void GeneticSolver::select_best_and_worst()
{
	int best = 0, worst = 0;
	for (int i = 0; i <(int)gene_pool.size(); i++)
	{
		if (gene_pool[i].get_score() <gene_pool[best].get_score())
		{
			best = i;
		}
		if (gene_pool[i].get_score() > gene_pool[worst].get_score())
		{
			worst = i;
		}
	}
	std::swap(gene_pool[0], gene_pool[best]);
	std::swap(gene_pool[gene_pool.size() - 1], gene_pool[worst]);

}


void GeneticSolver::insert_to_gene_pool(Gene &a,Gene &b)
{
	a = b;
}

void GeneticSolver::select()
{
	//the genes in gene_pool should be ordered by its score
	std::vector<Gene>previous_gene_pool = gene_pool;
	int n = gene_pool.size();

	std::vector<std::thread>thread_pool;
	static std::random_device rd;  
	static std::mt19937 gen(rd()); 
	std::uniform_real_distribution<> dis(0,n*(n+1)/2);
	for (int i = 1; i < (int)gene_pool.size(); i++)
	{
		double rand = dis(gen);
		//assgin weight n,n-1,n-2... to the 1st,2nd,3rd... gene so that the gene has the probabitily of i/sum(1...n) to be seleccted
		int j = (int)(n-1-(-1 + sqrt(1 + 8 * rand)) / 2);
		thread_pool.push_back(std::thread(&GeneticSolver::insert_to_gene_pool, this,std::ref(gene_pool[i]),std::ref(previous_gene_pool[j])));
	}
	for (int i = 0; i <(int)thread_pool.size(); i++)
	{
		thread_pool[i].join();
	}

	select_best_and_worst();
}


void GeneticSolver::on_cross(Gene&a, Gene&b)
{
	//random generate 2 values, start, end
	static std::random_device rd;
	static std::mt19937 gen(rd());
	std::uniform_real_distribution<> dis(0, graph.size());
	int start = (int)dis(gen), end = (int)dis(gen);
	if (start > end)
		std::swap(start, end);
	else if(start == end)
		return;

	std::vector<bool>marka(graph.size()), markb(graph.size());
	Gene previous_a(a), previous_b(b);
	//a get the code bwteen 0 and 'start' and 'end' and end of the gene from b and b vice versa. 
	for (int i = 0; i < graph.size(); i++)
	{
		if (i == start)
		{
			i = end - 1;
		}
		else
		{
			marka[a[i]] = true;
			markb[b[i]] = true;
			std::swap(a[i], b[i]);
		}	
	}

	//get code from 'start' to 'end'.
	for (int i = start,ja=0,jb=0; i < end; i++,ja++,jb++)
	{
		while (marka[previous_b[ja]])ja++;
		while (markb[previous_a[jb]])jb++;
		a[i] = previous_a[jb];
		b[i] = previous_b[ja];
	}
	a.calculate_score(graph);
	b.calculate_score(graph);

}

void GeneticSolver::cross()
{
	static std::random_device rd;
	static std::mt19937 gen(rd());
	std::uniform_real_distribution<> dis(0, 1);
	std::vector<int>need_mutating;
	gene_pool.push_back(gene_pool[0]);
	for (int i = 1; i <(int)gene_pool.size(); i++)
	{
		if (dis(gen) < cross_rate)
		{
			need_mutating.push_back(i);
		}
	}

	std::shuffle(need_mutating.begin(), need_mutating.end(), gen);
	std::vector<std::thread>thread_pool;
	for (int i = 1; i < (int)need_mutating.size(); i+=2)
	{
		//use multithreading to speed up the process
		thread_pool.push_back(std::thread(&GeneticSolver::on_cross, this, std::ref(gene_pool[need_mutating[i]]), std::ref(gene_pool[need_mutating[i-1]])));
	}

	for (int i = 0; i < (int)thread_pool.size(); i++)
	{
		thread_pool[i].join();
	}

	select_best_and_worst();
	gene_pool.pop_back();
}

void GeneticSolver::mutate()
{
	gene_pool.push_back(gene_pool[0]);
	static std::random_device rd;
	static std::mt19937 gen(rd());
	std::uniform_real_distribution<> dis(0, 1);
	for (int i = 1; i <(int)gene_pool.size(); i++)
	{
		if (dis(gen) < mutation_rate)
		{
			std::uniform_real_distribution<> temp(0, graph.size());
			int a = -1, b = -1;
			while (a == b)
			{
				a = (int)temp(gen);
				b = (int)temp(gen);
			}
			
			auto func = [](int index,Graph&g,Gene&gene) 
			{
				int len = g.size();
				int l = (index + len - 1) % len, r = (index + 1) % len;
				return g[gene[index]].distance(g[gene[l]]) + g[gene[index]].distance(g[gene[r]]);
			};
			
			int v1 = gene_pool[i][a], v2 = gene_pool[i][b];
			//substract old distance
			double score = gene_pool[i].get_score()-func(a,graph,gene_pool[i])-func(b,graph,gene_pool[i]);
			//if a and b are adjacenment, add the distance between them
			if (abs(a-b) == 1||abs(a-b)==graph.size()-1)
			{
				score += graph[v1].distance(graph[v2]);
			}
			std::swap(gene_pool[i][a], gene_pool[i][b]);
			//add the new distance
			score += func(a, graph, gene_pool[i]) + func(b, graph, gene_pool[i]);
			if (abs(a - b) == 1 || abs(a - b) == graph.size() - 1)
			{
				score -= graph[v1].distance(graph[v2]);
			}
			gene_pool[i].set_score(score);
			gene_pool[i].calculate_score(graph);
		}
	}

	std::sort(gene_pool.begin(),gene_pool.end());
	gene_pool.pop_back();
}

void GeneticSolver::solve()
{
	
	std::chrono::time_point<std::chrono::system_clock> start, end;
	std::chrono::duration<double> elapsed_seconds = start - start;
	start = std::chrono::system_clock::now();
	init();
	//set the execution time
	int seconds = EXECUTION_TIME[0];
	if (graph.size() > 1000)
		seconds = EXECUTION_TIME[1];
	for (int iter = 0; iter < max_iter&&elapsed_seconds.count() < seconds; iter++)
	{
		select();
		cross();
		mutate();
		if (iter % 5 == 0)
		{
			end = std::chrono::system_clock::now();
			elapsed_seconds = end - start;
			#ifdef DEBUG 
				std::cout<<"Iteration: "<<iter<<" Execution time: "<<elapsed_seconds.count()<<" Value: "<<gene_pool[0].get_score()<<std::endl;
			#endif 
		}
	}

	std::vector<int>code = gene_pool[0].get_code();
	for (int i = 0; i <(int)code.size(); i++)
	{
		solution.push_back(graph[code[i]]);
	}

	//used Karp's algorithm to fineture the final result
	Graph temp;

	for (int i = 0; i < (int)solution.size(); i++)
	{
		temp.add_vertex(solution[i]);
		if (temp.size() == brute_force_batch || i ==(int) solution.size() - 1)
		{
			BruteForceSolver bfs(temp);
			bfs.solve();
			std::vector<Vertex>res = bfs.get_solution(temp[0], temp[temp.size() - 1]);
			for (int j = 0; j < (int)res.size(); j++)
			{
				solution[i] = res[j];
			}
			temp.clear();
		}
	}
	score = solution[solution.size() - 1].distance(solution[0]);
	for (int i = 1; i < (int)solution.size(); i++)
	{
		score += solution[i].distance(solution[i - 1]);
	}
}

