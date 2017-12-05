#pragma once

#define HOST "192.16/8.100.255"
//#define HOST "localhost"
#define PORT 9000

#include "ofMain.h"
#include "ofxSharedMemory.h"
#include "ofxOSC.h"
#include "assettoCorsa.h"
//#include "r3e.h"
//#include "utils.h"
//#include <atlconv.h>
#include "telemetry.h"

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
	void dragEvent(ofDragInfo dragInfo);
	void gotMessage(ofMessage msg);

	bool bRecordOn;
	

	/* ACS Telemetry */
	ofxSharedMemory<SPageFilePhysics*> acsPhysics;
	SPageFilePhysics* acsPhysicsData;

	ofxSharedMemory<SPageFileGraphic*> acsGraphics;
	SPageFileGraphic* acsGraphicsData;

	ofxSharedMemory<SPageFileStatic*> acsFileStatic;
	SPageFileStatic* acsStaticData;

	bool isAcsConnected = false;
	bool isAcsPhysicsConnected = false, isAcsGraphicsConnected = false, isAcsStaticConnected = false;
	
	//stringstream acsPhysicsOutput, acsGraphicsOutput, acsFileStaticOutput;

	void printAcsPhysics(float x, float y);
	void printAcsGraphics(float x, float y);
	void printAcsStatic(float x, float y);

	/* osc */
	ofxOscSender sender;
	uint64_t lastOscSentTimer;

	void sendOscMessage();

	void sendIntTelemetryMessage(string addressEndpoint, int value);
	void sendFloatTelemetryMessage(string addressEndpoint, float value);

	void sendStringTelemetryMessage(string addressEndpoint, string value);
	void sendBoolTelemetryMessage(string addressEndpoint, bool value);

	/* util */
	string	wstring2string(wstring wstr);
	wstring string2wstring(string str);

	telemetryData tData;
	Telemetry telemetry;
	ofPolyline carPos;
	
	void updateStaticData();

	bool bPlayLogOn = false;

};
