#include "Telemetry.h"



Telemetry::Telemetry() {
	sender.setup(HOST, PORT);
}


Telemetry::~Telemetry() {
}

void Telemetry::push2Telemetry(telemetryData data) {
	telemetryBuffers.push_back(data);
}

void Telemetry::export2Json() {

	string file = "telemetryLogs/log.json";

	ofxJSONElement json;

	json["staticData"]["playerName"] = tStaticData.playerName;
	json["staticData"]["carInfo"] = tStaticData.carInfo;
	json["staticData"]["trackName"] = tStaticData.trackInfo;

	json["staticData"]["maxTorque"] = tStaticData.maxTorque;
	json["staticData"]["maxRPM"] = tStaticData.maxRpm;
	json["staticData"]["hasDRS"] = tStaticData.hasDRS;
	json["staticData"]["hasERS"] = tStaticData.hasERS;


	for (int i = 0; i < telemetryBuffers.size(); i++) {
		json["telemetry"][i]["lastLapTimeMillis"] = tStaticData.lastLapTimeMillis;
		json["telemetry"][i]["bestLapTimeMillis"] = tStaticData.bestLapTimeMillis;
		json["telemetry"][i]["laps"] = telemetryBuffers[i].completedLaps + 1;
		json["telemetry"][i]["iCurrentTime"] = telemetryBuffers[i].iCurrentTime;
		json["telemetry"][i]["frameNo"] = i + 1;
		json["telemetry"][i]["gear"] = telemetryBuffers[i].gear;
		json["telemetry"][i]["rpm"] = telemetryBuffers[i].rpm;
		json["telemetry"][i]["speedKmh"] = telemetryBuffers[i].speedKmh;
		json["telemetry"][i]["drsAvailable"] = telemetryBuffers[i].drsAvailable;
		json["telemetry"][i]["drsEnabled"] = telemetryBuffers[i].drsEnabled;
		json["telemetry"][i]["normalizedCarPosition"] = telemetryBuffers[i].normalizedCarPosition;
		
		/*
		float heading;	// heading of the car on world coord
		float pitch;	// pitch of the car on world coord
		float roll;		// roll of the car on world coord
		ofVec3f carCoordinates;		// car position on world coord(x,y,z)								
		*/

		json["telemetry"][i]["heading"] = telemetryBuffers[i].heading;
		json["telemetry"][i]["pitch"] = telemetryBuffers[i].pitch;
		json["telemetry"][i]["roll"] = telemetryBuffers[i].roll;

		json["telemetry"][i]["carCoordinates"]["x"] = telemetryBuffers[i].carCoordinates.x;
		json["telemetry"][i]["carCoordinates"]["y"] = telemetryBuffers[i].carCoordinates.y;
		json["telemetry"][i]["carCoordinates"]["z"] = telemetryBuffers[i].carCoordinates.z;


		json["telemetry"][i]["accG"]["x"] = telemetryBuffers[i].accG.x;
		json["telemetry"][i]["accG"]["y"] = telemetryBuffers[i].accG.y;
		json["telemetry"][i]["accG"]["z"] = telemetryBuffers[i].accG.z;
										 
		json["telemetry"][i]["velocity"]["x"] = telemetryBuffers[i].velocity.x;
		json["telemetry"][i]["velocity"]["y"] = telemetryBuffers[i].velocity.y;
		json["telemetry"][i]["velocity"]["z"] = telemetryBuffers[i].velocity.z;
										 
		json["telemetry"][i]["tireTemp"]["FL"] = telemetryBuffers[i].tireTemp.x;
		json["telemetry"][i]["tireTemp"]["FR"] = telemetryBuffers[i].tireTemp.y;
		json["telemetry"][i]["tireTemp"]["RL"] = telemetryBuffers[i].tireTemp.z;
		json["telemetry"][i]["tireTemp"]["RR"] = telemetryBuffers[i].tireTemp.w;
										 
		json["telemetry"][i]["clutch"] = telemetryBuffers[i].clutch;
		json["telemetry"][i]["brake"] = telemetryBuffers[i].brake;
		json["telemetry"][i]["throttle"] = telemetryBuffers[i].throttle;
		json["telemetry"][i]["steerAngle"] = telemetryBuffers[i].steerAngle;
	}

	if (!json.save(file, true)) {
		ofLogNotice("save logs") << "log json failed";
	}
}

