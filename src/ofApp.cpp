#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
	ofSetFrameRate(60);
	ofSetVerticalSync(true);

	acsPhysicsData  = new SPageFilePhysics();
	acsGraphicsData = new SPageFileGraphic();
	acsStaticData   = new SPageFileStatic();

	acsPhysics.setup("Local\\acpmf_physics", sizeof(acsPhysics), false);
	acsGraphics.setup("Local\\acpmf_graphics", sizeof(acsGraphics), false);
	acsFileStatic.setup("Local\\acpmf_static", sizeof(acsFileStatic), false);

	isAcsConnected = acsPhysics.connect() && acsGraphics.connect() && acsFileStatic.connect();

	sender.setup(HOST, PORT);
	lastOscSentTimer = ofGetElapsedTimeMillis();

	bRecordOn = false;


}

//--------------------------------------------------------------
void ofApp::update(){
	ofSetWindowTitle(ofToString("Telemetry connection : ") + ofToString(isAcsConnected));

	if (!isAcsPhysicsConnected && ofGetFrameNum() % 300 == 0)	isAcsPhysicsConnected = acsPhysics.connect();
	if (!isAcsGraphicsConnected && ofGetFrameNum() % 300 == 0)	isAcsGraphicsConnected = acsGraphics.connect();
	if (!isAcsStaticConnected && ofGetFrameNum() % 300 == 0)	isAcsStaticConnected = acsFileStatic.connect();

	if (isAcsPhysicsConnected)	acsPhysicsData = acsPhysics.getData();
	if (isAcsGraphicsConnected) acsGraphicsData = acsGraphics.getData();
	if (isAcsStaticConnected)	acsStaticData = acsFileStatic.getData();

	if (ofGetElapsedTimeMillis() - lastOscSentTimer > 50) {
		if(!bPlayLogOn)	sendOscMessage();

		if (bRecordOn) {
			
			tData.iCurrentTime = acsGraphicsData->iCurrentTime;
			tData.gear = acsPhysicsData->gear;
			tData.rpm = acsPhysicsData->rpms;
			tData.speedKmh = acsPhysicsData->speedKmh;
			tData.drsAvailable = acsPhysicsData->drsAvailable;
			tData.drsEnabled = acsPhysicsData->drsEnabled;
			tData.normalizedCarPosition = acsGraphicsData->normalizedCarPosition;
			//acsPhysicsData->drsAvailable == 1 ? tData.drsAvailable = true : tData.drsAvailable = false;
			//acsPhysicsData->drsEnabled == 1 ? tData.drsEnabled = true : tData.drsEnabled = false;

			tData.heading = acsPhysicsData->heading;
			tData.pitch = acsPhysicsData->pitch;
			tData.roll = acsPhysicsData->roll;

			ofVec3f carCoordinates;
			ofVec3f accGData;
			ofVec3f velocityData;
			ofVec4f tireTempData;

			carCoordinates.x = acsGraphicsData->carCoordinates[0];
			carCoordinates.y = acsGraphicsData->carCoordinates[2];
			carCoordinates.z = acsGraphicsData->carCoordinates[1];

			accGData.x = acsPhysicsData->accG[0];
			accGData.y = acsPhysicsData->accG[1];
			accGData.z = acsPhysicsData->accG[2];

			velocityData.x = acsPhysicsData->velocity[0];
			velocityData.y = acsPhysicsData->velocity[1];
			velocityData.z = acsPhysicsData->velocity[2];

			tireTempData.x = acsPhysicsData->tyreCoreTemperature[0];
			tireTempData.y = acsPhysicsData->tyreCoreTemperature[1];
			tireTempData.z = acsPhysicsData->tyreCoreTemperature[2];
			tireTempData.w = acsPhysicsData->tyreCoreTemperature[3];

			tData.carCoordinates = carCoordinates;
			tData.accG = accGData;
			tData.velocity = velocityData;
			tData.tireTemp = tireTempData;

			tData.throttle = acsPhysicsData->gas;
			tData.brake = acsPhysicsData->brake;
			tData.clutch = acsPhysicsData->clutch;
			tData.steerAngle = acsPhysicsData->steerAngle;

			telemetry.push2Telemetry(tData);

			carPos.addVertex(acsGraphicsData->carCoordinates[0]/2, acsGraphicsData->carCoordinates[2]/2, acsGraphicsData->carCoordinates[1]/2);
		}

		lastOscSentTimer = ofGetElapsedTimeMillis();
	}

	
	if (bPlayLogOn)	telemetry.playLog();

}

