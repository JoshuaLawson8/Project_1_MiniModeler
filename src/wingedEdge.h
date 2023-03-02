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
	Edge * e;
};

class Face {
	public:
		Face(Edge * edg) {e = edg; };
		Edge * e;
};

class Edge {
	public:
		Edge(Vertex * h, Vertex * t) {
			head = h;
			head->e = this;
			tail = t;
			tail->e = this;
		};
		Edge * lprev, * lnext, * rprev, * rnext;
		Vertex * head, * tail;
		Face * left, * right;
		void draw() { 
			ofDrawLine(head->location,tail->location); 
		}
};



class WingedEdgeMesh {
public:
	std::vector<Edge *> edgeList;
	std::map<Vertex *, Edge *> vertList;
	std::map<Face *, Edge *> faceList;
	void addEdge(Edge * e) {
		edgeList.push_back(e);
		vertList.insert({ e->head, e });
		vertList.insert({ e->tail, e });
	}

	vector<Edge *> edgesOfVertex(Vertex v) {
		vector<Edge *> edges;
		Edge * e = v.e;
		edges.push_back(e);
		while (e != v.e) {
			if (e->tail == &v) {
				e = e->lprev;
				edges.push_back(e);
			}
			else {
				e = e->rprev;
				edges.push_back(e);
			}
		}
		return edges;
	}

	vector<Face *> facesOfVertex(Vertex v) {
		vector<Face *> faces;
		Edge * e = v.e;
		faces.push_back(e->left);
		while (e != v.e) {
			if (e->tail == &v) {
				e = e->lprev;
				faces.push_back(e->left);
			}
			else {
				e = e->rprev;
				faces.push_back(e->left);
			}
		}
		return faces;
	}

	vector<Edge *> edgesOfFace(Face * f) {
		vector<Edge *> edges;
		Edge * e = f->e;
		while (e != f->e) {
			if (e->left == f) {
				e = e->lnext;
				edges.push_back(e);
			}
			else {
				e = e->rnext;
				edges.push_back(e);
			}
		}
		return edges;
	}

	vector<Vertex *> vertsOfFace(Face * f) {
		set<Vertex *> vertSet;
		vector<Vertex *> verts;
		vector<Edge *> edges = edgesOfFace(f);
		for (Edge * e : edges) {
			vertSet.insert(e->head);
			vertSet.insert(e->tail);
		}
		for (Vertex * v : vertSet) {
			verts.push_back(v);
		}
		return verts;
	}
};


#endif // _WINGEDEDGE_H_