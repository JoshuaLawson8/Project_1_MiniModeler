#include "ofApp.h"
#include <iostream>
#include <fstream>

// test comment to test pushing

//--------------------------------------------------------------
void ofApp::setup() {
	ofNoFill();
	theCam = &cam;
	cam.setPosition(orthoDistance, 0, 0);
	cam.lookAt(glm::vec3(0, 0, 0));
	cam.setNearClip(.1);

}

//--------------------------------------------------------------
void ofApp::update() {

}

//--------------------------------------------------------------
void ofApp::draw() {
	cam.begin();
	for (Vertex* v : floatingVerts) {
		if (firstSelect) {
			if (v->location == firstSelect->location) {
				ofSetColor(ofColor::purple);
				v->draw();
				ofSetColor(ofColor::white);
			}
			else {
				v->draw();
			}
		}
		else
			v->draw();
	}
	for (Edge* e : mesh.edgeList) {
		e->draw();
	}
	/*
	for (auto f : mesh.faceList) {
		vector<Vertex*> verts = mesh.vertsOfFace(f.first);
	}
	*/

	vector<ofMesh> faces;
	for (auto f : mesh.faceList) {
		vector<Vertex*> verts = mesh.vertsOfFace(f.first);	// currently this only returns an empty vector
		ofMesh face;
		for (auto v : verts) {
			face.addVertex(v->location);
		}
		faces.push_back(face);
	}
	for (auto f : faces) {
		f.drawFaces();
	}

	drawAxis(ofVec3f(0, 0, 0));
	ofSetColor(ofColor::white);
	ofDrawBox(10);
	ofDrawSphere(5, 5, 5, 1);
	cam.end();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key) {
	switch (key) {
	case 't':
		// enables orthographic mode
		// changes near clip/far clip distance so the object can actually be seen
		cam.enableOrtho();
		cam.setNearClip(-1000000);
		cam.setFarClip(1000000);

		// left-click dragging only pans the XY plane
		// right-click dragging only zooms in and out using the Z-axis
		cam.removeAllInteractions();
		cam.addInteraction(ofEasyCam::TRANSFORM_TRANSLATE_XY, OF_MOUSE_BUTTON_LEFT);
		cam.addInteraction(ofEasyCam::TRANSFORM_TRANSLATE_Z, OF_MOUSE_BUTTON_RIGHT);
		cam.setPosition(0, orthoDistance, 0);
		cam.lookAt(glm::vec3(0, 0, 0));
		break;
	case 'f':
		cam.enableOrtho();
		cam.setNearClip(-1000000);
		cam.setFarClip(1000000);

		cam.removeAllInteractions();
		cam.addInteraction(ofEasyCam::TRANSFORM_TRANSLATE_XY, OF_MOUSE_BUTTON_LEFT);
		cam.addInteraction(ofEasyCam::TRANSFORM_TRANSLATE_Z, OF_MOUSE_BUTTON_RIGHT);
		cam.setPosition(orthoDistance, 0, 0);
		cam.lookAt(glm::vec3(0, 0, 0));
		break;
	case 'r':
		cam.enableOrtho();
		cam.setNearClip(-1000000);
		cam.setFarClip(1000000);

		cam.removeAllInteractions();
		cam.addInteraction(ofEasyCam::TRANSFORM_TRANSLATE_XY, OF_MOUSE_BUTTON_LEFT);
		cam.addInteraction(ofEasyCam::TRANSFORM_TRANSLATE_Z, OF_MOUSE_BUTTON_RIGHT);
		cam.setPosition(0, 0, orthoDistance);
		cam.lookAt(glm::vec3(0, 0, 0));
		break;
	case 'p':
		if (cam.getOrtho()) {
			cam.disableOrtho();
			// reset interactions to original Easy Camera controls
			cam.removeAllInteractions();
			cam.addInteraction(ofEasyCam::TRANSFORM_ROTATE, OF_MOUSE_BUTTON_LEFT);
			cam.addInteraction(ofEasyCam::TRANSFORM_TRANSLATE_XY, OF_MOUSE_BUTTON_LEFT, 'm'); // m for move
			cam.addInteraction(ofEasyCam::TRANSFORM_TRANSLATE_Z, OF_MOUSE_BUTTON_RIGHT);
			cam.addInteraction(ofEasyCam::TRANSFORM_TRANSLATE_XY, OF_MOUSE_BUTTON_MIDDLE);
			cam.setNearClip(.1);
		}
		else {
			// if ortho is already disabled, reset perspective camera position/viewing angle to default
			cam.setPosition(orthoDistance, 0, 0);
			cam.lookAt(glm::vec3(0, 0, 0));
		}
		break;
	case 'd':
		bDelete = true;
		break;
	case 'm':
		bFace = true;
		break;
	case 1:
		bShift = true;
		break;
	case 's':
		saveToFile();
		break;
	case 'v':
		if (!bAddVertex)
			cam.disableMouseInput();
		else
			cam.enableMouseInput();
		bAddVertex = !bAddVertex;
		break;
	}
}

