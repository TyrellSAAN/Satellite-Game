
//--------------------------------------------------------------
//
//  Kevin M. Smith
//
//  Octree Test - startup scene
// 
//
//  Student Name:   < Your Name goes Here >
//  Date: <date of last version>


#include "ofApp.h"
#include "Util.h"


//--------------------------------------------------------------
// setup scene, lighting, state and load geometry
//
void ofApp::setup(){
	bWireframe = false;
	bDisplayPoints = false;
	bAltKeyDown = false;
	bCtrlKeyDown = false;
	bLanderLoaded = false;
	bTerrainSelected = true;
//	ofSetWindowShape(1024, 768);
	cam.setDistance(25);
	cam.setNearClip(.1);
	cam.setFov(65.5);   // approx equivalent to 28mm in 35mm format
	ofSetVerticalSync(true);
	cam.disableMouseInput();
	ofEnableSmoothing();
	ofEnableDepthTest();

	distanceShot.setPosition(0, 5, -50);
	distanceShot.lookAt(glm::vec3(0, 8, 0));
	distanceShot.setNearClip(.1);
	distanceShot.setFov(65.5);

	
	trackCam.setDistance(10);
	trackCam.setNearClip(.1);
	trackCam.setFov(65.5);   // approx equivalent to 28mm in 35mm format
	trackCam.lookAt(glm::vec3(0, 1, 0));
	ofSetVerticalSync(true);
	trackCam.disableMouseInput();
	ofEnableSmoothing();
	ofEnableDepthTest();

	landerCam.setDistance(10);
	landerCam.setNearClip(.1);
	landerCam.setFov(65.5);   // approx equivalent to 28mm in 35mm format
	landerCam.lookAt(glm::vec3(0, 1, 0));
	ofSetVerticalSync(true);
	landerCam.disableMouseInput();
	ofEnableSmoothing();
	ofEnableDepthTest();

	theCam = &cam;

	// texture loading
	//
	ofDisableArbTex();     // disable rectangular textures

	// load textures
	//
	if (!ofLoadImage(particleTex, "images/dot.png")) {
		cout << "Particle Texture File: images/dot.png not found" << endl;
		ofExit();
	}

	// load the shader
	//
#ifdef TARGET_OPENGLES
	shader.load("shaders_gles/shader");
#else
	shader.load("shaders/shader");
#endif


	// Create Forces
	//
	turbForce = new TurbulenceForce(ofVec3f(0, -20, 0), ofVec3f(0, -20, 0));
	gravityForce = new GravityForce(ofVec3f(0, -0.5, 0));
	radialForce = new ImpulseRadialForce(400);
	cyclicForce = new CyclicForce(60);

	// set up the emitter for the thrust
	// 
	radialForce->setHeight(1);

	emitter.sys->addForce(turbForce);
	emitter.sys->addForce(gravityForce);
	emitter.sys->addForce(radialForce);
	emitter.sys->addForce(cyclicForce);

	emitter.setDamping(0.878);
	emitter.setMass(10);
	emitter.setParticleRadius(3.475);
	emitter.setVelocity(ofVec3f(0, 0, 0));
	emitter.setOneShot(true);
	emitter.setEmitterType(DirectionalEmitter);
	emitter.setGroupSize(500);
	emitter.setRandomLife(true);
	emitter.setLifespanRange(ofVec2f(0.18, 0.6));

	// Create Forces
	//
	turbForce = new TurbulenceForce(ofVec3f(0, 0, 0), ofVec3f(0, 0, 0));
	gravityForce = new GravityForce(ofVec3f(0, 0, 0));
	radialForce = new ImpulseRadialForce(1000);
	cyclicForce = new CyclicForce(0);

	// set up the emitter for the explosion
	// 
	radialForce->setHeight(0.2);

	explosionEmitter.sys->addForce(turbForce);
	explosionEmitter.sys->addForce(gravityForce);
	explosionEmitter.sys->addForce(radialForce);
	explosionEmitter.sys->addForce(cyclicForce);

	explosionEmitter.setDamping(0.99);
	explosionEmitter.setMass(1);
	explosionEmitter.setParticleRadius(5);
	explosionEmitter.setVelocity(ofVec3f(0, 0, 0));
	explosionEmitter.setOneShot(true);
	explosionEmitter.setEmitterType(RadialEmitter);
	explosionEmitter.setGroupSize(10000);
	explosionEmitter.setRandomLife(true);
	explosionEmitter.setLifespanRange(ofVec2f(1, 5));


	// setup rudimentary lighting 
	//
	initLightingAndMaterials();


	// Setup 3 - Light System
	// 
	lightTopRight.setup();
	lightTopRight.enable();
	lightTopRight.setAreaLight(1, 1);
	lightTopRight.setAmbientColor(ofFloatColor(0.1, 0.1, 0.1));
	lightTopRight.setDiffuseColor(ofFloatColor(1, 1, 1));
	lightTopRight.setSpecularColor(ofFloatColor(1, 1, 1));
	lightTopRight.rotate(45, ofVec3f(0, 1, 0));
	lightTopRight.rotate(-45, ofVec3f(1, 0, 0));
	lightTopRight.setPosition(100, 50, -100);

	lightBttmLeft.setup();
	lightBttmLeft.enable();
	lightBttmLeft.setAreaLight(1, 1);
	lightBttmLeft.setAmbientColor(ofFloatColor(0.1, 0.1, 0.1));
	lightBttmLeft.setDiffuseColor(ofFloatColor(1, 1, 1));
	lightBttmLeft.setSpecularColor(ofFloatColor(1, 1, 1));
	lightBttmLeft.rotate(45, ofVec3f(0, 1, 0));
	lightBttmLeft.rotate(-45, ofVec3f(1, 0, 0));
	lightBttmLeft.setPosition(-100, 50, 100);

	lightMountains.setup();
	lightMountains.enable();
	lightMountains.setAreaLight(1, 1);
	lightMountains.setAmbientColor(ofFloatColor(0.1, 0.1, 0.1));
	lightMountains.setDiffuseColor(ofFloatColor(1, 1, 1));
	lightMountains.setSpecularColor(ofFloatColor(1, 1, 1));
	lightMountains.rotate(45, ofVec3f(0, 1, 0));
	lightMountains.rotate(-45, ofVec3f(1, 0, 0));
	lightMountains.setPosition(-100, 50, -150);


	//mars
	//mars.loadModel("geo/mars-low-5x-v2.obj");
	
	//moon testing
	mars.loadModel("geo/moon-houdini.obj");
	mars.setScaleNormalization(false);

	//background
	if (!screenBackground.load("images/starry.jpg"))
	{
		cout << "Can't find image" << endl;
		ofExit();
	}

	//  setup sounds 
	if (!engineSound.load("sounds/thrusters-loop.wav"))
	{
		cout << "Can't find image" << endl;
		ofExit();
	}
	else {
		
	}
	engineSound.setLoop(true);

	//Particle Physics for the player
	player = new Particle();
	playerSys = new ParticleSystem();

	player->position = ofVec3f(0, 20, 0);
	player->radius = 1;
	player->lifespan = 100000;
	playerSys->add(*player);
	playerSys->addForce(new GravityForce(ofVec3f(0, -1, 0)));
	playerSys->addForce(new TurbulenceForce(ofVec3f(-3, -1, -1), ofVec3f(3, 1, 1)));


	// create sliders for testing
	//
	gui.setup();
	gui.add(numLevels.setup("Number of Octree Levels", 1, 1, 10));
	gui.add(bTimingInfo.setup("Timing Info", false));
	//cout << bTimingInfo;
	bHide = false;

	//  Create Octree for testing.
	//
	//start time?
	//ofTime octreeTime;
	startTime = ofGetElapsedTimeMillis();
	octree.create(mars.getMesh(0), 20);
	cout << "Octree build time: " << startTime << " milliseconds" << endl;
	//end time??


	cout << "Number of Verts: " << mars.getMesh(0).getNumVertices() << endl;

	testBox = Box(Vector3(3, 3, 0), Vector3(5, 5, 2));


	color[0] = ofColor::red;
	color[1] = ofColor::orange;
	color[2] = ofColor::yellow;
	color[3] = ofColor::green;
	color[4] = ofColor::blue;
	color[5] = ofColor::indigo;
	color[6] = ofColor::violet;
	color[7] = ofColor::teal;
	color[8] = ofColor::pink;
	color[9] = ofColor::lime;
}

