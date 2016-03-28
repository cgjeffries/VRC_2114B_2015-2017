#pragma config(I2C_Usage, I2C1, i2cSensors)
#pragma config(Sensor, in1,    flywheelBall,   sensorLineFollower)
#pragma config(Sensor, in2,    puncherBall,    sensorLineFollower)
#pragma config(Sensor, in7,    expanderBatt,   sensorAnalog)
#pragma config(Sensor, dgtl1,  speed,          sensorQuadEncoder)
#pragma config(Sensor, dgtl3,  ball,           sensorTouch)
#pragma config(Sensor, dgtl4,  driveEncoderleft, sensorQuadEncoder)
#pragma config(Sensor, dgtl6,  driveEncoderright, sensorQuadEncoder)
#pragma config(Sensor, dgtl9,  led,            sensorLEDtoVCC)
#pragma config(Sensor, dgtl10, Puncher,        sensorQuadEncoder)
#pragma config(Sensor, dgtl12, PuncherLimit,   sensorTouch)
#pragma config(Sensor, I2C_1,  ,               sensorQuadEncoderOnI2CPort,    , AutoAssign )
#pragma config(Motor,  port1,           leftLauncherTop, tmotorVex393TurboSpeed_HBridge, openLoop)
#pragma config(Motor,  port2,           leftPuncherTop, tmotorVex393_MC29, openLoop, reversed)
#pragma config(Motor,  port3,           rightPuncherTop, tmotorVex393_MC29, openLoop)
#pragma config(Motor,  port4,           rightPuncherBottom, tmotorVex393_MC29, openLoop)
#pragma config(Motor,  port5,           leftDrive,     tmotorVex393HighSpeed_MC29, openLoop)
#pragma config(Motor,  port6,           intakeBottom,  tmotorVex393HighSpeed_MC29, openLoop)
#pragma config(Motor,  port7,           intakeTop,     tmotorVex393_MC29, openLoop)
#pragma config(Motor,  port8,           rightDrive,    tmotorVex393HighSpeed_MC29, openLoop, reversed)
#pragma config(Motor,  port9,           leftLauncherBottom, tmotorVex393TurboSpeed_MC29, openLoop)
#pragma config(Motor,  port10,          leftLauncherCenter, tmotorVex393TurboSpeed_HBridge, openLoop, reversed, encoderPort, I2C_1)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

int RPMAverage2 = 0;
int RPMAverage = 0;
long launcherTemp111 = 0;
long launcherTemp222 = 0;
long launcherRPM = 0;
float Kp = 0.004;
float Kd = 0.05;
float Kp2left = 0.01;
float Kd2left = 0;
float Kp2right = 0.01;
float Kd2right = 0;
int error = 0;
int target = 1600;
int derivative = 0;
int previousError = 0;
int error2left = 0;
int target2left = 0;
int derivative2left = 0;
int previousError2left = 0;
int error2right = 0;
int target2right = 0;
int derivative2right = 0;
int previousError2right = 0;
int rampRPM = target - 100;
float temp1 = 100;
float rampingSpeed = 0;
float PDout = 0;
int time = 0;
bool PDenabled = false;
bool controlEnabled = false;
bool launcherEnabled = false;
bool bangbangEnabled = false;
string mode2 = "disabled";
bool isAuton = false;
bool master = false;
string test;
int ballNumber = 0;
string auton;
string state;
int puncherValue = 0;
int debugStateThingy;
int baseLightValueFlywheel = 3000;
int baseLightValuePuncher = 3000;
bool reverse = true;
bool puncherControl = false;
bool drivePDEnabled = false;
bool timeout = false;
const short leftButton = 1;
const short centerButton = 2;
const short rightButton = 4;
//int stuff = 0;
//Competition Control and Duration Settings
#pragma competitionControl(Competition)
#pragma autonomousDuration(20)
#pragma userControlDuration(120)

#include "Competition_Control.c"	 //Main competition background code...do not modify!

void resetDriveEncoders()
{
	SensorValue[driveEncoderleft] = 0;
	SensorValue[driveEncoderright] = 0;
}