//--------------------------------------------------------------
void ofApp::draw(){
	ofBackground(ofColor::fromHex(0xFFFF00));
	printAcsGraphics(20, 20);
	printAcsPhysics(420, 20);
	printAcsStatic(920, 20);

	if (bRecordOn) {
		ofPushMatrix();
		ofTranslate(ofGetWidth() / 2, ofGetHeight() / 2);
		ofPushStyle();
		ofNoFill();
		ofSetLineWidth(5);
		ofSetHexColor(0xFF0000);
		carPos.draw();
		ofPopStyle();
		ofPopMatrix();
	}
}

void ofApp::printAcsPhysics(float x, float y) {
	if (isAcsPhysicsConnected) {
		stringstream acsPhysicsOutput;
		acsPhysicsOutput << "PHYSICS ====================================================" << endl	<< endl
			<< setw(24) << "packetId : "			<< ofToString(acsPhysicsData->packetId, 0)		<< endl
			<< setw(24) << "gas : "					<< ofToString(acsPhysicsData->gas, 2)			<< endl
			<< setw(24) << "brake : "				<< ofToString(acsPhysicsData->brake, 2)			<< endl
			<< setw(24) << "fuel : "				<< ofToString(acsPhysicsData->fuel, 2)			<< endl
			<< setw(24) << "gear : "				<< ofToString(acsPhysicsData->gear, 0)			<< endl
			<< setw(24) << "rpms : "				<< ofToString(acsPhysicsData->rpms, 0)			<< endl
			<< setw(24) << "steerAngle : "			<< ofToString(acsPhysicsData->steerAngle, 0)	<< endl
			<< setw(24) << "speedKmh : "			<< ofToString(acsPhysicsData->speedKmh, 0)		<< endl
			<< setw(24) << "velocity : "			<< ofToString(acsPhysicsData->velocity[0], 2)	<< ", " 
													<< ofToString(acsPhysicsData->velocity[1], 2)	<< ", " 
													<< ofToString(acsPhysicsData->velocity[2], 2)	<< endl
			<< setw(24) << "accG : "				<< ofToString(acsPhysicsData->accG[0], 2)		<< ", " 
													<< ofToString(acsPhysicsData->accG[1], 2)		<< ", " 
													<< ofToString(acsPhysicsData->accG[2], 2)		<< endl
			<< setw(24) << "wheelSlip : "			<< ofToString(acsPhysicsData->wheelSlip[0], 2)	<< ", " 
													<< ofToString(acsPhysicsData->wheelSlip[1], 2)	<< ", " 
													<< ofToString(acsPhysicsData->wheelSlip[2], 2)	<< ", " 
													<< ofToString(acsPhysicsData->wheelSlip[3], 2)	<< endl
			<< setw(24) << "wheelLoad : "			<< ofToString(acsPhysicsData->wheelLoad[0], 2)	<< ", " 
													<< ofToString(acsPhysicsData->wheelLoad[1], 2)	<< ", " 
													<< ofToString(acsPhysicsData->wheelLoad[2], 2)	<< ", " 
													<< ofToString(acsPhysicsData->wheelLoad[3], 2)	<< endl
			<< setw(24) << "wheelsPressure : "		<< ofToString(acsPhysicsData->wheelsPressure[0], 2)		<< ", " 
													<< ofToString(acsPhysicsData->wheelsPressure[1], 2)		<< ", "
													<< ofToString(acsPhysicsData->wheelsPressure[2], 2)		<< ", "
													<< ofToString(acsPhysicsData->wheelsPressure[3], 2)		<< endl
			<< setw(24) << "wheelAngularSpeed : "	<< ofToString(acsPhysicsData->wheelAngularSpeed[0], 2)	<< ", " 
													<< ofToString(acsPhysicsData->wheelAngularSpeed[1], 2)	<< ", " 
													<< ofToString(acsPhysicsData->wheelAngularSpeed[2], 2)	<< ", "
													<< ofToString(acsPhysicsData->wheelAngularSpeed[3], 2)	<< endl
			<< setw(24) << "tyreWear : "			<< ofToString(acsPhysicsData->tyreWear[0], 2)			<< ", " 
													<< ofToString(acsPhysicsData->tyreWear[1], 2)			<< ", " 
													<< ofToString(acsPhysicsData->tyreWear[2], 2)			<< ", " 
													<< ofToString(acsPhysicsData->tyreWear[3], 2)			<< endl
			<< setw(24) << "tyreDirtyLevel : "		<< ofToString(acsPhysicsData->tyreDirtyLevel[0], 2)		<< ", " 
													<< ofToString(acsPhysicsData->tyreDirtyLevel[1], 2)		<< ", " 
													<< ofToString(acsPhysicsData->tyreDirtyLevel[2], 2)		<< ", " 
													<< ofToString(acsPhysicsData->tyreDirtyLevel[3], 2)		<< endl
			<< setw(24) << "tyreCoreTemperature : " << ofToString(acsPhysicsData->tyreCoreTemperature[0], 2) << ", " 
													<< ofToString(acsPhysicsData->tyreCoreTemperature[1], 2) << ", " 
													<< ofToString(acsPhysicsData->tyreCoreTemperature[2], 2) << ", " 
													<< ofToString(acsPhysicsData->tyreCoreTemperature[3], 2) << endl
			<< setw(24) << "camberRAD : "			<< ofToString(acsPhysicsData->camberRAD[0], 2)			<< ", " 
													<< ofToString(acsPhysicsData->camberRAD[1], 2)			<< ", " 
													<< ofToString(acsPhysicsData->camberRAD[2], 2)			<< ", " 
													<< ofToString(acsPhysicsData->camberRAD[3], 2)			<< endl
			<< setw(24) << "suspensionTravel : "	<< ofToString(acsPhysicsData->suspensionTravel[0], 2)	<< ", " 
													<< ofToString(acsPhysicsData->suspensionTravel[1], 2)	<< ", " 
													<< ofToString(acsPhysicsData->suspensionTravel[2], 2)	<< ", " 
													<< ofToString(acsPhysicsData->suspensionTravel[3], 2)	<< endl
			<< setw(24) << "drs : "					<< acsPhysicsData->drs									<< endl
			<< setw(24) << "tc : "					<< acsPhysicsData->tc									<< endl
			<< setw(24) << "heading : "				<< acsPhysicsData->heading								<< endl
			<< setw(24) << "pitch : "				<< acsPhysicsData->pitch								<< endl
			<< setw(24) << "roll : "				<< acsPhysicsData->roll									<< endl
			<< setw(24) << "cgHeight : "			<< acsPhysicsData->cgHeight								<< endl
			<< setw(24) << "carDamage : "			<< acsPhysicsData->carDamage[0]							<< ", " 
													<< acsPhysicsData->carDamage[1]							<< ", " 
													<< acsPhysicsData->carDamage[2]							<< ", " 
													<< acsPhysicsData->carDamage[3]							<< ", " 
													<< acsPhysicsData->carDamage[4]							<< endl
			<< setw(24) << "numberOfTyresOut : "	<< acsPhysicsData->numberOfTyresOut						<< endl
			<< setw(24) << "pitLimiterOn : "		<< acsPhysicsData->pitLimiterOn							<< endl
			<< setw(24) << "abs : "					<< acsPhysicsData->abs									<< endl
			<< setw(24) << "kersCharge : "			<< acsPhysicsData->kersCharge							<< endl
			<< setw(24) << "kersInput : "			<< acsPhysicsData->kersInput							<< endl
			<< setw(24) << "autoShifterOn : "		<< acsPhysicsData->autoShifterOn						<< endl
			<< setw(24) << "rideHeight : "			<< acsPhysicsData->rideHeight[0]						<< ", " 
													<< acsPhysicsData->rideHeight[1]						<< endl
			<< setw(24) << "turboBoost : "			<< acsPhysicsData->turboBoost							<< endl
			<< setw(24) << "ballast : "				<< acsPhysicsData->ballast								<< endl
			<< setw(24) << "airDensity : "			<< acsPhysicsData->airDensity							<< endl
			<< setw(24) << "airTemp : "				<< acsPhysicsData->airTemp								<< endl
			<< setw(24) << "roadTemp : "			<< acsPhysicsData->roadTemp								<< endl
			<< setw(24) << "localAngularVel : "		<< ofToString(acsPhysicsData->localAngularVel[0], 2)	<< ", " 
													<< ofToString(acsPhysicsData->localAngularVel[1], 2)	<< ", " 
													<< ofToString(acsPhysicsData->localAngularVel[2], 2)	<< endl
			<< setw(24) << "finalFF : "				<< acsPhysicsData->finalFF								<< endl
			<< setw(24) << "performanceMeter : "	<< acsPhysicsData->performanceMeter						<< endl
			<< setw(24) << "engineBrake : "			<< acsPhysicsData->engineBrake							<< endl
			<< setw(24) << "ersRecoveryLevel : "	<< acsPhysicsData->ersRecoveryLevel						<< endl
			<< setw(24) << "ersPowerLevel : "		<< acsPhysicsData->ersPowerLevel						<< endl
			<< setw(24) << "ersHeatCharging : "		<< acsPhysicsData->ersHeatCharging						<< endl
			<< setw(24) << "ersIsCharging : "		<< acsPhysicsData->ersIsCharging						<< endl
			<< setw(24) << "kersCurrentKJ : "		<< acsPhysicsData->kersCurrentKJ						<< endl
			<< setw(24) << "drsAvailable : "		<< acsPhysicsData->drsAvailable							<< endl
			<< setw(24) << "drsEnabled : "			<< acsPhysicsData->drsEnabled							<< endl;
		
		ofDrawBitmapStringHighlight(acsPhysicsOutput.str(), x, y);
	}

	
}