// load vertex buffer in preparation for rendering
//
void ofApp::loadVbo() {
	if (emitter.sys->particles.size() < 1) return;

	vector<ofVec3f> sizes;
	vector<ofVec3f> points;
	for (int i = 0; i < emitter.sys->particles.size(); i++) {
		points.push_back(emitter.sys->particles[i].position);
		sizes.push_back(ofVec3f(5));
	}
	// upload the data to the vbo
	//
	int total = (int)points.size();
	if (!points.empty() && !sizes.empty()) {
		vbo.clear();
		vbo.setVertexData(&points[0], total, GL_STATIC_DRAW);
		vbo.setNormalData(&sizes[0], total, GL_STATIC_DRAW);
	}
}

// load vertex buffer in preparation for rendering
//
void ofApp::loadVboExplosion() {
	if (explosionEmitter.sys->particles.size() < 1) return;

	vector<ofVec3f> sizes;
	vector<ofVec3f> points;
	for (int i = 0; i < explosionEmitter.sys->particles.size(); i++) {
		points.push_back(explosionEmitter.sys->particles[i].position);
		sizes.push_back(ofVec3f(5));
	}
	// upload the data to the vbo
	//
	int total = (int)points.size();
	if (!points.empty() && !sizes.empty()) {
		vbo.clear();
		vbo.setVertexData(&points[0], total, GL_STATIC_DRAW);
		vbo.setNormalData(&sizes[0], total, GL_STATIC_DRAW);
	}
}
 