void setTimeout(int timeInSeconds)
{
	time = timeInSeconds * 4;
	timeout = false;
	startTask(time);
}
void setAllDrive(int speed)
{
	motor[leftDrive] = speed;
	motor[rightDrive] = speed;
}

void setLauncher(float speed)
{
	motor[leftLauncherBottom] = (int) speed;
	motor[leftLauncherTop] = (int) speed;
	motor[leftLauncherCenter] = (int) speed;
	//motor[rightLauncherBottom] = (int) speed;
	//motor[rightLauncherTop] = (int) speed;
}

void setPuncher(int speed)
{
	motor[leftPuncherTop] = speed;
	//motor[leftPuncherBottom] = speed;
	motor[rightPuncherTop] = speed;
	motor[rightPuncherBottom] = speed;
	puncherValue = speed;
}

void setIntake(int power)
{
	motor[intakeTop] = power;
	motor[intakeBottom] = power;
}

void rampDown()
{
	rampingSpeed = temp1;
	while(launcherRPM > 500)
	{
		rampingSpeed = (rampingSpeed - 1); //Should take about 5 seconds
		if(rampingSpeed < 10)
		{
			rampingSpeed = 10;
		}
		setLauncher(rampingSpeed);
		delay(50);
	}
	setLauncher(0);
}

void rampUp()
{
	rampingSpeed = 30;
	bool timeout = false;
	while(launcherRPM < rampRPM && !timeout)
	{
		rampingSpeed = (rampingSpeed + 2); //Should take about 5 seconds
		setLauncher(rampingSpeed);
		if(rampingSpeed > 127)
			timeout = true;
		delay(50);
	}
}

void shootPuncherPreloads()
{
	setTimeout(4);
	for(int i = 0; i < 4; i++)
	{
		//SensorValue[PuncherLimit] = 0;
		while(baseLightValuePuncher - SensorValue[puncherBall] > 1500 && !timeout)
		{
			setPuncher(127);
			delay(10);
		}
		delay(20);
		while(baseLightValuePuncher - SensorValue[puncherBall] < 1500 && !timeout)
		{
			setPuncher(0);
			delay(10);
		}
		//delay(1000);
	}
}

void shootFlywheelBalls()
{
	for(int i = 0; i < 4; i++)
	{
		while(baseLightValueFlywheel - SensorValue[flywheelBall] < 1500)
		{
			motor[intakeTop] = 127;
			motor[intakeBottom] = 127;
			delay(10);
		}
		delay(200);
		while(baseLightValueFlywheel - SensorValue[flywheelBall] > 1500)
		{
			motor[intakeTop] = 127;
			motor[intakeBottom] = 127;
			delay(10);
		}
		if(target - RPMAverage2 > 100)
		{
			motor[intakeTop] = -10;
			delay(100);
		}
		while(target - RPMAverage2 > 100)
		{
			motor[intakeBottom] = 0;
			motor[intakeTop] = 0;
			delay(10);
		}
		//delay(750);
	}
}

void waitForRelease(char * button)
{
	int number;
	if(button != "all")
	{
		if(button == "rightButton")
			number = 4;
		else if(button == "leftButton")
			number = 1;
		else if(button == "centerButton")
			number = 2;

		while(nLCDButtons == number)
			delay(5);
	}
	else
	{
		while(nLCDButtons != 0){}
		wait1Msec(5);
	}
}

//void waitForRelease()
//{
//	while(nLCDButtons != 0){}
//	wait1Msec(5);
//}

void calibrateLightSensors()
{
	string lightValueFlywheel;
	string lightValuePuncher;
	while(nLCDButtons != centerButton && bIfiRobotDisabled)
	{
		displayLCDCenteredString(0, "Please Calibrate");
		displayLCDCenteredString(1, "Start");
	}
	waitForRelease("centerButton");
	while(nLCDButtons != centerButton && bIfiRobotDisabled)
	{
		clearLCDLine(0);
		clearLCDLine(1);
		sprintf(lightValueFlywheel, "%1.2i%c", SensorValue[flywheelBall]);
		sprintf(lightValuePuncher, "%1.2i%c", SensorValue[puncherBall]);
		displayLCDString(0, 0, "F: ");
		displayNextLCDString(lightValueFlywheel);
		displayLCDString(0, 9, " P:");
		displayNextLCDString(lightValuePuncher);
		displayLCDCenteredString(1, "Done");
		delay(100);
	}
	baseLightValueFlywheel = SensorValue[flywheelBall];
	baseLightValuePuncher = SensorValue[puncherBall];

	waitForRelease("centerButton");
}

