#include "Telemetry.h"



Telemetry::Telemetry() {
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