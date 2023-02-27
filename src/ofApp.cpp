#include "ofApp.h"

// test comment to test pushing

//--------------------------------------------------------------
void ofApp::setup(){
	theCam = &cam;
	cam.setPosition(15, 0, 0);
	cam.lookAt(glm::vec3(0, 0, 0));
	cam.setNearClip(.1);

}

//--------------------------------------------------------------
void ofApp::update(){

}

//--------------------------------------------------------------
void ofApp::draw(){
	cam.begin();
	ofNoFill();
	for (int i = 0; i < testMesh.getNumVertices(); i++) {
		ofDrawSphere(testMesh.getVertex(i),.1);
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
		/*
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
		*/

		cam.setPosition(0, 15, 0);
		cam.lookAt(glm::vec3(0, 0, 0));
		break;
	case 'f':
		/*
		cam.enableOrtho();
		cam.setNearClip(-1000000);
		cam.setFarClip(1000000);

		cam.removeAllInteractions();
		cam.addInteraction(ofEasyCam::TRANSFORM_TRANSLATE_XY, OF_MOUSE_BUTTON_LEFT);
		cam.addInteraction(ofEasyCam::TRANSFORM_TRANSLATE_Z, OF_MOUSE_BUTTON_RIGHT);
		*/

		cam.setPosition(15, 0, 0);
		cam.lookAt(glm::vec3(0, 0, 0));
		break;
	case 'r':
		/*
		cam.enableOrtho();
		cam.setNearClip(-1000000);
		cam.setFarClip(1000000);

		cam.removeAllInteractions();
		cam.addInteraction(ofEasyCam::TRANSFORM_TRANSLATE_XY, OF_MOUSE_BUTTON_LEFT);
		cam.addInteraction(ofEasyCam::TRANSFORM_TRANSLATE_Z, OF_MOUSE_BUTTON_RIGHT);
		*/

		cam.setPosition(0, 0, 15);
		cam.lookAt(glm::vec3(0, 0, 0));
		break;
	case 'p':
		if (cam.getOrtho()) {
			cam.disableOrtho();

			/*
			// reset interactions to original Easy Camera controls
			cam.removeAllInteractions();
			cam.addInteraction(ofEasyCam::TRANSFORM_ROTATE, OF_MOUSE_BUTTON_LEFT);
			cam.addInteraction(ofEasyCam::TRANSFORM_TRANSLATE_XY, OF_MOUSE_BUTTON_LEFT, 'm'); // m for move
			cam.addInteraction(ofEasyCam::TRANSFORM_TRANSLATE_Z, OF_MOUSE_BUTTON_RIGHT);
			cam.addInteraction(ofEasyCam::TRANSFORM_TRANSLATE_XY, OF_MOUSE_BUTTON_MIDDLE);
			cam.setNearClip(.1);
			*/
		}
		/*
		else {
			// if ortho is already disabled, reset perspective camera position/viewing angle to default
			cam.setPosition(15, 0, 0);
			cam.lookAt(glm::vec3(0, 0, 0));
			
			// comment out the other else block if you want to try using this
		}
		*/
		else {
			cam.enableOrtho();
			cam.setPosition(glm::vec3(0, 0, 5));
		}
		break;
	case 'v':
		if (!bAddVertex)
			cam.disableMouseInput();
		else
			cam.enableMouseInput();
		bAddVertex = !bAddVertex;

	}
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button) {

	//setting up the rays
	glm::vec3 origin = theCam->getPosition();
	glm::vec3 camAxis = theCam->getZAxis();
	glm::vec3 mouseWorld = theCam->screenToWorld(glm::vec3(mouseX, mouseY, 0));
	glm::vec3 mouseDir = glm::normalize(mouseWorld - origin);
	
	if (bAddVertex) {
		std::cout << "adding vert" << endl;
		float distance;
		glm::vec3 intersect, normal;
		glm::intersectRayPlane(origin, mouseDir, glm::vec3(0, 0, 0), glm::normalize(cam.getZAxis()), distance);
		testMesh.addVertex(ofPoint(origin + mouseDir * distance));
	}

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

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