//--------------------------------------------------------------
// incrementally update scene (animation)
//
void ofApp::update() {

	//Integration of physics
	if (bLanderLoaded) {
		integrate();
		//Player particle
		playerSys->update();
		checkCollisions();

		//camera
		trackCam.setPosition(lander.getPosition().x, lander.getPosition().y, lander.getPosition().z+20);
		landerCam.setPosition(lander.getPosition().x, lander.getPosition().y, lander.getPosition().z);

		//// check thrusters for sound
		if (bThrust && !engineSound.isPlaying()) {
			engineSound.play();
		}
		else if (!bThrust && engineSound.isPlaying()) {
			engineSound.stop();
		}
		emitter.position = lander.getPosition();
		emitter.update();
		explosionEmitter.position = lander.getPosition();
		explosionEmitter.update();

		//cout << endl << "Lander velocity" << velocity;
	}
	//Zero out the forces
	force = glm::vec3(0, 0, 0);

	
	
}
//--------------------------------------------------------------
void ofApp::draw() {

	//loadVbo();
	//loadVboExplosion();

	ofBackground(ofColor::black);

	//Drawing the screen background
	glDepthMask(false);
	screenBackground.draw(0, 0);
	glDepthMask(true);


	glDepthMask(false);
	if (!bHide) gui.draw();
	glDepthMask(true);
	
	// this makes everything look glowy :)
	//
	ofEnableBlendMode(OF_BLENDMODE_ADD);
	ofEnablePointSprites();

	
	theCam->begin();
	ofPushMatrix();
	if (bWireframe) {                    // wireframe mode  (include axis)
		ofDisableLighting();
		ofSetColor(ofColor::slateGray);
		mars.drawWireframe();
		if (bLanderLoaded) {
			lander.drawWireframe();
			if (!bTerrainSelected) drawAxis(lander.getPosition());
		}
		if (bTerrainSelected) drawAxis(ofVec3f(0, 0, 0));
	}
	else {
		ofEnableLighting();              // shaded mode
		//drawing light
		//lightTopRight.draw(); 
		//lightBttmLeft.draw();
		//ylightMountains.draw();

		mars.drawFaces();
		ofMesh mesh;

		

		if (bLanderLoaded) {
			lander.drawFaces();

			//player particle
			//
			//playerSys->draw();
			player->position = lander.getPosition();

			//shader.begin();
			//emitter for the rocket
			// draw particle emitter here..
			//
			//	emitter.draw();
			//particleTex.bind();
			//vbo.draw(GL_POINTS, 0, (int)emitter.sys->particles.size());
			//vbo.draw(GL_POINTS, 0, (int)explosionEmitter.sys->particles.size());
			//particleTex.unbind();
			//shader.end();

			//maybe add light for lander here??

			if (!bTerrainSelected) drawAxis(lander.getPosition());
			if (bDisplayBBoxes) {
				ofNoFill();
				ofSetColor(ofColor::white);
				for (int i = 0; i < lander.getNumMeshes(); i++) {
					ofPushMatrix();
					ofMultMatrix(lander.getModelMatrix());
					ofRotate(-90, 1, 0, 0);
					Octree::drawBox(bboxList[i]);
					ofPopMatrix();
				}
			}

			if (bLanderSelected) {

				ofVec3f min = lander.getSceneMin() + lander.getPosition();
				ofVec3f max = lander.getSceneMax() + lander.getPosition();

				Box bounds = Box(Vector3(min.x, min.y, min.z), Vector3(max.x, max.y, max.z));
				ofSetColor(ofColor::white);
				ofNoFill();
				Octree::drawBox(bounds);

				// draw colliding boxes
				//
				ofSetColor(ofColor::lime);
				for (int i = 0; i < colBoxList.size(); i++) {
					
					Octree::drawBox(colBoxList[i]);
					
				}
			}
		}
	}
	if (bTerrainSelected) drawAxis(ofVec3f(0, 0, 0));



	if (bDisplayPoints) {                // display points as an option    
		glPointSize(3);
		ofSetColor(ofColor::green);
		mars.drawVertices();
	}

	// highlight selected point (draw sphere around selected point)
	//
	if (bPointSelected) {
		ofSetColor(ofColor::blue);
		ofDrawSphere(selectedPoint, .1);
	}


	// recursively draw octree
	//
	ofDisableLighting();
	int level = 0;
	//	ofNoFill();

	if (bDisplayLeafNodes) {
		octree.drawLeafNodes(octree.root);
		cout << "num leaf: " << octree.numLeaf << endl;
    }
	else if (bDisplayOctree) {
		ofNoFill();
		ofSetColor(color[numLevels - 1]);
		octree.draw(numLevels, 0);
	}

	// if point selected, draw a sphere
	//
	if (pointSelected) {
		ofVec3f p = octree.mesh.getVertex(selectedNode.points[0]);
		ofVec3f d = p - theCam->getPosition();
		ofSetColor(ofColor::lightGreen);
		ofDrawSphere(p, .02 * d.length());
	}

	/*ofSetColor(ofColor::white);
	ofNoFill();
	Octree::drawBox(testBox);*/

	ofPopMatrix();
	theCam->end();
	
}


