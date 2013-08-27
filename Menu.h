int count = 0;
//Wait for button release on the LCD.
void waitForRelease(){
	while(nLCDButtons != 0){}
}
//Top menu for LCD
int top_menu(){
	bool picked2 = false;
	int menu = -1;
	clearLCDLine(0);
	clearLCDLine(1);
	while(!picked2){
  	displayLCDCenteredString(0,"<Autonomous>");
  	displayLCDString(1,0,"<Sensor | Power>");
  	if(nLCDButtons == 1) {
      waitForRelease();
      menu=0;
      picked2 = true;
    }
    if(nLCDButtons == 4) {
      waitForRelease();
      menu = 2;
      picked2 = true;
    }
    if(nLCDButtons == 2) {
      waitForRelease();
      menu = 1;
      picked2 = true;
    }
  }
  return menu;
}

//Autonomous Menu
int auton_menu(){
	bool picked = false;
	bool canceled = false;
	clearLCDLine(0);
	clearLCDLine(1);
	while(!picked) {
    displayLCDCenteredString(1,"<   Enter    >");
    if(nLCDButtons == 1) {
      waitForRelease();
      count --;
      if(count < 0) {
        count = MAX_AUTONS;
      }
    }
    if(nLCDButtons == 4) {
      waitForRelease();
      count ++;
      if(count > MAX_AUTONS) {
              count = 0;
      }
    }
    if(nLCDButtons == 2) {
      waitForRelease();
      picked = true;
    }

    switch(count) {
      case 0: {
        displayLCDCenteredString(0,"Left 2-5");
        break;
      }
      case 1: {
        displayLCDCenteredString(0,"Left 2-5-1");
        break;
      }
      case 2: {
        displayLCDCenteredString(0,"Right 2-5");
        break;
      }
      case 3: {
        displayLCDCenteredString(0,"Right 2-5-1");
        break;
      }
      case 4: {
        displayLCDCenteredString(0,"derp");
        break;
      }
    }
  }
  //clearLCDLine(1);

  while(!canceled){
  	displayLCDCenteredString(1,"   < Cancel >   ");
    if(nLCDButtons == 2) {
      waitForRelease();
      return -1;
    }
  }

  return 0;
}

//More sensor values
int moar_sensor_menu(){
	bool picked2 = false;
	string rEncoder = "";
	string lEncoder = "";
	int menu = -1;
	clearLCDLine(0);
	clearLCDLine(1);
	while(!picked2){
  	displayLCDCenteredString(1,"<Moar | Back>");
  	displayLCDString(0,0,"LE:");
  	sprintf(rEncoder, "%d", nMotorEncoder[btrtdrive]); //Build the value to be displayed
  	sprintf(lEncoder, "%d", nMotorEncoder[btlftdrive]); //Build the value to be displayed
  	displayNextLCDString(lEncoder);
  	displayLCDString(0,8,"RE:");
  	displayNextLCDString(rEncoder);
    if(nLCDButtons == 4) {
      waitForRelease();
      menu = -1;
      picked2 = true;
    }
    if(nLCDButtons == 1) {
      waitForRelease();
      menu = 0;
      picked2 = true;
    }
  }
  return menu;
}

//Sensor values
int sensor_menu(){
	bool picked2 = false;
	string armHeight = "";
	string sonarBack = "";
	string sonarAng = "";
	int menu = -1;
	clearLCDLine(0);
	clearLCDLine(1);
	StartTask(updateSonarValues);
	while(!picked2){
		if(SensorValue[limit]==1){
			nMotorEncoder[btlftarm]=0;
		}
  	displayLCDCenteredString(1,"<Moar | Back>");
  	displayLCDString(0,0,"Arm:");
  	sprintf(armHeight, "%d", nMotorEncoder[btlftarm]); //Build the value to be displayed
  	displayNextLCDString(armHeight);
  	sprintf(sonarBack, "%4.2f%c", sonarBackDistance()); //Build the value to be displayed
  	displayNextLCDString(" SR:");
  	displayNextLCDString(sonarBack);
  	sprintf(sonarAng, "%4.2f%c", sonarAngle()); //Build the value to be displayed
    if(nLCDButtons == 4) {
      waitForRelease();
      menu = -1;
      picked2 = true;
    }
    if(nLCDButtons == 1) {
      waitForRelease();
      menu = 3;
      picked2 = true;
    }
  }
  return menu;
}

//Power values
int power_menu(){
	bool picked2 = false;
	string mainBattery = "";
	string secondary = "";
	int menu = -1;
	clearLCDLine(0);
	clearLCDLine(1);
	while(!picked2){
		displayLCDString(0, 0, "P:");
		sprintf(mainBattery, "%1.2f%c", nImmediateBatteryLevel/1000.0,'V'); //Build the value to be displayed
		displayNextLCDString(mainBattery);
		sprintf(secondary, "%1.2f%c", (float)SensorValue[pExpander]/70.0,'V');
		displayLCDString(0,9,"S:");
		displayNextLCDString(secondary);
		displayLCDCenteredString(1,"<Back>");
		if(nLCDButtons == 2) {
      waitForRelease();
      menu = -1;
      picked2 = true;
    }
  }
  return menu;
}

//Menu task
task Menu(){
	int whichMenu = -1;
	while(true){
		if(whichMenu == -1){
			whichMenu = top_menu();
		}
		if(whichMenu == 0){
			whichMenu = sensor_menu();
		}
		if(whichMenu == 1){
			whichMenu = auton_menu();
		}
		if(whichMenu == 2){
			whichMenu = power_menu();
		}
		if(whichMenu == 3){
			whichMenu = moar_sensor_menu();
		}
		wait1Msec(5);
	}
}
