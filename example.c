#include <pca9685.h>

#include <wiringPi.h>

#include <stdio.h>
#include <stdlib.h>

#include <ctype.h>
#include <unistd.h>
#include <string.h>
#include <getopt.h>
#include <time.h>
#include "i2c_bno055.c"
#include "example.h"
//#include "getbno055.h"



/**
 * Calculate the number of ticks the signal should be high for the required amount of time
 */
int calcTicks(float impulseMs, int hertz)
{
	float cycleMs = 1000.0f / hertz;
	return (int)(MAX_PWM * impulseMs / cycleMs + 0.5f);
}

/**
 * input is [0..1]
 * output is [min..max]
 */
float map(float input, float min, float max)
{
	return (input * max) + (1 - input) * min;
}


int main(void)
{
	int res;
	printf("Flap control example\n");
	//printf("Connect a servo to any pin. It will rotate to random angles\n");
	get_i2cbus(i2c_bus, senaddr);
	// Calling wiringPi setup first.
	wiringPiSetup();

	// Setup with pinbase 300 and i2c location 0x40
	int fd = pca9685Setup(PIN_BASE, 0x40, HERTZ);
	if (fd < 0)
	{
		printf("Error in servo setup\n");
		return fd;
	}

	// Reset all output
	pca9685PWMReset(fd);


	// Set servo to neutral position at 1.5 milliseconds
	// (View http://en.wikipedia.org/wiki/Servo_control#Pulse_duration)
	float millis = 1.5;
	int tick = calcTicks(millis, HERTZ);
	pwmWrite(PIN_BASE + 16, tick);
	
	
	//IMU setup
	opmode_t newmode;
	newmode = imu;
	//newmode = ndof; //reference modes in getbno055.c line 364 onwards
	res = set_mode(newmode);
	if(res != 0) {
         printf("Error: could not set sensor mode %s [0x%02X].\n", opr_mode, newmode);
         exit(-1);
      }
	/*
	int mode = get_mode();
	if(mode < 8) {
	 printf("Error getting Euler data, sensor mode %d is not a fusion mode.\n", mode);
	 exit(-1);
	}
	*/
	delay(2000);
	
	struct bnoeul bnod;
	res = get_eul(&bnod);
	if(res != 0) {
	 printf("Error: Cannot read Euler orientation data.\n");
	 exit(-1);
	}

	


	int active = 1;
	while (active)
	{
		// That's a hack. We need a random number < 1
		//float r = rand();
		//while (r > 1)
		//	r /= 10;

		//millis = map(r, 1, 2);
		//tick = calcTicks(millis, HERTZ);

		//pwmWrite(PIN_BASE + 16, tick);
		
		res = get_eul(&bnod);
		if(res != 0) {
		 printf("Error: Cannot read Euler orientation data.\n");
		 exit(-1);
		}
		
		double angle = bnod.eul_pitc;
		if(angle > 90) angle = 90;
		if(angle < -90) angle = -90;
		
		float percent = (angle + 90)/180;
		millis = map(percent, 1, 2); 
		
		tick = calcTicks(millis, HERTZ);
		/* ----------------------------------------------------------- *
		* print the formatted output string to stdout (Example below) *
		* EUL 66.06 -3.00 -15.56 (EUL H R P in Degrees)               *
		* ----------------------------------------------------------- */
		//printf("EUL %3.4f %3.4f %3.4f \n", bnod.eul_head, bnod.eul_roll, bnod.eul_pitc);
		pwmWrite(PIN_BASE + 15, tick);
		//printf("%3.4f %3.4f %3.4f \n", angle, percent, millis);
		
		delay(100);
	}

	return 0;
}