//Integrator for physics
void ofApp::integrate() {

	// init current framerate (or you can use ofGetLastFrameTime())
	//
	float framerate = ofGetFrameRate();
	if (framerate == 0)
		framerate = 1;
	float dt = 1.0 / framerate;

	
	if (bLanderSelected) { //Quick Janky fix to a bug when you select the lander with the mouse
		glm::vec3 playerParticlePos = playerSys->particles[0].position;
		playerParticlePos += (velocity * dt); //gravity + thrusters
		glm::vec3 pos = lander.getPosition();
		pos += (velocity * dt);
		lander.setPosition(pos.x, pos.y, pos.z);
		playerSys->particles[0].position = ofVec3f(pos.x, pos.y, pos.z);
		glm::vec3 accel = acceleration;
		accel += (force * 1.0 / mass);
		velocity += accel * dt;
		velocity *= damping;
	}
	else { //Main Integrate
	// linear motion
	//
	glm::vec3 playerParticlePos = playerSys->particles[0].position; //uses gravity and turbulence forces
	glm::vec3 pos = lander.getPosition();
	pos += (velocity * dt);
	playerParticlePos += (velocity * dt); //gravity + thrusters
	//cout << endl << "Lander velocity" << velocity;
	lander.setPosition(pos.x, playerParticlePos.y, pos.z); //lander uses gravity in the y position
	playerSys->particles[0].position = ofVec3f(pos.x, playerParticlePos.y, pos.z); //I want the particle to follow my lander
	glm::vec3 accel = acceleration;
	accel += (force * 1.0 / mass);
	velocity += accel * dt;
	velocity *= damping;

	// angular motion
	//
	rotation += (angularVelocity * dt);
	lander.setRotation(0, glm::radians(rotation), 0, rotation, 0);
	float a = angularAcceleration;;
	a += (angularForce * 1.0 / mass);
	angularVelocity += a * dt;
	angularVelocity *= damping;
	}

}