void setAuton (int override)
{
	int autonTemp = 1;
	while(nLCDButtons != centerButton && bIfiRobotDisabled)
	{
		clearLCDLine(0);
		clearLCDLine(1);
		if(nLCDButtons == leftButton)
		{
			if(autonTemp != 1)
			{
				autonTemp --;
			}
			waitForRelease("leftButton");
			//waitForRelease();
		}
		if(nLCDButtons == rightButton)
		{
			if(autonTemp != 6)
			{
				autonTemp ++;
			}
			waitForRelease("rightButton");
		}
		if(autonTemp == 1)
		{
			auton = "full_court_preloads";
		}
		else if(autonTemp == 2)
		{
			auton = "field_preloads";
		}
		else if(autonTemp == 3)
		{
			auton = "field_spin";
		}
		else if(autonTemp == 4)
		{
			auton = "none";
		}
		else if(autonTemp == 5)
		{
			auton = "TestDistance";
		}
		else if(autonTemp == 6)
		{
			auton = "TestRotate";
		}
		displayLCDString(0, 0, auton);
		delay(100);
	}
	waitForRelease("centerButton");
	int blah = 0;
	while(blah == 0 && bIfiRobotDisabled)
	{
		clearLCDLine(0);
		clearLCDLine(1);
		displayLCDString(0, 0, "Intake Side is");
		displayLCDString(1, 0, "Front      Back");

		if(nLCDButtons == leftButton)
		{
			blah = 1;
			reverse = false;
		}
		else if(nLCDButtons == rightButton)
		{
			blah = 1;
			reverse = true;
		}
		delay(100);
	}
	/*
	while(nLCDButtons != centerButton && bIfiRobotDisabled)
	{
		clearLCDLine(0);
		clearLCDLine(1);
		displayLCDString(0, 0, "Preload");
		displayLCDString(1, 0, "OUT   Done    IN");
		if(nLCDButtons == leftButton)
			setIntake(-127);
		else if(nLCDButtons == rightButton)
			setIntake(127);
		else
			setIntake(0);
		delay(100);
	}
	*/
}


task launcherSpeedAverage()
{
	int RPMA1 = 0;
	int RPMA2 = 0;
	int RPMA3 = 0;
	int RPMA4 = 0;
	int RPMA5 = 0;
	int RPMA6 = 0;
	int RPMA7 = 0;
	int RPMA8 = 0;
	int RPMA9 = 0;
	int RPMA10 = 0;
	while(true)
	{
		for(int i = 0; i <= 9; i++)
		{
			RPMA10 = RPMA9;
			RPMA9 = RPMA8;
			RPMA8 = RPMA7;
			RPMA7 = RPMA6;
			RPMA6 = RPMA5;
			RPMA5 = RPMA4;
			RPMA4 = RPMA3;
			RPMA3 = RPMA2;
			RPMA2 = RPMA1;
			RPMA1 = launcherRPM;
			delay(50);
		}

		RPMAverage = ((RPMA1 + RPMA2 + RPMA3 + RPMA4 + RPMA5 + RPMA6 + RPMA7 + RPMA8 + RPMA9 + RPMA10) / 10);
	}
}

task launcherSpeedAverage2()
{
	int RPMA21 = 0;
	int RPMA22 = 0;
	int RPMA23 = 0;

	while(true)
	{
		for(int i = 0; i < 3; i++)
		{
			RPMA23 = RPMA22;
			RPMA22 = RPMA21;
			RPMA21 = launcherRPM;
			delay(50);
		}

		RPMAverage2 = ((RPMA21 + RPMA22 + RPMA23) / 3);
	}
}

