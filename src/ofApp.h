#pragma once

#include "ofMain.h"
#include "ofxGui.h"
#include  "ofxAssimpModelLoader.h"
#include "Octree.h"
#include <glm/gtx/intersect.hpp>
#include "ParticleSystem.h"
#include "ParticleEmitter.h"

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void windowResized(int w, int h);
		void dragEvent2(ofDragInfo dragInfo);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
		void drawAxis(ofVec3f);
		void initLightingAndMaterials();
		void savePicture();
		void toggleWireframeMode();
		void togglePointsDisplay();
		void toggleSelectTerrain();
		void setCameraTarget();
		bool mouseIntersectPlane(ofVec3f planePoint, ofVec3f planeNorm, ofVec3f &point);
		bool raySelectWithOctree(ofVec3f &pointRet);
		glm::vec3 ofApp::getMousePointOnPlane(glm::vec3 p , glm::vec3 n);

		ofEasyCam cam;
		ofCamera distanceShot;
		ofEasyCam trackCam;
		ofEasyCam landerCam;
		ofCamera* theCam;

		ofxAssimpModelLoader mars, lander;

		//Movement for the lander
		//
		//heading moves on the y axis
		glm::vec3 heading() {
			glm::vec3 initialHeading = glm::vec3(0, 0, 1);    // heading at start
			glm::mat4 Mrot = glm::rotate(glm::mat4(1.0), glm::radians(lander.getRotationAngle(0)), glm::vec3(0, 1, 0));
			glm::vec3 h = Mrot * glm::vec4(initialHeading, 1);
			return glm::normalize(h);
		}

		//heading moves the the z axis
		//For when the lander in thrusting upwards
		glm::vec3 headingZ() {
			glm::vec3 initialHeading = glm::vec3(0, 1, 0);    // heading at start
			glm::mat4 Mrot = glm::rotate(glm::mat4(1.0), glm::radians(float(0)), glm::vec3(0, 0, 1));
			glm::vec3 h = Mrot * glm::vec4(initialHeading, 1);
			return glm::normalize(h);
		}

		void integrate();

		// Physics data goes here  (for integrate() );
		//
		glm::vec3 velocity = glm::vec3(0, 0, 0);
		glm::vec3 acceleration = glm::vec3(0, 0, 0);
		glm::vec3 force = glm::vec3(0, 0, 0);
		float mass = 1.0;
		float damping = .99;
		float angularForce = 0;
		float angularVelocity = 0.0;
		float angularAcceleration = 0.0;
		bool bThrust = false;
		float prevDist = 0;

		float rotation = lander.getRotationAngle(0);


		//Altitude Sensor
		void altitudeSensor();
		float altitude;

		//
		//background
		ofImage screenBackground;

		//  sounds
		//
		ofSoundPlayer engineSound;

		// textures
		//
		ofTexture  particleTex;

		// shaders
		//
		ofVbo vbo;
		ofShader shader;
		void loadVbo();
		void loadVboExplosion();
		// Emitter and some forces;
		//
		ParticleEmitter emitter;
		ParticleEmitter explosionEmitter;

		TurbulenceForce* turbForce;
		GravityForce* gravityForce;
		ImpulseRadialForce* radialForce;
		CyclicForce* cyclicForce;

		//Gravity physics with the particle
		//GravityForce grav;
		Particle* player;
		ParticleSystem* playerSys;
		float restitution = .25;
		void checkCollisions();

		ofLight light;
		ofLight lightTopRight, lightBttmLeft, lightMountains;

		Box boundingBox, landerBounds;
		Box testBox;
		vector<Box> colBoxList;
		bool bLanderSelected = false;
		Octree octree;
		TreeNode selectedNode;
		glm::vec3 mouseDownPos, mouseLastPos;
		bool bInDrag = false;

		//time
		float startTime;
		float endTime;


		ofxIntSlider numLevels;
		ofxToggle bTimingInfo;
		ofxPanel gui;

		bool bAltKeyDown;
		bool bCtrlKeyDown;
		bool bWireframe;
		bool bDisplayPoints;
		bool bPointSelected;
		bool bHide;
		bool pointSelected = false;
		bool bDisplayLeafNodes = false;
		bool bDisplayOctree = false;
		bool bDisplayBBoxes = false;
		
		bool bLanderLoaded;
		bool bTerrainSelected;
	
		ofVec3f selectedPoint;
		ofVec3f intersectPoint;

		vector<Box> bboxList;

		const float selectionRange = 4.0;

		ofColor color[10];
};