void ofApp::printAcsGraphics(float x, float y) {
	if (isAcsGraphicsConnected) {

		// graphics
		stringstream acsGraphicsOutput;
		acsGraphicsOutput << "GRAPHICS INFO =================================" << endl
			<< setw(24) << "packetID : "				<< acsGraphicsData->packetId << endl
			<< setw(24) << "STATUS : "					<< acsGraphicsData->status << endl
			<< setw(24) << "session : "					<< acsGraphicsData->session << endl
			<< setw(24) << "completed laps : "			<< acsGraphicsData->completedLaps << endl
			<< setw(24) << "position : "				<< acsGraphicsData->position << endl
			<< setw(24) << "current time s : "			<< wstring2string(acsGraphicsData->currentTime) << endl
			<< setw(24) << "lastTime s : "				<< wstring2string(acsGraphicsData->lastTime) << endl
			<< setw(24) << "bestTime s : "				<< wstring2string(acsGraphicsData->bestTime) << endl
			<< setw(24) << "split : "					<< wstring2string(acsGraphicsData->split) << endl
			<< setw(24) << "current time : "			<< acsGraphicsData->iCurrentTime << endl
			<< setw(24) << "current time [i] : "		<< acsGraphicsData->iCurrentTime << endl

			<< setw(24) << "last time : "				<< acsGraphicsData->iLastTime << endl
			<< setw(24) << "best time : "				<< acsGraphicsData->iBestTime << endl
			<< setw(24) << "sessionTimeLeft : "			<< acsGraphicsData->sessionTimeLeft << endl
			<< setw(24) << "distanceTraveled : "		<< acsGraphicsData->distanceTraveled << endl
			<< setw(24) << "isInPit : "					<< acsGraphicsData->isInPit << endl
			<< setw(24) << "currentSectorIndex : "		<< acsGraphicsData->currentSectorIndex << endl
			<< setw(24) << "lastSectorTime : "			<< acsGraphicsData->lastSectorTime << endl
			<< setw(24) << "numberOfLaps : "			<< acsGraphicsData->numberOfLaps << endl
			<< setw(24) << "TYRE COMPOUND : "			<< wstring2string(acsGraphicsData->tyreCompound) << endl
			<< setw(24) << "replayMult : "				<< acsGraphicsData->replayTimeMultiplier << endl
			<< setw(24) << "normalizedCarPosition : "	<< acsGraphicsData->normalizedCarPosition << endl
			<< setw(24) << "carCoordinates : "			<< ofToString(acsGraphicsData->carCoordinates[0], 2) << ", " 
														<< ofToString(acsGraphicsData->carCoordinates[1], 2) << ", " 
														<< ofToString(acsGraphicsData->carCoordinates[2], 2) << endl;
		ofDrawBitmapStringHighlight(acsGraphicsOutput.str(), x, y);
	}
	
}

