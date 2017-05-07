#pragma once
#include<algorithm>
#include<string>
/*
Base vertex class
*/
class Vertex 
{
public:
	double x, y;
	std::string name;
	Vertex(double, double,std::string);
	Vertex(const Vertex&);
	double distance(Vertex)const;	//calcute the distance of 2 vertexs
};