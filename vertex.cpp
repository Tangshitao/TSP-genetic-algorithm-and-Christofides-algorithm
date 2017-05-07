#include"vertex.h"
Vertex::Vertex(double x_, double y_,std::string name_):x(x_),y(y_),name(name_) {}
Vertex::Vertex(const Vertex&a)
{
	name = a.name;
	x = a.x;
	y = a.y;
}

double Vertex::distance(Vertex v)const
{
	return sqrt((x - v.x)*(x - v.x) + (y - v.y)*(y - v.y));
}