task launcherSpeed()
{
	SensorValue[speed] = 0;
	int RPMA21 = 0;
	int RPMA22 = 0;
	int RPMA23 = 0;
	int number = 0;
	while(true)
	{
		launcherTemp111 = getMotorEncoder(leftLauncherCenter);

		delay(50);
		launcherTemp222 = (getMotorEncoder(leftLauncherCenter) - launcherTemp111);

		launcherRPM = (((launcherTemp222 * 1200) / (784/3) * (7)));	//need to take into account gear ratio between launcher wheel
		//and sensor. also, do I need to worry about how long it takes to perform the above calculations?
		//what about how often the quad encoder updates?
		//should I unplug the second port because I dont need to know which way its going?
		RPMA23 = RPMA22;
		RPMA22 = RPMA21;
		RPMA21 = launcherRPM;
		delay(50);
		RPMAverage2 = ((RPMA21 + RPMA22 + RPMA23) / 3);
	}
}

task PD()
{
	error = 0;
	previousError = 0;

	while(true)
	{
		if(PDenabled == true)
		{
			error = target - RPMAverage2;

			derivative = error - previousError;

			previousError = error;

			PDout = (error * Kp) + (derivative * Kd);
		}

		delay(50);
	}
}

task control()
{

	while(true)
	{
		if(controlEnabled == true)
		{
			temp1 += PDout ;

			if(temp1 < 0)
			{
				temp1 = 0;
			}
			setLauncher(temp1);

			delay(100);

			if(temp1 > 127)
			{
				temp1 = 127;
			}
			else if(temp1 < -127)
			{
				temp1 = -127;
			}
		}
	}
}

task bangbang()
{
	while(true)
	{
		if(bangbangEnabled)
		{
			if(RPMAverage2 < target)
			{
				setLauncher(107);
			}
			else if(RPMAverage2 > target)
			{
				setLauncher(50);
			}
		}
		delay(10);
	}
}

task PD2()
{
	while(true)
	{

		error2left = target2left - SensorValue[driveEncoderleft];

		derivative2left = error2left - previousError2left;

		previousError2left = error2left;

		motor[leftDrive] = ((error2left * Kp2left) + (derivative2left * Kd2left));


		error2right = target2right - SensorValue[driveEncoderright];

		derivative2right = error2right - previousError2right;

		previousError2right = error2right;

		motor[leftDrive] = ((error2right * Kp2right) + (derivative2right * Kd2right));

		delay(50);
	}
}

