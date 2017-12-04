#pragma once
#include "ofMain.h"
#include "ofxJSON.h"
#include "ofxOsc.h"

#define HOST "192.168.100.255"
#define PORT 9000


struct telemetryData{
	int gear;
	int rpm;
	int speedKmh;
	int drsAvailable;
	int drsEnabled;
	float normalizedCarPosition;

	float heading;	// heading of the car on world coord
	float pitch;	// pitch of the car on world coord
	float roll;		// roll of the car on world coord
	ofVec3f carCoordinates;		// car position on world coord(x,y,z)
	ofVec3f accG;
	ofVec3f velocity;
	ofVec4f tireTemp;

	float throttle;
	float brake;
	float clutch;
	float steerAngle;
	int iCurrentTime;
};

class Telemetry {

public:
	Telemetry();
	~Telemetry();

	int completedLaps;
	string lastTime;
	string bestTime;
	float lastLapTimeMillis;
	float bestLapTimeMillis;

	float maxTorque;
	int maxRpm;
	int hasDRS;
	int hasERS;

	// telemetry Buffer
	vector<telemetryData> telemetryBuffers;

	void push2Telemetry(telemetryData data);
	void export2Json();
	
	void importJson(string fileName);
	bool isFileLoaded = false;

	void playLog();
	uint64_t lastPlayedTimer;
	unsigned int logPlayHead;


	void sendIntTelemetryMessage(string addressEndpoint, int value);
	void sendFloatTelemetryMessage(string addressEndpoint, float value);

	void sendStringTelemetryMessage(string addressEndpoint, string value);
	void sendBoolTelemetryMessage(string addressEndpoint, bool value);
	
	ofxOscSender sender;
};

