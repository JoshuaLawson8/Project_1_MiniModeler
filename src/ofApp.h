#pragma once

#include "ofMain.h"
#include "ofxAssimpModelLoader.h"
#include <glm/gtx/intersect.hpp>
#include "vector3.h"
#include "ray.h"
#include "WingedEdge.h"

class ofApp : public ofBaseApp {

public:
	void setup();
	void update();
	void draw();

	void keyPressed(int key);
	void saveToFile();
	void keyReleased(int key);
	void mouseMoved(int x, int y);
	void mouseDragged(int x, int y, int button);
	void mousePressed(int x, int y, int button);
	void mouseReleased(int x, int y, int button);
	void mouseEntered(int x, int y);
	void mouseExited(int x, int y);
	void windowResized(int w, int h);
	void dragEvent(ofDragInfo dragInfo);
	void drawAxis(ofVec3f location);
	void gotMessage(ofMessage msg);

	bool bAddVertex = false;
	bool bShift = false;
	bool bDrag = false;
	bool bDelete = false;
	bool bFace = false;

	ofEasyCam cam;
	ofCamera* theCam;
	ofMesh testMesh;
	WingedEdgeMesh mesh;
	vector<Vertex*> floatingVerts;
	Vertex* firstSelect;

	vector<Vertex *> faceList;

	int orthoDistance = 15;
	glm::vec3 lastPoint;
};