task driveControl()
{
	while(true)
	{
		delay(50);

			if(vexRT[Btn5U] == 1)
			{
				motor[intakeTop] = 127;
				//motor[intakeBottom] = -127;
			}
			else if(vexRT[Btn5D] == 1)
			{
				motor[intakeTop] = -127;
				//motor[intakeBottom] = 127;
			}
			else
			{
				motor[intakeTop] = 0;
				//motor[intakeBottom] = 0;
			}
			if(puncherControl == false)
			{
				if(vexRT[Btn7L] == 1)
				{
					setPuncher(127);
				}
				else if(vexRT[Btn7R] == 1)
				{
					setPuncher(-60);
				}
				else
				{
					setPuncher(0);
				}
			}

		if(vexRT[Btn6U] == 1)
		{
			motor[intakeBottom] = 127;
		}
		else if(vexRT[Btn6D] == 1)
		{
			motor[intakeBottom] = -127;
		}
		else
		{
			motor[intakeBottom] = 0;
		}


		if(reverse == true)
		{
			if(abs(vexRT[Ch3]) > 10)
			{
				motor[leftDrive] = vexRT[Ch3];
				//motor[leftDrive2] = vexRT[Ch3];
	//			motor[leftRearDrive] = vexRT[Ch3];
			}
			else
			{
				motor[leftDrive] = 0;
				//motor[leftDrive2] = 0;
	//			motor[leftRearDrive] = 0;
			}

			if(abs(vexRT[Ch2]) > 10)
			{
				motor[rightDrive] = vexRT[Ch2];
				//motor[rightDrive2] = vexRT[Ch2];
	//			motor[rightRearDrive] = vexRT[Ch2];
			}
			else
			{
				motor[rightDrive] = 0;
				//motor[rightDrive2] = 0;
	//			motor[rightRearDrive] = 0;
			}
		}
		else
		{
			if(abs(vexRT[Ch2]) > 10)
			{
				motor[leftDrive] = -1 * vexRT[Ch2];
				//motor[leftDrive2] = vexRT[Ch3];
	//			motor[leftRearDrive] = vexRT[Ch3];
			}
			else
			{
				motor[leftDrive] = 0;
				//motor[leftDrive2] = 0;
	//			motor[leftRearDrive] = 0;
			}

			if(abs(vexRT[Ch3]) > 10)
			{
				motor[rightDrive] = -1 * vexRT[Ch3];
				//motor[rightDrive2] = vexRT[Ch2];
	//			motor[rightRearDrive] = vexRT[Ch2];
			}
			else
			{
				motor[rightDrive] = 0;
				//motor[rightDrive2] = 0;
	//			motor[rightRearDrive] = 0;
			}
		}
		if(RPMAverage2 >= target - 100)
		{
			SensorValue[led] = 1;
		}
		else
		{
			SensorValue[led] = 0;
		}

		//if(vexRT[Btn8D] == 1)
		//{
		//	motor[leftLift] = 127;
		//	//motor[rightLift] = 127;
		//}
		//else if(vexRT[Btn8U] == 1)
		//{
		//	motor[leftLift] = -127;
		//	//motor[rightLift] = -127;
		//}
		//else
		//{
		//	motor[leftLift] = 0;
		//	//motor[rightLift] = 0;
		//}
	}
}

task distance()
{
	while(true)
	{
		if(vexRT[Btn8U] == 1)
		{
			target = 1600;
		}
		else if(vexRT[Btn8D] == 1)
		{
			target = 1300;
		}
		else if(vexRT[Btn8L] == 1)
		{
			target = 1000;
		}

		state = nVexRCReceiveState;
		delay(50);
		while(vexRT[Btn8R] == 1)
		{
			puncherControl = true;
			SensorValue[PuncherLimit] = 0;
			while(SensorValue[PuncherLimit] == 0 && vexRT[Btn8R] == 1)
			{
				setPuncher(127);
				delay(10);
			}
			delay(20);
			while(SensorValue[PuncherLimit] == 1 && vexRT[Btn8R] == 1)
			{
				setPuncher(127);
				delay(10);
			}
			setPuncher(0);
			delay(750);
		}
		puncherControl = false;
	}
}



task launcherMaster()
{
//PDenabled = false;
//controlEnabled = false;
//	int suff = 9001;
master = true;
while(true)
{

	//mode == "usercontrol"
	if(!isAuton)
	{
		while(vexRT[Btn7U] == 0)
		{
			delay(100);
		}
		time1[T1] = 0;

		rampUp();
		bangbangEnabled = true;

		while(vexRT[Btn7D] == 0)
		{
			delay(100);
		}

		bangbangEnabled = false;
		rampDown();
	}
	else if(isAuton)
	{
		while(launcherEnabled == false && isAuton)
		{
			delay(100);
		}
		time1[T1] = 0;

		rampUp();
		bangbangEnabled = true;

		while(launcherEnabled == true && isAuton)
		{
			delay(100);
		}

		rampDown();
		bangbangEnabled = false;
	}


}

}

task timers()
{
	while(!timeout)
	{
		delay(250);
		time --;
		if(time <= 0)
		{
			timeout = true;
		}

	}
}

