#pragma once
#include"solver.h"
/*
Inplement the genetic algorithm
*/
class GeneticSolver:public Solver
{
private:
	class Gene
	{
	private:
		std::vector<int>code;	//gene code
		double score;			//the score of the gene
	public:
		Gene(std::vector<int>);
		Gene();
		Gene(const Gene&);
		void calculate_score(Graph&);
		double get_score();
		Gene& operator=(const Gene&);
		bool operator<(const Gene&)const;
		int& operator[](int);
		std::vector<int> get_code();
		void set_score(double);
	};

	std::vector<Gene>gene_pool;
	double total_score = 0;
	int gene_number;
	const int max_iter = 100000;	
	const int brute_force_batch = 10;	//the batch size of the vertexes used in the final Karp's algorithm
	const double mutation_rate = 0.3;
	const double cross_rate = 0.8;

	void insert_to_gene_pool(Gene&,Gene&);	//put a gene into the gene pool
	void random_generate_gene(Gene&);	//randomly generate a gene
	void on_cross(Gene&, Gene&);	//mate 2 genes
	void select_best_and_worst();	//put the best score in the front of the gene pool and put the wrost one in the end
	void init();
	void select();		//select this generation
	void cross();		//mate this generation
	void mutate();		//mutate this generation
public:
	GeneticSolver(Graph);
	void solve();
};