void ofApp::checkCollisions() {

	// for each particle, determine if we hit the groud plane.
	//
	for (int i = 0; i < playerSys->particles.size(); i++) {

		// only bother to check for descending particles.
		//
		ofVec3f vel = playerSys->particles[i].velocity; // velocity of particle
		if (vel.y >= 0) break;                             // ascending;

		ofVec3f pos = playerSys->particles[i].position;


		//
		ofVec3f min = lander.getSceneMin() + lander.getPosition();
		ofVec3f max = lander.getSceneMax() + lander.getPosition();

		Box bounds = Box(Vector3(min.x, min.y, min.z), Vector3(max.x, max.y, max.z));

		colBoxList.clear();
		octree.intersect(bounds, octree.root, colBoxList);

		//Once the lander hits the ground then apply a bit of impulse force
		for (int j = 0; j < colBoxList.size();j++) {
			if (bounds.overlap(colBoxList[j])) {




				
				ofVec3f norm = ofVec3f(0, 1, 0);  // just use vertical for normal for now
				ofVec3f f = (restitution + 1.0) * ((-vel.dot(norm)) * norm);

				cout << endl << "vel" << vel;
				
				if (vel.y <= -1.2) {
					explosionEmitter.sys->reset();
					explosionEmitter.start();
				}
				

				playerSys->particles[i].forces += ofGetFrameRate() * f;
			}
		}
		
	}


}

void ofApp::altitudeSensor() {
	


}


// 
// Draw an XYZ axis in RGB at world (0,0,0) for reference.
//
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


void ofApp::keyPressed(int key) {

	switch (key) {
	case 'B':
	case 'b':
		bDisplayBBoxes = !bDisplayBBoxes;
		break;
	case 'C':
	case 'c':
		if (cam.getMouseInputEnabled()) cam.disableMouseInput();
		else cam.enableMouseInput();
		break;
	case 'F':
	case 'f':
		ofToggleFullscreen();
		break;
	case 'H':
	case 'h':
		break;
	case 'L':
	case 'l':
		bDisplayLeafNodes = !bDisplayLeafNodes;
		break;
	case 'O':
	case 'o':
		bDisplayOctree = !bDisplayOctree;
		break;
	case 'r':
		cam.reset();
		break;
	case 's':
		savePicture();
		break;
	case 't':
		setCameraTarget();
		break;
	case 'u':
		break;
	case 'v':
		togglePointsDisplay();
		break;
	case 'V':
		break;
	case 'w':
		toggleWireframeMode();
		break;
	case OF_KEY_ALT:
		cam.enableMouseInput();
		bAltKeyDown = true;
		break;
	case OF_KEY_CONTROL:
		bCtrlKeyDown = true;
		break;
	case OF_KEY_SHIFT:
		break;
	case OF_KEY_DEL:
		break;
	case OF_KEY_F1:
		theCam = &cam;
		break;
	case OF_KEY_F2:
		theCam = &distanceShot;
		break;
	case OF_KEY_F3:
		theCam = &trackCam;
		break;
	case OF_KEY_F4:
		theCam = &landerCam;
		break;

	//Lander movement
	//
	case OF_KEY_LEFT:   // turn left
		bThrust = true;
		angularForce = 1000.0;
		emitter.sys->reset();
		emitter.start();
		break;
	case OF_KEY_RIGHT:  // turn right
		bThrust = true;
		angularForce = -1000.0;
		emitter.sys->reset();
		emitter.start();
		break;
	case OF_KEY_UP:     // go forward
		bThrust = true;
		//	engineSound.play();
		angularVelocity = 0;
		force = heading() * -float(50);
		emitter.sys->reset();
		emitter.start();
		break;
	case OF_KEY_DOWN:   // go backward
		bThrust = true;
		angularVelocity = 0;
		force = heading() * float(50);
		emitter.sys->reset();
		emitter.start();
		break;
	case ' ':   // thrust upwards
		bThrust = true;
		angularVelocity = 0;
		force = headingZ() * float(10);
		emitter.sys->reset();
		emitter.start();
		break;
	case '/':   // thrust downwards
		bThrust = true;
		angularVelocity = 0;
		force = headingZ() * float(-10);
		emitter.sys->reset();
		emitter.start();
		break;
	default:
		break;
	}
}