/////////////////////////////////////////////////////////////////////////////////////////
//
//													Pre-Autonomous Functions
//
// You may want to perform some actions before the competition starts. Do them in the
// following function.
//
/////////////////////////////////////////////////////////////////////////////////////////
int blahblah = 0;
void pre_auton()														//I might need to move the pre_auton call in the Vex_Competition_includes to line 54 instead
//of line 61.
{
	mode2 = "preAuton";
	bStopTasksBetweenModes = false;

	//motor[launcherAngleAdjust] = -50;					//Can I move motors in preauton? should I move the call to it in Vex_Competition_includes to a
	//waitUntil(SensorValue[angleReset] == 1);	//different spot?
	//motor[launcherAngleAdjust] = 0;
	//SensorValue[launcherAngle] = 0;

	mode2 = "afterPreAuton";
	blahblah = 1;
	calibrateLightSensors();
	setAuton(0);

	blahblah = 2;

	while(bIfiRobotDisabled)
	{
		nTimeXX++;
		clearLCDLine(0);
		clearLCDLine(1);
		displayStatusAndTime();
		delay(100);
	}


}

/////////////////////////////////////////////////////////////////////////////////////////
//
//																 Autonomous Task
//
// This task is used to control your robot during the autonomous phase of a VEX Competition.
// You must modify the code to add your own robot specific commands here.
//
/////////////////////////////////////////////////////////////////////////////////////////

task autonomous()
{
isAuton = true;
startTask(launcherMaster);							//All of this is for the launcher, and for diagnostics
startTask(bangbang);
//PDenabled = false;
//startTask(PD2);
//startTask(PD);
//startTask(control);
startTask(launcherSpeed);
//startTask(launcherSpeedAverage);
//startTask(launcherSpeedAverage2);
//startTask(timers);
mode2 = autonomous;
time = 0;
clearLCDLine(0);
clearLCDLine(1);

delay(100);


test = "AUTON_RUNNING";

if(auton == "full_court_preloads")
{
	shootPuncherPreloads();
}
else if(auton == "field_spin")
{
	startTask(PD2);
	target2left = 2300; 									//Need to give this an actual value
	target2right = 2300;
	delay(500);
	while(error2left > 100 && error2right > 100)
		delay(100);

	setAllDrive(-15);
	delay(200);
	setAllDrive(0);
	delay(200);
	motor[leftDrive] = -127;
	motor[rightDrive] = 127;
}
else if(auton == "field_preloads")
{
	resetDriveEncoders();
	setAllDrive(5);
	shootPuncherPreloads();

	target2left = 3400;
	target2right = 3400;
	drivePDEnabled = true;
	motor[intakeBottom] = 127;
	motor[intakeTop] = 127;
	bool done = false;
	int count = 0;
	setTimeout(6);
	while(!done && !timeout)
	{
		if(error2left < 50 && error2right < 50 && count != 10)
		{
			count ++;
		}
		else if(error2left > 50 || error2right > 50)
		{
			count = 	0;
		}
		else if(count >= 10)
		{
			done = true;
		}
		delay(25);
	}
	drivePDEnabled = false;
	motor[intakeBottom] = 0;
	motor[intakeTop] = 0;
	setAllDrive(0);
	delay(250);

	resetDriveEncoders();
	target2left = -1800;
	target2right = 1800;

	drivePDEnabled = true;
	setTimeout(0.75);
	while(baseLightValueFlywheel - SensorValue[flywheelBall] > 500 && !timeout)		//Making sure there isnt a ball on the flywheel
	{
		motor[intakeTop] = -60;
	}
	motor[intakeTop] = 0;

	launcherEnabled = true;
	setTimeout(1.5);
	count = 0;
	while(!done && !timeout)
	{
		if(error2left > -25 && error2left < 25 && error2right < 25 && error2right > -25 && count != 10)
		{
			count ++;
		}
		else if(error2left < -25 || error2left > 25 || error2right > 25 || error2right < -25)
		{
			count = 0;
		}
		else if(count >= 10)
		{
			done = true;
		}
		delay(25);
	}

	setAllDrive(0);
	delay(250);

	delay(100);

	shootFlywheelBalls();
	launcherEnabled = false;
}
else if(auton == "none")
{
																		//Self explanatory.
	displayLCDCenteredString(0, ":(");
}
else if(auton == "TestDistance")
{
	resetDriveEncoders();
	target2left = 7200;
	target2right = 7200;
	drivePDEnabled = true;
	bool done = false;
	int count = 0;
	while(!done)
	{
		if(error2left < 50 && error2right < 50 && count != 10)
		{
			count ++;
		}
		else if(error2left > 50 || error2right > 50)
		{
			count = 	0;
		}
		else if(count >= 10)
		{
			done = true;
		}
		delay(25);
	}
	drivePDEnabled = false;
	setAllDrive(0);
}
else if(auton == "TestRotate")
{
	resetDriveEncoders();
	target2left = -1800;
	target2right = 1800;
	drivePDEnabled = true;
	bool done = false;
	int count = 0;
	while(!done)
	{
		if(error2left > -25 && error2left < 25 && error2right < 25 && error2right > -25 && count != 10)
		{
			count ++;
		}
		else if(error2left < -25 || error2left > 25 || error2right > 25 || error2right < -25)
		{
			count = 0;
		}
		else if(count >= 10)
		{
			done = true;
		}
		delay(25);
	}

	setAllDrive(0);
}

//delay(14000);

//launcherEnabled = false;

//motor[leftFrontDrive] = motor[leftRearDrive] = motor[rightFrontDrive] = motor[rightRearDrive] = 127;

//delay(3000);

//motor[leftFrontDrive] = motor[leftRearDrive] = -127;
//setLauncher(0);
}

