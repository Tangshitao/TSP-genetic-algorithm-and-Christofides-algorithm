#include"graph_reader.h"
#include <fstream>
#include<string>
graphReader::graphReader() {}

Graph graphReader::read(std::string filename)
{
	std::ifstream in(filename);
	std::string temp;
	in >>temp>>  temp >> temp;

	Graph graph;
	std::string name;
	int x, y;
	while (in >>name>> x >> y)
	{
		graph.add_vertex(Vertex(x, y, name));
		
	}

	in.close();
	return graph;
}