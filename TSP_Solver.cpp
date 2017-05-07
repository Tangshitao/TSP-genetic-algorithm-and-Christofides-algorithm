#include"solver.h"
#include"graph.h"
#include"vertex.h"
#include"brute_force_solver.h"
#include"minimum_spanning_tree_solver.h"
#include"genetic_solver.h"
#include"graph_reader.h"
#include<fstream>

int main(int argc, char *argv[]) {


	graphReader gr;
	for (int i = 1; i < argc; i++)
	{
		
		Graph graph =  gr.read(argv[i]);
		GeneticSolver c(graph);
		c.solve();
		std::cout << argv[i] << ":" << c.get_score() << "\t";
		std::vector<Vertex>solution = c.get_solution();
		for (int j =0; j <(int)solution.size(); j++)
		{
			std::cout << solution[j].name << "\t";
		}
		std::cout << solution[0].name << std::endl;
	}

	return 0;
}
