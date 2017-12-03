#pragma once
#include "ofMain.h"
#include "ofxJSON.h"

struct telemetryData{
	int gear;
	int rpm;
	int speedKmh;
	int drsAvailable;
	int drsEnabled;
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

	vector<telemetryData> telemetryBuffers;

	// car status
	//vector<int> gear;
	//vector<int> rpms;
	//vector<int> speedKmh;
	//vector<bool> drsAvailable;
	//vector<bool> drsEnabled;
	//vector<ofVec3f> accG;
	//vector<ofVec3f> velocity;
	//vector<ofVec4f> tireTemp;

	//// controller information
	//vector<float> throttle;
	//vector<float> brake;
	//vector<float> clutch;
	//vector<float> steerAngle;

	void push2Telemetry(telemetryData data);
	void export2Json();
	/*void push2Telemetry(int _gear,
		int _rpms,
		int _speedKmh,
		int _drsAvailable,
		int _drsEnabled,
		ofVec3f _accG,
		ofVec3f _velocity,
		ofVec4f _tireTemp,

		float _throttle,
		float _brake,
		float _clutch,
		float _steerAngle
	) {
		gear.push_back(_gear);
		rpms.push_back(_rpms);
		speedKmh.push_back(_speedKmh);
		drsAvailable.push_back(_drsAvailable);
		drsEnabled.push_back(_drsEnabled);
		accG.push_back(_accG);
		velocity.push_back(_velocity);
		tireTemp.push_back(_tireTemp);
		throttle.push_back(_throttle);
		brake.push_back(_brake);
		clutch.push_back(_clutch);
		steerAngle.push_back(_steerAngle);
	}*/



};

