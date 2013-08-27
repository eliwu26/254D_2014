//Intake or output, with pneumatics
void intake(int power)
{
	motor[intk]=power;
	motor[intk2]=power;
	if(power == 127){
		SensorValue[lftSolenoid]=0;
		SensorValue[rtSolenoid]=0;
	}
	if(power == -127){
		if(nMotorEncoder[btlftarm]>30){
			SensorValue[lftSolenoid]=1;
			SensorValue[rtSolenoid]=1;
		}
	}
}

//A little script for outputting correctly
void output(){
	intake(-127);
	wait1Msec(600);
	intake(127);
	wait1Msec(5);
	intake(0);
}

void intakeControlOpComp(){
	//The left shoulder buttons will turn the intake.+++++
	//Output will also actuate pistons to dump the load.
	if (vexRT[Btn5U]==1)
	{
	  intake(127);
	}
	if (vexRT[Btn5U]==0&&vexRT[Btn5D]==0){
	  intake(0);
	}
	if (vexRT[Btn5D]==1)
	{
	  intake(-127);
	}

	//Separate buttons to control the pistons, in case we need to actuate them
	if (vexRT[Btn7U]==1)
	{
	  SensorValue[lftSolenoid]=0;
	  SensorValue[rtSolenoid]=0;
	}
	if (vexRT[Btn7D]==1)
	{
	  SensorValue[lftSolenoid]=1;
	  SensorValue[rtSolenoid]=1;
	}
}