void Telemetry::importJson(string fileName) {
	telemetryBuffers.clear();

	ofxJSONElement json;
	bool loadSuccessful;

	loadSuccessful = json.open("telemetryLogs/" + fileName + ".json");

	if (loadSuccessful) {
		cout << "file loading!!! " << endl;

		tStaticData.playerName = json["staticData"]["playerName"].asString();
		tStaticData.carInfo = json["staticData"]["carInfo"].asString();
		tStaticData.trackInfo = json["staticData"]["trackName"].asString();

		tStaticData.maxTorque = json["staticData"]["maxTorque"].asFloat();
		tStaticData.maxRpm = json["staticData"]["maxRPM"].asFloat();
		tStaticData.hasDRS = json["staticData"]["hasDRS"].asInt();
		tStaticData.hasERS = json["staticData"]["hasERS"].asInt();


		for (unsigned int i = 0; i < json["telemetry"].size(); i++) {
			telemetryData tData;
			tData.completedLaps = json["telemetry"][i]["laps"].asInt();
			tData.iCurrentTime = json["telemetry"][i]["iCurrentTime"].asInt();
			tData.gear = json["telemetry"][i]["gear"].asInt();
			tData.rpm = json["telemetry"][i]["rpm"].asInt();
			tData.speedKmh = json["telemetry"][i]["speedKmh"].asInt();
			tData.drsAvailable = json["telemetry"][i]["drsAvailable"].asInt();
			tData.drsEnabled = json["telemetry"][i]["drsEnabled"].asInt();
			tData.normalizedCarPosition = json["telemetry"][i]["normalizedCarPosition"].asFloat();

			tData.heading = json["telemetry"][i]["heading"].asFloat();
			tData.pitch = json["telemetry"][i]["pitch"].asFloat();
			tData.roll = json["telemetry"][i]["roll"].asFloat();

			tData.carCoordinates.x = json["telemetry"][i]["carCoordinates"]["x"].asFloat();
			tData.carCoordinates.y = json["telemetry"][i]["carCoordinates"]["y"].asFloat();
			tData.carCoordinates.z = json["telemetry"][i]["carCoordinates"]["z"].asFloat();

			tData.accG.x = json["telemetry"][i]["accG"]["x"].asFloat();
			tData.accG.y = json["telemetry"][i]["accG"]["y"].asFloat();
			tData.accG.z = json["telemetry"][i]["accG"]["z"].asFloat();

			tData.velocity.x = json["telemetry"][i]["velocity"]["x"].asFloat();
			tData.velocity.y = json["telemetry"][i]["velocity"]["y"].asFloat();
			tData.velocity.z = json["telemetry"][i]["velocity"]["z"].asFloat();

			tData.tireTemp.x = json["telemetry"][i]["tireTemp"]["FL"].asFloat();
			tData.tireTemp.y = json["telemetry"][i]["tireTemp"]["FR"].asFloat();
			tData.tireTemp.z = json["telemetry"][i]["tireTemp"]["RL"].asFloat();
			tData.tireTemp.w = json["telemetry"][i]["tireTemp"]["RR"].asFloat();

			tData.clutch = json["telemetry"][i]["clutch"].asFloat();
			tData.brake = json["telemetry"][i]["brake"].asFloat();
			tData.throttle = json["telemetry"][i]["throttle"].asFloat();
			tData.steerAngle = json["telemetry"][i]["steerAngle"].asFloat();

			telemetryBuffers.push_back(tData);
		}

		isFileLoaded = true;
		lastPlayedTimer = ofGetElapsedTimeMillis();
	}
	else {
		ofLog(OF_LOG_ERROR, "log load failed. check fileName");
		isFileLoaded = false;
	}
}