void ofApp::toggleWireframeMode() {
	bWireframe = !bWireframe;
}

void ofApp::toggleSelectTerrain() {
	bTerrainSelected = !bTerrainSelected;
}

void ofApp::togglePointsDisplay() {
	bDisplayPoints = !bDisplayPoints;
}

void ofApp::keyReleased(int key) {

	switch (key) {
	
	case OF_KEY_ALT:
		cam.disableMouseInput();
		bAltKeyDown = false;
		break;
	case OF_KEY_CONTROL:
		bCtrlKeyDown = false;
		break;
	case OF_KEY_SHIFT:
		break;

	//Lander Movement
		//
	case OF_KEY_LEFT:   // turn left
		bThrust = false;
		angularForce = 0;
		break;
	case OF_KEY_RIGHT:  // turn right
		bThrust = false;
		angularForce = 0;
		break;
	case OF_KEY_UP:     // go forward
		bThrust = false;
		force = glm::vec3(0, 0, 0);
		break;
	case OF_KEY_DOWN:   // go backward
		bThrust = false;
		force = glm::vec3(0, 0, 0);
		break;
	case ' ':   // thrust upwards
		bThrust = false;
		force = glm::vec3(0, 0, 0);
		break;
	case '/':   // thrust downwards
		bThrust = false;
		force = glm::vec3(0, 0, 0);
		break;
	default:
		break;

	}


}



//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

	
}


//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button) {

	// if moving camera, don't allow mouse interaction
	//
	if (cam.getMouseInputEnabled()) return;

	// if moving camera, don't allow mouse interaction
//
	if (cam.getMouseInputEnabled()) return;

	// if rover is loaded, test for selection
	//
	if (bLanderLoaded) {
		glm::vec3 origin = cam.getPosition();
		glm::vec3 mouseWorld = cam.screenToWorld(glm::vec3(mouseX, mouseY, 0));
		glm::vec3 mouseDir = glm::normalize(mouseWorld - origin);

		ofVec3f min = lander.getSceneMin() + lander.getPosition();
		ofVec3f max = lander.getSceneMax() + lander.getPosition();

		Box bounds = Box(Vector3(min.x, min.y, min.z), Vector3(max.x, max.y, max.z));
		
		bool hit = bounds.intersect(Ray(Vector3(origin.x, origin.y, origin.z), Vector3(mouseDir.x, mouseDir.y, mouseDir.z)), 0, 10000);
		if (hit) {
			
			
			bLanderSelected = true;
			mouseDownPos = getMousePointOnPlane(lander.getPosition(), cam.getZAxis());
			mouseLastPos = mouseDownPos;
			bInDrag = true;
		}
		else {
			bLanderSelected = false;
		}
	}
	else {
		//start time
		startTime = ofGetElapsedTimeMillis();
		//cout << endl << "start: " << startTime;
		ofVec3f p;
		raySelectWithOctree(p);
		//end time
		endTime = ofGetElapsedTimeMillis(); //same as start time....
		//cout << endl << "end: " << endTime;
		if (bTimingInfo) {
			cout << "Ray Selection search time: " << endTime-startTime << " milliseconds" << endl;
		}
	}
}

