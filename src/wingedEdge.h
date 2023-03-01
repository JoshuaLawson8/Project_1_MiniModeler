#pragma once
#ifndef _WINGEDEDGE_H_
#define _WINGEDEDGE_H_

#include <vector>

class Edge;
class Face;

class Vertex {
public:
	glm::vec3 location;
	Vertex(glm::vec3 l) { location = l; }
	void draw() { ofDrawSphere(location, .1); }
};

class Face {
	public:
		Face() {};
		Edge * e;
};

class Edge {
	public:
		Edge(Vertex * h, Vertex * t) {
			head = h;
			tail = t;
		};
		Edge * lprev, * lnext, * rprev, * rnext;
		Vertex * head, * tail;
		Face * left, * right;
		void draw() { 
			std::cout << "drawing" << endl;
			std::cout << head->location;
			std::cout << tail->location;
			ofDrawLine(head->location,tail->location); 
		}
};



class WingedEdgeMesh {
public:
	std::vector<Edge *> edgeList;
	std::map<Vertex, Edge> vertList;
	std::map<Face, Edge> faceList;
};


#endif // _WINGEDEDGE_H_