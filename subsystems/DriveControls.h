//Fdrive is a misnomer... more like driveMotors at power
void fdrive(int power)
{
	motor[frtrtdrive]=-power;
	motor[frtlftdrive]=-power;
	motor[btlftdrive]=power;
	motor[btrtdrive]=-power;
}

//Drive the right motors at power
void Rdrive(int power){
	motor[frtrtdrive]=-power;
	motor[btrtdrive]=-power;
}

//Drive the left motors at power
void Ldrive(int power){
	motor[frtlftdrive]=-power;
	motor[btlftdrive]=power;
}

//return the left encoder distance
float leftdistance()
{
	return (nMotorEncoder[btlftdrive] / (TPR_TORQUE)) * (4.0*PI);
}

//return right encoder distance
float rightdistance()
{
	return (nMotorEncoder[btrtdrive] / (TPR_TORQUE)) * (4.0*PI);
}

//distance variable for distancedrive
float dist;

//drive a certain distance based on encoders
void distancedrive(float inches, int direction, int speed)
{
	nMotorEncoder[btrtdrive]=0;
	while(true)
	{
		dist=abs(rightdistance());
	  if (direction==1){
			if (dist<inches){
				fdrive(speed);
			}
	   	else{
	  		fdrive(0);
	 			break;
	 		}
	 	}
	 	if (direction==0){
			if (dist<inches){
				fdrive(-speed);
			}
	   	else{
	  		fdrive(0);
	 			break;
	 		}
	  }
	}
}

//Drive a certain distance, while PID aligned with the wall
void distancedrivePIDUltra(float inches,float power,float direction){
	nMotorEncoder[btrtdrive]=0;
	float Up = 0.1;
	float Ui = 0.1;
	float Ud = 0.1;
	float Uerror = 0;
	int UpreviousError= 0;
	float Uderivative = 0;
	float Uintegral = 0;
	float change = 0;
	while(true)
	{
		Uerror = leftSonar-rightSonar;
		Uderivative = Uerror-UpreviousError;
		Uintegral += Uerror;
		change = Uerror*Up + Uderivative*Ud + Uintegral*Ui;
		if(change>20){
			change = 20;
		}
		if(change<-20){
			change = -20;
		}
		dist=abs(rightdistance());
	  if (direction==1){
			if (dist<inches){
				Rdrive(power-change);
				Ldrive(power+change);
			}
	   	else{
	  		fdrive(0);
	 			break;
	 		}
	 	}
	 	if (direction==0){
			if (dist<inches){
				Rdrive(-power-change);
				Ldrive(-power+change);
			}
	   	else{
	  		fdrive(0);
	 			break;
	 		}
	  }
	}
}

//Drive a certain distance with the ultrasound sensors
void distanceDriveUltra(int dist, int power){
	while(sonarBackDistance()!=dist){
		if(sonarBackDistance()<dist){
			fdrive(0);
		}
		if(sonarBackDistance()>dist){
			fdrive(power);
		}

	}
}

float alignError = 0;
float alignIntegral = 0;
float alignDerivative = 0;
float alignPreviousError = 0;
// This constant is for the P loop of the aligning against a wall.
float kPAlign = 1.3;
float kIAlign = 0.01;
float kDAlign = 0.1;

void resetUltraPidVars(){
    alignError = 0;
    alignIntegral = 0;
    alignDerivative = 0;
    alignPreviousError = 0;
}

//Align robot to the wall using Ultrasound
void ultraPIDalign(){
    alignError = leftSonar - rightSonar;
    alignDerivative = alignError - alignPreviousError;
    alignIntegral += alignError;
    alignPreviousError = alignError;
    int turnpower = kPAlign * alignError + kIAlign * alignIntegral + kDAlign * alignDerivative;
    if(turnpower<30){
        turnpower=30;
    }
    if(turnpower>120){
        turnpower=120;
    }
    motor[frtlftdrive]=turnpower;
    motor[btlftdrive]=-turnpower;
    motor[frtrtdrive]=-turnpower;
    motor[btrtdrive]=-turnpower;
}

//Task to align with wall
task alignWithWall(){
    resetUltraPidVars();
    while(true){
        ultraPIDalign();
    }
}

float gyroo = 0;
float angle = 0;

//Return the gyro angle
float gyroValue(){
	// Create float angle, remove offset
  angle = (SensorValue[gyro])  / 10.0;

  // normalize into the range 0 - 360
  if( angle < 0 )
      angle += 360;
  // work out change from last time

  return angle;
}

//A rather messy way to turn using the gyro
void gyroTurn(float degrees, int direction){
	if(direction==0){
		while(abs(gyroValue()-degrees)>=5){
			gyroo = gyroValue();
			motor[frtlftdrive]=127;
		  motor[btlftdrive]=-127;
		  motor[frtrtdrive]=-127;
		  motor[btrtdrive]=-127;
		}
	}
	if(direction==1){
		while(abs(gyroValue()-degrees)>=5){
			gyroo = gyroValue();
			motor[frtlftdrive]=-127;
		  motor[btlftdrive]=127;
		  motor[frtrtdrive]=127;
		  motor[btrtdrive]=127;
		}
	}
}

//Super simple drive a distance using the ulrasound.
void ultradist(float distanceUltra){
	while(SensorValue[backLeftSonar]<distanceUltra){
		fdrive(127);
	}
	fdrive(0);
}

//Turn a certain number of degrees based on encoders
void turn(float degrees,int direction)
{
	float inches = (degrees/20)*PI;
	nMotorEncoder[btrtdrive]=0;
	nMotorEncoder[btlftdrive]=0;
	if(direction==1){
		while(true){
		  float leftdist=abs(leftdistance());
		  float rightdist=abs(rightdistance());
		  if (leftdist<inches){
			  motor[frtlftdrive]=127;
			  motor[btlftdrive]=-127;
		  }
		  if(rightdist<inches){
			  motor[frtrtdrive]=-127;
			  motor[btrtdrive]=-127;
		  }
		  else{
			  fdrive(0);
			  break;
		  }
		}
	}
	if(direction==0){
		while(true)
		{
			float leftdist=abs(leftdistance());
			float rightdist=abs(rightdistance());
			if (leftdist<inches){
				motor[frtlftdrive]=-127;
				motor[btlftdrive]=127;
			}
			if(rightdist<inches){
				motor[frtrtdrive]=127;
				motor[btrtdrive]=127;
			}
			else{
				fdrive(0);
				break;
			}
		}
	}
}
float Y1 = 0;
float X2 = 0;
void driveOpControlComp(){
	//The up and down controls on the left joystick controls the forward and back drive.
	//Moving the left joystick left will increase right side drive speed, while lowering left side.
	//Going full power and turning full left should leave just the right side drive with full power.
	if(Y1<vexRT[Ch3]){
		Y1 += COAST;
	}
	if(X2<vexRT[Ch1]){
		X2 += COAST;
	}
	if(Y1>vexRT[Ch3]){
		Y1 -= COAST;
	}
	if(X2>vexRT[Ch1]){
		X2 -= COAST;
	}

	motor[frtrtdrive]=Y1-X2;
	motor[btrtdrive]=Y1-X2;
	motor[btlftdrive]=-Y1+X2;
	motor[frtlftdrive]=Y1+X2;
}