/////////////////////////////////////////////////////////////////////////////////////////
//
//																 User Control Task
//
// This task is used to control your robot during the user control phase of a VEX Competition.
// You must modify the code to add your own robot specific commands here.
//
/////////////////////////////////////////////////////////////////////////////////////////
float battery = 0.0;
task usercontrol()
{
isAuton = false;
mode2 = usercontrol;
startTask(driveControl);								//Start drive/intake code. Should I move the code in that task to the usercontrol task?
//startTask(launcherMaster);
startTask(launcherMaster);							//All of this is for the launcher, and for diagnostics
startTask(bangbang);
PDenabled = false;
//startTask(PD2);
//startTask(PD);
//startTask(control);
startTask(launcherSpeed);
//startTask(launcherSpeedAverage);
//startTask(launcherSpeedAverage2);
//startTask(timers);
startTask(distance);
int count = 0;
int number = 1;
// User control code here, inside the loop
string mainBattery;
string expanderBattery;
string backupBattery;
string LCDlauncherRPM;
string motorSpeed;
while (true)
{

	clearLCDLine(0);
	clearLCDLine(1);
	battery = SensorValue[expanderBatt]/280;
	sprintf(LCDlauncherRPM, "%1.2i%c", RPMAverage2);
	sprintf(mainBattery, "%1.2f%c", nImmediateBatteryLevel/1000.0);
	sprintf(expanderBattery, "%1.2f%c", (float)SensorValue[expanderBatt]/280);
	sprintf(backupBattery, "%1.2f%c", BackupBatteryLevel/1000.0);
	sprintf(motorSpeed, "%1.2f%c", temp1);
	displayLCDString(0, 0, "RPM:");
	displayNextLCDString(LCDlauncherRPM);
	displayNextLCDString("PWR:");
	displayNextLCDString(motorSpeed);
	if(number == 1)
	{
		displayLCDString(1, 0, "Main:");
		displayNextLCDString(mainBattery);
	}
	else if(number == 2)
	{
		displayLCDString(1, 0, "Puncher:");
		displayNextLCDString(expanderBattery);
	}
	else if(number == 3)
	{
		displayLCDString(1, 0, "Backup:");
		displayNextLCDString(backupBattery);
	}
	count ++;
	if(count == 30)
		number = 2;
	else if(count == 60)
		number = 3;
	else if(count == 90)
	{
		number = 1;
		count = 0;
	}
	delay(100);

	debugStateThingy = nVexRCReceiveState;



	// This is the main execution loop for the user control program. Each time through the loop
	// your program should update motor + servo values based on feedback from the joysticks.

	// .....................................................................................
	// Insert user code here. This is where you use the joystick values to update your motors, etc.
	// .....................................................................................

	// Remove this function call once you have "real" code.
}
}
