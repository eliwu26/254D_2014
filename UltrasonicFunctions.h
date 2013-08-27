//Sonar variables
float leftSonar = 0;
float rightSonar = 0;
int iLeft = 0;
int iRight = 0;
float bufferLeft[5] = {0,0,0,0,0};
float bufferRight[5] = {0,0,0,0,0};

//Part of the Sonar group - gets the Median of 5 values.
float getMedian(float val1,float val2,float val3,float val4,float val5){
	float tmp ;

	// get min
	if (val1>=val2) {
		tmp = val2; val2 = val1; val1 = tmp;
	}
	if (val1>=val3) {
		tmp = val3; val3 = val1; val1 = tmp;
	}
	if (val1>=val4) {
		tmp = val4; val4 = val1; val1 = tmp;
	}
	if (val1>=val5) {
		tmp = val5; val5 = val1; val1 = tmp;
	}

	// get second min
	if (val2>=val3) {
		tmp = val3; val3 = val2; val2 = tmp;
	}
	if (val2>=val4) {
		tmp = val4; val4 = val2; val2 = tmp;
	}
	if (val2>=val5) {
		tmp = val5; val5 = val2; val2 = tmp;
	}

	// get median
	if (val3>=val4) {
		val3=val4;
	}
	if (val3>=val5) {
		val3=val5;
	}

	return val3;

}

//Filters values from the Left sonar.
float filterValsLeft(float value){
	if(iLeft<5){
		bufferLeft[iLeft]=value;
		iLeft++;
	}
	else if (iLeft>=5){
		bufferLeft[0]=bufferLeft[1];
		bufferLeft[1]=bufferLeft[2];
		bufferLeft[2]=bufferLeft[3];
		bufferLeft[3]=bufferLeft[4];
		bufferLeft[4]=value;
	}
	if(iLeft>=5){
		return getMedian(bufferLeft[0],bufferLeft[1],bufferLeft[2],bufferLeft[3],bufferLeft[4]);
	}
	else return -1;
}

//Filters values from the Right sonar.
float filterValsRight(float value){
	if(iRight<5){
		bufferRight[iRight]=value;
		iRight++;
	}
	else if (iRight>=5){
		bufferRight[0]=bufferRight[1];
		bufferRight[1]=bufferRight[2];
		bufferRight[2]=bufferRight[3];
		bufferRight[3]=bufferRight[4];
		bufferRight[4]=value;
	}
	if(iRight>=5){
		return getMedian(bufferRight[0],bufferRight[1],bufferRight[2],bufferRight[3],bufferRight[4]);
	}
	else return -1;
}
//Calculates robot rotation up to 15 degrees from two different sonar sensors on the back.
float sonarAngle()
{
	float height = abs(leftSonar) - (rightSonar);
	float calcAngle = ((atan(height/SONAR_LENGTH))/(2*PI))*360;
	if(calcAngle < 15.0){
		return calcAngle;
	}
	else return NULL;
}

//Calculates the real distance from the robot to the wall
float sonarBackDistance(){
	if(leftSonar&&rightSonar>1&&sonarAngle()!=NULL){
		return abs((((leftSonar)+(rightSonar))/2)*sin(sonarAngle()));
	}
	else return -1;
}
//Updates the sonar values.
task updateSonarValues(){
	while(true){
		rightSonar = filterValsRight(SensorValue[backRightSonar]);
		leftSonar = filterValsLeft(SensorValue[backLeftSonar]);
		writeDebugStreamLine("%f",sonarAngle());
		wait1Msec(5);
	}
}