void ofApp::printAcsStatic(float x, float y) {
	if (isAcsStaticConnected) {
		//acsStaticData->strCarModel.assign((acsStaticData->carModel).begin(), (acsStaticData->carModel*).end());
		// file static 
		stringstream acsFileStaticOutput;
		acsFileStaticOutput << "STATIC INFO =============================" << endl
			<< setw(28) << "smVersion : "					<< wstring2string(acsStaticData->smVersion) << endl
			<< setw(28) << "acVersion : "					<< wstring2string(acsStaticData->acVersion) << endl
			<< setw(28) << "numberOfSessions : "			<< acsStaticData->numberOfSessions << endl
			<< setw(28) << "numCars : "						<< acsStaticData->numCars << endl
			<< setw(28) << "carModel : "					<< wstring2string(acsStaticData->carModel) << endl
			<< setw(28) << "track : "						<< wstring2string(acsStaticData->track) << endl
			<< setw(28) << "playerName : "					<< wstring2string(acsStaticData->playerName) << endl
			<< setw(28) << "playerSurname : "				<< wstring2string(acsStaticData->playerSurname) << endl
			<< setw(28) << "playerNick : "					<< wstring2string(acsStaticData->playerNick) << endl
			<< setw(28) << "sectorCount : "					<< acsStaticData->sectorCount << endl
			<< setw(28) << "maxTorque : "					<< acsStaticData->maxTorque << endl
			<< setw(28) << "maxPower : "					<< acsStaticData->maxPower << endl
			<< setw(28) << "maxRpm : "						<< acsStaticData->maxRpm << endl
			<< setw(28) << "maxFuel : "						<< acsStaticData->maxFuel << endl
			<< setw(28) << "suspensionMaxTravel : "			<< acsStaticData->suspensionMaxTravel[0] << ", " 
															<< acsStaticData->suspensionMaxTravel[1] << ", " 
															<< acsStaticData->suspensionMaxTravel[2] << ", " 
															<< acsStaticData->suspensionMaxTravel[3] << endl
			<< setw(28) << "tyreRadius : "					<< acsStaticData->tyreRadius[0] << ", " 
															<< acsStaticData->tyreRadius[1] << ", " 
															<< acsStaticData->tyreRadius[2] << ", " 
															<< acsStaticData->tyreRadius[3] << endl
			<< setw(28) << "maxTurboBoost : "				<< acsStaticData->maxTurboBoost << endl
			//<< setw(28) << "deprecated_1 : " 				<< acsStaticData->deprecated_1 << endl
			//<< setw(28) << "deprecated_2 : " 				<< acsStaticData->deprecated_2 << endl
			<< setw(28) << "penaltiesEnabled : "			<< acsStaticData->penaltiesEnabled << endl
			<< setw(28) << "aidFuelRate : "					<< acsStaticData->aidFuelRate << endl
			<< setw(28) << "aidTireRate : "					<< acsStaticData->aidTireRate << endl
			<< setw(28) << "aidMechanicalDamage : "			<< acsStaticData->aidMechanicalDamage << endl
			<< setw(28) << "aidAllowTyreBlankets : "		<< acsStaticData->aidAllowTyreBlankets << endl
			<< setw(28) << "aidStability : "				<< acsStaticData->aidStability << endl
			<< setw(28) << "aidAutoClutch : "				<< acsStaticData->aidAutoClutch << endl
			<< setw(28) << "aidAutoBlip : "					<< acsStaticData->aidAutoBlip << endl
			<< setw(28) << "hasDRS : "						<< acsStaticData->hasDRS << endl
			<< setw(28) << "hasERS : "						<< acsStaticData->hasERS << endl
			<< setw(28) << "hasKERS : "						<< acsStaticData->hasKERS << endl
			<< setw(28) << "kersMaxJ : "					<< acsStaticData->kersMaxJ << endl
			<< setw(28) << "engineBrakeSettingsCount : "	<< acsStaticData->engineBrakeSettingsCount << endl
			<< setw(28) << "ersPowerControllerCount : "		<< acsStaticData->ersPowerControllerCount << endl
			<< setw(28) << "trackSPlineLength : "			<< acsStaticData->trackSPlineLength << endl
			<< setw(28) << "trackConfiguration : "			<< wstring2string(acsStaticData->trackConfiguration) << endl
			<< setw(28) << "ersMaxJ : "						<< acsStaticData->ersMaxJ << endl
			<< setw(28) << "isTimedRace : "					<< acsStaticData->isTimedRace << endl
			<< setw(28) << "hasExtraLap : "					<< acsStaticData->hasExtraLap << endl
			<< setw(28) << "carSkin : "						<< wstring2string(acsStaticData->carSkin) << endl
			<< setw(28) << "reversedGridPosition : "		<< acsStaticData->reversedGridPositions << endl
			<< setw(28) << "PitWindowStart : "				<< acsStaticData->PitWindowStart << endl
			<< setw(28) << "PitWindowEnd : "				<< acsStaticData->PitWindowEnd << endl;

		ofDrawBitmapStringHighlight(acsFileStaticOutput.str(), x, y);
	}
}

