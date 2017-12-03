#pragma once
#include "ofMain.h"
#include "ofxJSON.h"

struct telemetryData{
	int gear;
	int rpm;
	int speedKmh;
	int drsAvailable;
	int drsEnabled;

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
};

class Telemetry {

public:
	Telemetry();
	~Telemetry();

	int completedLaps;
	float lastLapTimeMillis;
	float bestLapTimeMillis;

	// telemetry Buffer
	vector<telemetryData> telemetryBuffers;

	void push2Telemetry(telemetryData data);
	void export2Json();




};

