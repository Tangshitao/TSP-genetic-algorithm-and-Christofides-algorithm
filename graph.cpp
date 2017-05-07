#include"graph.h"
Graph::Graph() {}
Graph::Graph(std::vector<Vertex> graph_) :graph(graph_)
{

}

void Graph::add_vertex(Vertex v) 
{
	graph.push_back(v);
}

int Graph::size()
{
	return graph.size();
}

Vertex Graph::operator[](int i)
{
	return graph[i];
}

void Graph::clear()
{
	graph.clear();
}