void ofApp::sendOscMessage(){
	// car, track, player
	sendStringTelemetryMessage("/playerInfo/carName",	wstring2string(acsStaticData->carModel));
	sendStringTelemetryMessage("/playerInfo/trackName",	wstring2string(acsStaticData->track));
	sendStringTelemetryMessage("/playerInfo/playerName", wstring2string(acsStaticData->playerName));
	
	// car info
	sendFloatTelemetryMessage("/carInfo/maxTorque", acsStaticData->maxTorque);
	sendFloatTelemetryMessage("/carInfo/maxRPM", acsStaticData->maxRpm);
	sendIntTelemetryMessage("/carInfo/hasDRS", acsStaticData->hasDRS);
	sendIntTelemetryMessage("/carInfo/hasERS", acsStaticData->hasERS);

	// lap info
	sendIntTelemetryMessage("/telemetry/completedLaps", acsGraphicsData->completedLaps);
	sendStringTelemetryMessage("/telemetry/currentLapTime", wstring2string(acsGraphicsData->currentTime));
	sendStringTelemetryMessage("/telemetry/lastLapTime", wstring2string(acsGraphicsData->lastTime));
	sendStringTelemetryMessage("/telemetry/bestLapTime", wstring2string(acsGraphicsData->bestTime));
	sendIntTelemetryMessage("/telemetry/currentLapTimeMillis", acsGraphicsData->iCurrentTime);
	sendIntTelemetryMessage("/telemetry/lastLapTimeMillis", acsGraphicsData->iLastTime);
	sendIntTelemetryMessage("/telemetry/bestLapTimeMillis", acsGraphicsData->iBestTime);
	sendFloatTelemetryMessage("/telemetry/normalizedCarPosition", acsGraphicsData->normalizedCarPosition);

	// car status
	sendIntTelemetryMessage("/telemetry/gear", acsPhysicsData->gear);
	sendIntTelemetryMessage("/telemetry/rpms", acsPhysicsData->rpms);
	sendFloatTelemetryMessage("/telemetry/speedKmh", acsPhysicsData->speedKmh);

	sendFloatTelemetryMessage("/telemetry/heading", acsPhysicsData->heading);
	sendFloatTelemetryMessage("/telemetry/pitch", acsPhysicsData->pitch);
	sendFloatTelemetryMessage("/telemetry/roll", acsPhysicsData->roll);

	sendFloatTelemetryMessage("/telemetry/carCoordinates/x", acsGraphicsData->carCoordinates[0]);
	sendFloatTelemetryMessage("/telemetry/carCoordinates/y", acsGraphicsData->carCoordinates[2]);
	sendFloatTelemetryMessage("/telemetry/carCoordinates/z", acsGraphicsData->carCoordinates[1]);
	
	// accG
	sendFloatTelemetryMessage("/telemetry/accG/x", acsPhysicsData->accG[0]);
	sendFloatTelemetryMessage("/telemetry/accG/y", acsPhysicsData->accG[1]);
	sendFloatTelemetryMessage("/telemetry/accG/z", acsPhysicsData->accG[2]);

	// velocity
	sendFloatTelemetryMessage("/telemetry/velocity/x", acsPhysicsData->velocity[0]);
	sendFloatTelemetryMessage("/telemetry/velocity/y", acsPhysicsData->velocity[1]);
	sendFloatTelemetryMessage("/telemetry/velocity/z", acsPhysicsData->velocity[2]);

	// tireTemp
	sendFloatTelemetryMessage("/telemetry/tireTemp/fl", acsPhysicsData->tyreCoreTemperature[0]);
	sendFloatTelemetryMessage("/telemetry/tireTemp/fr", acsPhysicsData->tyreCoreTemperature[1]);
	sendFloatTelemetryMessage("/telemetry/tireTemp/rl", acsPhysicsData->tyreCoreTemperature[2]);
	sendFloatTelemetryMessage("/telemetry/tireTemp/rr", acsPhysicsData->tyreCoreTemperature[3]);

	// 
	sendIntTelemetryMessage("/telemetry/drsAvailable", acsPhysicsData->drsAvailable);
	sendIntTelemetryMessage("/telemetry/drsEnabled", acsPhysicsData->drsEnabled);

	// controller information

	sendFloatTelemetryMessage("/controller/clutch", acsPhysicsData->clutch);
	sendFloatTelemetryMessage("/controller/brake", acsPhysicsData->brake);
	sendFloatTelemetryMessage("/controller/throttle", acsPhysicsData->gas);
	sendFloatTelemetryMessage("/controller/steerAngle", acsPhysicsData->steerAngle);


}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){ 
	switch (key) {
		case 'l':
		case 'L':
			telemetry.importJson("log");
			break;

		case 'p':
		case 'P':
			bPlayLogOn = !bPlayLogOn;
			break;
		case 'r':
		case 'R':
			bRecordOn = true;
			break;
		case 'F':
		case 'f':
			if (bRecordOn) {
				telemetry.export2Json();
				carPos.clear();
				bRecordOn = false;
			}
			break;
	}
}
//--------------------------------------------------------------
void ofApp::keyReleased(int key){ }
//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y){ }
//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){ }
//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){ }
//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){ }
//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){ }
//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){ }
//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){ }
//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){ }
//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ }


