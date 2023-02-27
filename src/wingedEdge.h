#pragma once
#ifndef _WINGEDEDGE_H_
#define _WINGEDEDGE_H_

#include <vector>

class Vertex {
public:
	glm::vec3 location;
	Vertex(glm::vec3 l) { location = l; }
};

class Face {
public:
	Face() {};
	Edge e;
};

class Edge {
	public:
		Edge() {};
		Edge * lprev, * lnext, * rprev, * rnext;
		Vertex * head, * tail;
		Face * left, * right;

};


class WingedEdgeMesh {
public:
	std::vector<Edge> EdgeList;

};


#endif // _WINGEDEDGE_H_