bool ofApp::raySelectWithOctree(ofVec3f &pointRet) {
	ofVec3f mouse(mouseX, mouseY);
	ofVec3f rayPoint = cam.screenToWorld(mouse);
	ofVec3f rayDir = rayPoint - cam.getPosition();
	rayDir.normalize();
	Ray ray = Ray(Vector3(rayPoint.x, rayPoint.y, rayPoint.z),
		Vector3(rayDir.x, rayDir.y, rayDir.z));

	pointSelected = octree.intersect(ray, octree.root, selectedNode);

	if (pointSelected) {
		pointRet = octree.mesh.getVertex(selectedNode.points[0]);
	}
	return pointSelected;
}




//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button) {

	// if moving camera, don't allow mouse interaction
	//
	if (cam.getMouseInputEnabled()) return;

	if (bInDrag) {

		glm::vec3 landerPos = lander.getPosition();

		glm::vec3 mousePos = getMousePointOnPlane(landerPos, cam.getZAxis());
		glm::vec3 delta = mousePos - mouseLastPos;
	
		landerPos += delta;
		lander.setPosition(landerPos.x, landerPos.y, landerPos.z);
		mouseLastPos = mousePos;

		ofVec3f min = lander.getSceneMin() + lander.getPosition();
		ofVec3f max = lander.getSceneMax() + lander.getPosition();

		Box bounds = Box(Vector3(min.x, min.y, min.z), Vector3(max.x, max.y, max.z));

		colBoxList.clear();
		octree.intersect(bounds, octree.root, colBoxList);
		//cout << endl << colBoxList.size();

		//for (int i = 0; i < colBoxList.size();i++) {
		//	if (bounds.overlap(colBoxList[i])) {
		//		cout << "overlap" << endl;
		//	}
		//	else {
		//		cout << "OK" << endl;
		//	}
		//}



	}
	else {
		ofVec3f p;
		//startTime
		startTime = ofGetElapsedTimeMillis();
		raySelectWithOctree(p);
		//end time
		endTime = ofGetElapsedTimeMillis(); //same as start time....
		//cout << endl << "end: " << endTime;
		if (bTimingInfo) {
			cout << "Ray Selection search time: " << endTime - startTime << " milliseconds" << endl;
		}
	}
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button) {
	bInDrag = false;
}



// Set the camera to use the selected point as it's new target
//  
void ofApp::setCameraTarget() {

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
// setup basic ambient lighting in GL  (for now, enable just 1 light)
//
void ofApp::initLightingAndMaterials() {

	static float ambient[] =
	{ .5f, .5f, .5, 1.0f };
	static float diffuse[] =
	{ 1.0f, 1.0f, 1.0f, 1.0f };

	static float position[] =
	{5.0, 5.0, 5.0, 0.0 };

	static float lmodel_ambient[] =
	{ 1.0f, 1.0f, 1.0f, 1.0f };

	static float lmodel_twoside[] =
	{ GL_TRUE };


	glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);
	glLightfv(GL_LIGHT0, GL_POSITION, position);

	glLightfv(GL_LIGHT1, GL_AMBIENT, ambient);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, diffuse);
	glLightfv(GL_LIGHT1, GL_POSITION, position);


	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, lmodel_ambient);
	glLightModelfv(GL_LIGHT_MODEL_TWO_SIDE, lmodel_twoside);

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
//	glEnable(GL_LIGHT1);
	glShadeModel(GL_SMOOTH);
} 

void ofApp::savePicture() {
	ofImage picture;
	picture.grabScreen(0, 0, ofGetWidth(), ofGetHeight());
	picture.save("screenshot.png");
	cout << "picture saved" << endl;
}

//--------------------------------------------------------------
//
// support drag-and-drop of model (.obj) file loading.  when
// model is dropped in viewport, place origin under cursor
//
void ofApp::dragEvent2(ofDragInfo dragInfo) {

	ofVec3f point;
	mouseIntersectPlane(ofVec3f(0, 0, 0), cam.getZAxis(), point);
	if (lander.loadModel(dragInfo.files[0])) {
		lander.setScaleNormalization(false);
//		lander.setScale(.1, .1, .1);
	//	lander.setPosition(point.x, point.y, point.z);
		lander.setPosition(1, 1, 0);

		bLanderLoaded = true;
		for (int i = 0; i < lander.getMeshCount(); i++) {
			bboxList.push_back(Octree::meshBounds(lander.getMesh(i)));
		}

		cout << "Mesh Count: " << lander.getMeshCount() << endl;
	}
	else cout << "Error: Can't load model" << dragInfo.files[0] << endl;
}