void Telemetry::playLog() {
	if (isFileLoaded) {
//		cout << "play!!!" << endl;
		if (ofGetElapsedTimeMillis() - lastPlayedTimer > 50) {

			// static datas
			// car, track, player
			sendStringTelemetryMessage("/playerInfo/carName", tStaticData.carInfo);
			sendStringTelemetryMessage("/playerInfo/trackName", tStaticData.trackInfo);
			sendStringTelemetryMessage("/playerInfo/playerName", tStaticData.playerName);
//
//			// car info
			sendFloatTelemetryMessage("/carInfo/maxTorque", tStaticData.maxTorque);
			sendFloatTelemetryMessage("/carInfo/maxRPM", tStaticData.maxRpm);
			sendIntTelemetryMessage("/carInfo/hasDRS", tStaticData.hasDRS);
			sendIntTelemetryMessage("/carInfo/hasERS", tStaticData.hasERS);

			// laptime info
			sendIntTelemetryMessage("/telemetry/lastLapTimeMillis", tStaticData.lastLapTimeMillis);
			sendIntTelemetryMessage("/telemetry/bestLapTimeMillis", tStaticData.bestLapTimeMillis);


			sendIntTelemetryMessage("/telemetry/currentLapTimeMillis", telemetryBuffers[logPlayHead].iCurrentTime);
			sendFloatTelemetryMessage("/telemetry/normalizedCarPosition", telemetryBuffers[logPlayHead].normalizedCarPosition);

			// car status
			sendIntTelemetryMessage("/telemetry/completedLaps", telemetryBuffers[logPlayHead].completedLaps);
			sendIntTelemetryMessage("/telemetry/gear", telemetryBuffers[logPlayHead].gear);
			sendIntTelemetryMessage("/telemetry/rpms", telemetryBuffers[logPlayHead].rpm);
			sendFloatTelemetryMessage("/telemetry/speedKmh", telemetryBuffers[logPlayHead].speedKmh);

			sendFloatTelemetryMessage("/telemetry/heading", telemetryBuffers[logPlayHead].heading);
			sendFloatTelemetryMessage("/telemetry/pitch", telemetryBuffers[logPlayHead].pitch);
			sendFloatTelemetryMessage("/telemetry/roll", telemetryBuffers[logPlayHead].roll);

			//sendFloatTelemetryMessage("/telemetry/carCoordinates/x", telemetryBuffers[logPlayHead].carCoordinates.x);
			//sendFloatTelemetryMessage("/telemetry/carCoordinates/y", telemetryBuffers[logPlayHead].carCoordinates.y);
			//sendFloatTelemetryMessage("/telemetry/carCoordinates/z", telemetryBuffers[logPlayHead].carCoordinates.z);
			sendVec3fTelemetryMessage("/telemetry/carCoordinates", telemetryBuffers[logPlayHead].carCoordinates.x, telemetryBuffers[logPlayHead].carCoordinates.y, telemetryBuffers[logPlayHead].carCoordinates.z);
			
			// accG
			//sendFloatTelemetryMessage("/telemetry/accG/x", telemetryBuffers[logPlayHead].accG.x);
			//sendFloatTelemetryMessage("/telemetry/accG/y", telemetryBuffers[logPlayHead].accG.y);
			//sendFloatTelemetryMessage("/telemetry/accG/z", telemetryBuffers[logPlayHead].accG.z);
			sendVec3fTelemetryMessage("/telemetry/", telemetryBuffers[logPlayHead].accG.x, telemetryBuffers[logPlayHead].accG.y, telemetryBuffers[logPlayHead].accG.z);
			
			// velocity
			//sendFloatTelemetryMessage("/telemetry/velocity/x", telemetryBuffers[logPlayHead].velocity.x);
			//sendFloatTelemetryMessage("/telemetry/velocity/y", telemetryBuffers[logPlayHead].velocity.y);
			//sendFloatTelemetryMessage("/telemetry/velocity/z", telemetryBuffers[logPlayHead].velocity.z);
			sendVec3fTelemetryMessage("/telemetry/velocity", telemetryBuffers[logPlayHead].velocity.x, telemetryBuffers[logPlayHead].velocity.y, telemetryBuffers[logPlayHead].velocity.z);

			// tireTemp
			//sendFloatTelemetryMessage("/telemetry/tireTemp/fl", telemetryBuffers[logPlayHead].tireTemp.x);
			//sendFloatTelemetryMessage("/telemetry/tireTemp/fr", telemetryBuffers[logPlayHead].tireTemp.y);
			//sendFloatTelemetryMessage("/telemetry/tireTemp/rl", telemetryBuffers[logPlayHead].tireTemp.z);
			//sendFloatTelemetryMessage("/telemetry/tireTemp/rr", telemetryBuffers[logPlayHead].tireTemp.w);
			sendVec4fTelemetryMessage("/telemetry/tireTemp", telemetryBuffers[logPlayHead].tireTemp.x, telemetryBuffers[logPlayHead].tireTemp.y, telemetryBuffers[logPlayHead].tireTemp.z, telemetryBuffers[logPlayHead].tireTemp.w);
			
			// 
			sendIntTelemetryMessage("/telemetry/drsAvailable", telemetryBuffers[logPlayHead].drsAvailable);
			sendIntTelemetryMessage("/telemetry/drsEnabled", telemetryBuffers[logPlayHead].drsEnabled);

			// controller information

			sendFloatTelemetryMessage("/controller/clutch", telemetryBuffers[logPlayHead].clutch);
			sendFloatTelemetryMessage("/controller/brake", telemetryBuffers[logPlayHead].brake);
			sendFloatTelemetryMessage("/controller/throttle", telemetryBuffers[logPlayHead].throttle);
			sendFloatTelemetryMessage("/controller/steerAngle", telemetryBuffers[logPlayHead].steerAngle);
			//telemetryBuffers[logPlayHead].

			logPlayHead++;
			if (logPlayHead == telemetryBuffers.size())	logPlayHead = 0;
			lastPlayedTimer = ofGetElapsedTimeMillis();
		}
	}
	else {
		ofLog(OF_LOG_ERROR, "log play failed");
	}

}

