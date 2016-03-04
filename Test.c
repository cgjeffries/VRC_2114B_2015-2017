
task main()
{
	while(true)
	{
		if(vexRT[Btn7D] == 1)
		{
			motor[port1] = 60;
		}
		else
		{
			motor[port1] = 0;
		}

		if(vexRT[Btn7L] == 1)
		{
			motor[port2] = 60;
		}
		else
		{
			motor[port2] = 0;
		}
		if(vexRT[Btn7U] == 1)
		{
			motor[port3] = 60;
		}
		else
		{
			motor[port3] = 0;
		}
		if(vexRT[Btn7R] == 1)
		{
			motor[port4] = 60;
		}
		else
		{
			motor[port4] = 0;
		}
		if(vexRT[Btn8D] == 1)
		{
			motor[port5] = 60;
		}
		else
		{
			motor[port5] = 0;
		}
		if(vexRT[Btn8L] == 1)
		{
			motor[port6] = 60;
		}
		else
		{
			motor[port6] = 0;
		}
		if(vexRT[Btn8U] == 1)
		{
			motor[port7] = 60;
		}
		else
		{
			motor[port7] = 0;
		}
		if(vexRT[Btn8R] == 1)
		{
			motor[port8] = 60;
		}
		else
		{
			motor[port8] = 0;
		}
		if(vexRT[Btn6U] == 1)
		{
			motor[port9] = 60;
		}
		else
		{
			motor[port9] = 0;
		}
		if(vexRT[Btn6D] == 1)
		{
			motor[port10] = 60;
		}
		else
		{
			motor[port10] = 0;
		}


		delay(10);

	}



}
