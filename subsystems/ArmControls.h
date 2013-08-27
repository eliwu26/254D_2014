//Arm PID Variables
float armKp = 0.925;
float armKi = 0.015;
float armKd = 0.1;
int previousError = 0;
float errorSum = 0;
int motPower = 0;

//Lift
void lift(int power)
{
	motor[tparms]=power;
	motor[btrtarm]=power;
	motor[btlftarm]=-power;
}

//Reset the PID Variables
void PIDreset(){
  previousError = 0;
  errorSum = 0;
}

//PID
void armPID(int target)
{
  int error = target - nMotorEncoder[btlftarm];
  int derivative = error-previousError;
  motPower = (error*armKp+errorSum*armKi+derivative*armKd);
  lift(-motPower);
  previousError = error;
  errorSum += error/1024.0;
}

//Task to set arm to trough height
task armGoalPreset(){
	while(true){
		armPID(580);
		wait1Msec(5);
	}
}
//Position hold variables; i is the loop counter, and position is the current position hold target
int position = 0;
bool savedPosition = false;
void armControlOpComp(){
	if(SensorValue[limit]==1){
		nMotorEncoder[btlftarm]=0;
	}
	//Moves the lift full speed up
	if (vexRT[Btn6U]==1)
	{
	  lift(-127);
	  savedPosition = false;
	}

  //Moves the lift full speed down, and deactuates the pistons
	if (vexRT[Btn6D]==1)
	{
		if(nMotorEncoder[btlftarm]<30){
			lift(90);
		}
		else{
		  lift(127);
		}
	  savedPosition = false;
	}

	//If no buttons pertaining to the lift are pressed, hold the arm at current position if not down, otherwise, power down lift
	if (vexRT[Btn6D]==0&&vexRT[Btn6U]==0&&vexRT[Btn8U]==0)
	{
		if(!savedPosition){
			position = nMotorEncoder[btlftarm];
			savedPosition = true;
		}
		if(nMotorEncoder[btlftarm]<=20){
			lift(10);
		}
		else{
			armPID(position);
		}
	}
	//Try to auto-descore height
	if (vexRT[Btn8U]==1&&vexRT[Btn6D]==0&&vexRT[Btn6U]==0)
	{
		savedPosition = true;
		position = 400;
		SensorValue[lftSolenoid]=1;
	  SensorValue[rtSolenoid]=1;
	}
}
