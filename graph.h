#pragma once
#include<set>
#include<vector>
#include<iostream>
#include"vertex.h"
/*
Base graph class
*/
class Graph
{
	std::vector<Vertex>graph;
public:
	Graph();
	Graph(std::vector<Vertex>);
	void add_vertex(Vertex);
	int size();
	void clear();	//clear this graph
	Vertex operator[](int);
};