void ofApp::saveToFile() {
	std::cout << "saving" << endl;
	ofstream objFile("data/mini_model.obj");
	for (auto const& x : mesh.vertList) {
		Vertex* v = x.first;
		objFile << "v " << v->location.x << " " << v->location.y << " " << v->location.z << endl;
	}
	for (auto const& x : mesh.faceList) {
		Face* f = x.first;
		vector<Vertex*> verts = mesh.vertsOfFace(f);
		//use std find to find the index of verts 
		vector<int> indices;
		for (Vertex* v : verts) {
			auto it = find(verts.begin(), verts.end(), v);
			indices.push_back(it - verts.begin());

		}
		objFile << "f" << " " << indices[0] << " " << indices[1] << " " << indices[2] << " " << indices[3] << endl;
	}
	objFile.close();
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key) {
	switch (key) {
	case 1:
		bShift = false;
		firstSelect = NULL;
		break;
	case 'd':
		bDelete = false;
		break;
	case 'm':
		//empty out the array
		faceList.clear();
		bFace = false;
		break;
	}
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y) {

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button) {
	if (firstSelect && bDrag) {
		//setting up the rays
		glm::vec3 origin = theCam->getPosition();
		glm::vec3 camAxis = theCam->getZAxis();
		glm::vec3 mouseWorld = theCam->screenToWorld(glm::vec3(mouseX, mouseY, 0));
		glm::vec3 mouseDir = glm::normalize(mouseWorld - origin);

		glm::vec3 point;

		float distance;
		glm::vec3 intersect, normal;

		if (glm::intersectRayPlane(origin, mouseDir, glm::vec3(0, 0, 0), glm::normalize(cam.getZAxis()), distance))
			point = origin + mouseDir * distance;

		firstSelect->location += point - lastPoint;
		lastPoint = point;
	}
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button) {

	//setting up the rays
	glm::vec3 origin = theCam->getPosition();
	glm::vec3 camAxis = theCam->getZAxis();
	glm::vec3 mouseWorld = theCam->screenToWorld(glm::vec3(mouseX, mouseY, 0));
	glm::vec3 mouseDir = glm::normalize(mouseWorld - origin);

	if (bShift) {
		glm::vec3 intersect, norm;
		for (int i = 0; i < floatingVerts.size(); i++) {
			if (glm::intersectRaySphere(origin, mouseDir, floatingVerts[i]->location, .1, intersect, norm)) {
				if (firstSelect) {
					Edge* newEdge = new Edge(firstSelect, floatingVerts[i]);
					mesh.addEdge(newEdge);
				}
				else
					firstSelect = floatingVerts[i];
				break;
			}
		}
		return;
	}

	if (bFace) {
		glm::vec3 intersect, norm;
		for (int i = 0; i < floatingVerts.size(); i++) {
			if (glm::intersectRaySphere(origin, mouseDir, floatingVerts[i]->location, .1, intersect, norm)) {
				if (faceList.size() >= 3) {
					std::cout << "adding face" << endl;
					faceList.push_back(floatingVerts[i]);
					mesh.faceList.insert({ new Face(faceList[0]->e), faceList[0]->e });
				}
				else {
					std::cout << "face lsit" << faceList.size() << endl;

					faceList.push_back(floatingVerts[i]);
					//Edge* newEdge = new Edge(firstSelect, floatingVerts[i]);
					//mesh.addEdge(newEdge);
				}

				break;
			}
		}
		return;
	}


	if (bDelete) {
		glm::vec3 intersect, norm;
		for (int i = 0; i < floatingVerts.size(); i++) {
			if (glm::intersectRaySphere(origin, mouseDir, floatingVerts[i]->location, .1, intersect, norm)) {
				// remove the edges of the vertex
				cout << mesh.edgesOfVertex(*floatingVerts[i]).size() << endl;
				cout << "removing edges" << endl;
				for (auto re : mesh.edgesOfVertex(*floatingVerts[i])) {
					//remove(mesh.edgeList.begin(), mesh.edgeList.end(), re);
					mesh.edgeList.erase(std::remove(mesh.edgeList.begin(), mesh.edgeList.end(), re), mesh.edgeList.end());
				}
				cout << mesh.edgesOfVertex(*floatingVerts[i]).size() << endl;

				mesh.vertList.erase(floatingVerts[i]);
				floatingVerts.erase(floatingVerts.begin() + i);

				break;
			}
		}
		return;
	}


	if (bAddVertex) {
		std::cout << "adding vert" << endl;

		if (theCam->getOrtho()) {
			floatingVerts.push_back(new Vertex(mouseWorld - glm::normalize(camAxis) * orthoDistance));
		}
		else {
			float distance;
			glm::vec3 intersect, normal;
			glm::intersectRayPlane(origin, mouseDir, glm::vec3(0, 0, 0), glm::normalize(cam.getZAxis()), distance);
			floatingVerts.push_back(new Vertex(origin + mouseDir * distance));
		}
	}

	else {

		glm::vec3 intersect, norm;
		for (int i = 0; i < floatingVerts.size(); i++) {
			if (glm::intersectRaySphere(origin, mouseDir, floatingVerts[i]->location, .1, intersect, norm)) {
				firstSelect = floatingVerts[i];
				lastPoint = intersect;

				bDrag = true;
				cam.disableMouseInput();
				break;
			}
		}
	}


}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button) {
	if (bDrag) {
		bDrag = false;
		cam.enableMouseInput();
	}
}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y) {

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y) {

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h) {

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg) {

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo) {

}

void ofApp::drawAxis(ofVec3f location) {

	ofPushMatrix();
	ofTranslate(location);

	ofSetLineWidth(1.0);

	// X Axis
	ofSetColor(ofColor(255, 0, 0));
	ofDrawLine(ofPoint(0, 0, 0), ofPoint(1, 0, 0));


	// Y Axis
	ofSetColor(ofColor(0, 255, 0));
	ofDrawLine(ofPoint(0, 0, 0), ofPoint(0, 1, 0));

	// Z Axis
	ofSetColor(ofColor(0, 0, 255));
	ofDrawLine(ofPoint(0, 0, 0), ofPoint(0, 0, 1));

	ofPopMatrix();
}