string ofApp::wstring2string(wstring wstr) {
	std::string str(wstr.length(), ' ');
	copy(wstr.begin(), wstr.end(), str.begin());
	return str;
}

wstring ofApp::string2wstring(string str) {
	std::wstring wstr(str.length(), L' ');
	copy(str.begin(), str.end(), wstr.begin());
	return wstr;
}

void ofApp::sendIntTelemetryMessage(string addressEndpoint, int value) {
	ofxOscMessage m;
	m.setAddress(addressEndpoint);
	m.addInt32Arg(value);
	sender.sendMessage(m);
}

void ofApp::sendFloatTelemetryMessage(string addressEndpoint, float value) {
	ofxOscMessage m;
	m.setAddress(addressEndpoint);
	m.addFloatArg(value);
	sender.sendMessage(m);
}

void ofApp::sendStringTelemetryMessage(string addressEndpoint, string value) {
	ofxOscMessage m;
	m.setAddress(addressEndpoint);
	m.addStringArg(value);
	sender.sendMessage(m);
}

void ofApp::sendBoolTelemetryMessage(string addressEndpoint, bool value) {
	ofxOscMessage m;
	m.setAddress(addressEndpoint);
	m.addBoolArg(value);
	sender.sendMessage(m);
}

void ofApp::updateStaticData(){
//	telemetry.tStaticData.playerName = acsStaticData->playerName;
//	telemetry.tStaticData.carINfo = acsStaticData->
}