void Telemetry::sendIntTelemetryMessage(string addressEndpoint, int value) {
	ofxOscMessage m;
	m.setAddress(addressEndpoint);
	m.addInt32Arg(value);
	sender.sendMessage(m);
}

void Telemetry::sendFloatTelemetryMessage(string addressEndpoint, float value) {
	ofxOscMessage m;
	m.setAddress(addressEndpoint);
	m.addFloatArg(value);
	sender.sendMessage(m);
}

void ofApp::sendVec3fTelemetryMessage(string addressEndpoint, float x, float y, float z) {
	ofxOscMessage m;
	m.setAddress(addressEndpoint);
	m.addFloatArg(x);
	m.addFloatArg(y);
	m.addFloatArg(z);
	sender.sendMessage(m);
}

void ofApp::sendVec4fTelemetryMessage(string addressEndpoint, float x, float y, float z, float w) {
	ofxOscMessage m;
	m.setAddress(addressEndpoint);
	m.addFloatArg(x);
	m.addFloatArg(y);
	m.addFloatArg(z);
	m.addFloatArg(w);
	sender.sendMessage(m);
}

void Telemetry::sendStringTelemetryMessage(string addressEndpoint, string value) {
	ofxOscMessage m;
	m.setAddress(addressEndpoint);
	m.addStringArg(value);
	sender.sendMessage(m);
}

void Telemetry::sendBoolTelemetryMessage(string addressEndpoint, bool value) {
	ofxOscMessage m;
	m.setAddress(addressEndpoint);
	m.addBoolArg(value);
	sender.sendMessage(m);
}