bool ofApp::mouseIntersectPlane(ofVec3f planePoint, ofVec3f planeNorm, ofVec3f &point) {
	ofVec2f mouse(mouseX, mouseY);
	ofVec3f rayPoint = cam.screenToWorld(glm::vec3(mouseX, mouseY, 0));
	ofVec3f rayDir = rayPoint - cam.getPosition();
	rayDir.normalize();
	return (rayIntersectPlane(rayPoint, rayDir, planePoint, planeNorm, point));
}

//--------------------------------------------------------------
//
// support drag-and-drop of model (.obj) file loading.  when
// model is dropped in viewport, place origin under cursor
//
void ofApp::dragEvent(ofDragInfo dragInfo) {
	if (lander.loadModel(dragInfo.files[0])) {
		bLanderLoaded = true;
		lander.setScaleNormalization(false);
		lander.setPosition(0, 0, 0);
		cout << "number of meshes: " << lander.getNumMeshes() << endl;
		bboxList.clear();
		for (int i = 0; i < lander.getMeshCount(); i++) {
			bboxList.push_back(Octree::meshBounds(lander.getMesh(i)));
		}

		//		lander.setRotation(1, 180, 1, 0, 0);

				// We want to drag and drop a 3D object in space so that the model appears 
				// under the mouse pointer where you drop it !
				//
				// Our strategy: intersect a plane parallel to the camera plane where the mouse drops the model
				// once we find the point of intersection, we can position the lander/lander
				// at that location.
				//

				// Setup our rays
				//
		glm::vec3 origin = cam.getPosition();
		glm::vec3 camAxis = cam.getZAxis();
		glm::vec3 mouseWorld = cam.screenToWorld(glm::vec3(mouseX, mouseY, 0));
		glm::vec3 mouseDir = glm::normalize(mouseWorld - origin);
		float distance;

		bool hit = glm::intersectRayPlane(origin, mouseDir, glm::vec3(0, 0, 0), camAxis, distance);
		if (hit) {
			// find the point of intersection on the plane using the distance 
			// We use the parameteric line or vector representation of a line to compute
			//
			// p' = p + s * dir;
			//
			glm::vec3 intersectPoint = origin + distance * mouseDir;

			// Now position the lander's origin at that intersection point
			//
			glm::vec3 min = lander.getSceneMin();
			glm::vec3 max = lander.getSceneMax();
			float offset = (max.y - min.y) / 2.0;
			lander.setPosition(intersectPoint.x, intersectPoint.y - offset, intersectPoint.z);

			// set up bounding box for lander while we are at it
			//
			landerBounds = Box(Vector3(min.x, min.y, min.z), Vector3(max.x, max.y, max.z));
		}
	}


}

//  intersect the mouse ray with the plane normal to the camera 
//  return intersection point.   (package code above into function)
//
glm::vec3 ofApp::getMousePointOnPlane(glm::vec3 planePt, glm::vec3 planeNorm) {
	// Setup our rays
	//
	glm::vec3 origin = cam.getPosition();
	glm::vec3 camAxis = cam.getZAxis();
	glm::vec3 mouseWorld = cam.screenToWorld(glm::vec3(mouseX, mouseY, 0));
	glm::vec3 mouseDir = glm::normalize(mouseWorld - origin);
	float distance;

	bool hit = glm::intersectRayPlane(origin, mouseDir, planePt, planeNorm, distance);

	if (hit) {
		// find the point of intersection on the plane using the distance 
		// We use the parameteric line or vector representation of a line to compute
		//
		// p' = p + s * dir;
		//
		glm::vec3 intersectPoint = origin + distance * mouseDir;

		return intersectPoint;
	}
	else return glm::vec3(0, 0, 0);
}
