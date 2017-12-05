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

	json["telemetry"]["laps"] = completedLaps+1;
	//json["telemetry"]["laps"]["lastLapTimeMillis"] = lastLapTimeMillis;
	//json["telemetry"]["laps"]["bestLapTimeMillis"] = bestLapTimeMillis;

	for (int i = 0; i < telemetryBuffers.size(); i++) {
		json["telemetry"]["datas"][i]["frameNo"] = i + 1;
		json["telemetry"]["datas"][i]["gear"] = telemetryBuffers[i].gear;
		json["telemetry"]["datas"][i]["rpm"] = telemetryBuffers[i].rpm;
		json["telemetry"]["datas"][i]["speedKmh"] = telemetryBuffers[i].speedKmh;
		json["telemetry"]["datas"][i]["drsAvailable"] = telemetryBuffers[i].drsAvailable;
		json["telemetry"]["datas"][i]["drsEnabled"] = telemetryBuffers[i].drsEnabled;
		json["telemetry"]["datas"][i]["normalizedCarPosition"] = telemetryBuffers[i].normalizedCarPosition;
		json["telemetry"]["datas"][i]["iCurrentTime"] = telemetryBuffers[i].iCurrentTime;
		/*
		float heading;	// heading of the car on world coord
		float pitch;	// pitch of the car on world coord
		float roll;		// roll of the car on world coord
		ofVec3f carCoordinates;		// car position on world coord(x,y,z)								
		*/

		json["telemetry"]["datas"][i]["heading"] = telemetryBuffers[i].heading;
		json["telemetry"]["datas"][i]["pitch"] = telemetryBuffers[i].pitch;
		json["telemetry"]["datas"][i]["roll"] = telemetryBuffers[i].roll;

		json["telemetry"]["datas"][i]["carCoordinates"]["x"] = telemetryBuffers[i].carCoordinates.x;
		json["telemetry"]["datas"][i]["carCoordinates"]["y"] = telemetryBuffers[i].carCoordinates.y;
		json["telemetry"]["datas"][i]["carCoordinates"]["z"] = telemetryBuffers[i].carCoordinates.z;


		json["telemetry"]["datas"][i]["accG"]["x"] = telemetryBuffers[i].accG.x;
		json["telemetry"]["datas"][i]["accG"]["y"] = telemetryBuffers[i].accG.y;
		json["telemetry"]["datas"][i]["accG"]["z"] = telemetryBuffers[i].accG.z;
													 
		json["telemetry"]["datas"][i]["velocity"]["x"] = telemetryBuffers[i].velocity.x;
		json["telemetry"]["datas"][i]["velocity"]["y"] = telemetryBuffers[i].velocity.y;
		json["telemetry"]["datas"][i]["velocity"]["z"] = telemetryBuffers[i].velocity.z;
													 
		json["telemetry"]["datas"][i]["tireTemp"]["FL"] = telemetryBuffers[i].tireTemp.x;
		json["telemetry"]["datas"][i]["tireTemp"]["FR"] = telemetryBuffers[i].tireTemp.y;
		json["telemetry"]["datas"][i]["tireTemp"]["RL"] = telemetryBuffers[i].tireTemp.z;
		json["telemetry"]["datas"][i]["tireTemp"]["RR"] = telemetryBuffers[i].tireTemp.w;
													 
		json["telemetry"]["datas"][i]["clutch"] = telemetryBuffers[i].clutch;
		json["telemetry"]["datas"][i]["brake"] = telemetryBuffers[i].brake;
		json["telemetry"]["datas"][i]["throttle"] = telemetryBuffers[i].throttle;
		json["telemetry"]["datas"][i]["steerAngle"] = telemetryBuffers[i].steerAngle;
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
		completedLaps = json["telemetry"]["laps"].asInt() - 1;


		for (unsigned int i = 0; i < json["telemetry"]["datas"].size(); i++) {
			telemetryData tData;
			tData.iCurrentTime = json["telemetry"]["datas"][i]["iCurrentTime"].asInt();
			tData.gear = json["telemetry"]["datas"][i]["gear"].asInt();
			tData.rpm = json["telemetry"]["datas"][i]["rpm"].asInt();
			tData.speedKmh = json["telemetry"]["datas"][i]["speedKmh"].asInt();
			tData.drsAvailable = json["telemetry"]["datas"][i]["drsAvailable"].asInt();
			tData.drsEnabled = json["telemetry"]["datas"][i]["drsEnabled"].asInt();
			tData.normalizedCarPosition = json["telemetry"]["datas"][i]["normalizedCarPosition"].asFloat();

			tData.heading = json["telemetry"]["datas"][i]["heading"].asFloat();
			tData.pitch = json["telemetry"]["datas"][i]["pitch"].asFloat();
			tData.roll = json["telemetry"]["datas"][i]["roll"].asFloat();

			tData.carCoordinates.x = json["telemetry"]["datas"][i]["carCoordinates"]["x"].asFloat();
			tData.carCoordinates.y = json["telemetry"]["datas"][i]["carCoordinates"]["y"].asFloat();
			tData.carCoordinates.z = json["telemetry"]["datas"][i]["carCoordinates"]["z"].asFloat();

			tData.accG.x = json["telemetry"]["datas"][i]["accG"]["x"].asFloat();
			tData.accG.y = json["telemetry"]["datas"][i]["accG"]["y"].asFloat();
			tData.accG.z = json["telemetry"]["datas"][i]["accG"]["z"].asFloat();

			tData.velocity.x = json["telemetry"]["datas"][i]["velocity"]["x"].asFloat();
			tData.velocity.y = json["telemetry"]["datas"][i]["velocity"]["y"].asFloat();
			tData.velocity.z = json["telemetry"]["datas"][i]["velocity"]["z"].asFloat();

			tData.tireTemp.x = json["telemetry"]["datas"][i]["tireTemp"]["FL"].asFloat();
			tData.tireTemp.y = json["telemetry"]["datas"][i]["tireTemp"]["FR"].asFloat();
			tData.tireTemp.z = json["telemetry"]["datas"][i]["tireTemp"]["RL"].asFloat();
			tData.tireTemp.w = json["telemetry"]["datas"][i]["tireTemp"]["RR"].asFloat();

			tData.clutch = json["telemetry"]["datas"][i]["clutch"].asFloat();
			tData.brake = json["telemetry"]["datas"][i]["brake"].asFloat();
			tData.throttle = json["telemetry"]["datas"][i]["throttle"].asFloat();
			tData.steerAngle = json["telemetry"]["datas"][i]["steerAngle"].asFloat();

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

			// lap info
			sendIntTelemetryMessage("/telemetry/completedLaps", tStaticData.completedLaps);
			sendIntTelemetryMessage("/telemetry/lastLapTimeMillis", tStaticData.lastLapTimeMillis);
			sendIntTelemetryMessage("/telemetry/bestLapTimeMillis", tStaticData.bestLapTimeMillis);


			sendIntTelemetryMessage("/telemetry/currentLapTimeMillis", telemetryBuffers[logPlayHead].iCurrentTime);
			sendFloatTelemetryMessage("/telemetry/normalizedCarPosition", telemetryBuffers[logPlayHead].normalizedCarPosition);

			// car status
			sendIntTelemetryMessage("/telemetry/gear", telemetryBuffers[logPlayHead].gear);
			sendIntTelemetryMessage("/telemetry/rpms", telemetryBuffers[logPlayHead].rpm);
			sendFloatTelemetryMessage("/telemetry/speedKmh", telemetryBuffers[logPlayHead].speedKmh);

			sendFloatTelemetryMessage("/telemetry/heading", telemetryBuffers[logPlayHead].heading);
			sendFloatTelemetryMessage("/telemetry/pitch", telemetryBuffers[logPlayHead].pitch);
			sendFloatTelemetryMessage("/telemetry/roll", telemetryBuffers[logPlayHead].roll);

			sendFloatTelemetryMessage("/telemetry/carCoordinates/x", telemetryBuffers[logPlayHead].carCoordinates.x);
			sendFloatTelemetryMessage("/telemetry/carCoordinates/y", telemetryBuffers[logPlayHead].carCoordinates.y);
			sendFloatTelemetryMessage("/telemetry/carCoordinates/z", telemetryBuffers[logPlayHead].carCoordinates.z);

			// accG
			sendFloatTelemetryMessage("/telemetry/accG/x", telemetryBuffers[logPlayHead].accG.x);
			sendFloatTelemetryMessage("/telemetry/accG/y", telemetryBuffers[logPlayHead].accG.y);
			sendFloatTelemetryMessage("/telemetry/accG/z", telemetryBuffers[logPlayHead].accG.z);

			// velocity
			sendFloatTelemetryMessage("/telemetry/velocity/x", telemetryBuffers[logPlayHead].velocity.x);
			sendFloatTelemetryMessage("/telemetry/velocity/y", telemetryBuffers[logPlayHead].velocity.y);
			sendFloatTelemetryMessage("/telemetry/velocity/z", telemetryBuffers[logPlayHead].velocity.z);

			// tireTemp
			sendFloatTelemetryMessage("/telemetry/tireTemp/fl", telemetryBuffers[logPlayHead].tireTemp.x);
			sendFloatTelemetryMessage("/telemetry/tireTemp/fr", telemetryBuffers[logPlayHead].tireTemp.y);
			sendFloatTelemetryMessage("/telemetry/tireTemp/rl", telemetryBuffers[logPlayHead].tireTemp.z);
			sendFloatTelemetryMessage("/telemetry/tireTemp/rr", telemetryBuffers[logPlayHead].tireTemp.w);

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
