#include <signal.h>
#include <time.h>
#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "linuxanalog.h"

timer_t timer1;
uint16_t posVol; //= (uint16_t) (2.0+5.0)/10.0*4096;
uint16_t negVol; //= (uint16_t) (-2.0+5.0)/10.0*4096;
int state = 0;

void setup_timer();
void handler_function(int signum);
void setup_signal_handler();

int main(){
	double user_voltage_1 = 6;
	double user_voltage_2 = 6;
	printf("Enter voltage #1 (-5 to +5 volts, other to quit):");
	while (user_voltage_1 > 5.0 || user_voltage_1 < -5.0){	
		//printf("Enter voltage #1 (-5 to +5 volts, other to quit):");
		scanf("%lf", &user_voltage_1);
	}
	printf("Enter voltage #2 (-5 to +5 volts, other to quit):");
	while ((user_voltage_2 < -5.0 || user_voltage_2 > 5.0)){// || user_voltage_1 < user_voltage_2){
		//printf("Enter voltage #2 (-5 to +5 volts, other to quit):");
		scanf("%lf", &user_voltage_2); 
	}
	 posVol = (uint16_t) (user_voltage_1+5.0)/10.0*4095;
	 negVol = (uint16_t) (user_voltage_2+5.0)/10.0*4095;

	printf("Init... \n");
	das1602_initialize();
	printf("DAS 1602 Initted\n");
	setup_timer();
	printf("Timer Initted\n");
	setup_signal_handler();
	printf("SigHandler Initted\n");

	printf("Hello\n");
	// Do some busy work forever. The square wave
	// should be generated by the timer handler.
	uint32_t counter = 0;
	while (1){
		//Output the count value after every 100 million loops.
		if ((counter % 100000000) == 0)
		printf("Counter: %u\n", counter);
		counter++;
	}

	return 0;
}

void setup_timer(){
	long user_frequency = 10000000000000;
	if (timer_create(CLOCK_REALTIME,NULL,&timer1) != 0){
		// If there is an error, print out a message and exit.
		perror("timer_create");
		exit(1);
	}

	// get the timer resolution
	struct timespec res;
	if (clock_getres(CLOCK_REALTIME,&res) != 0){
		// If there is an error, print out a message and exit.
		perror("clock_getres");
		exit(1);
	}
	printf("System clock resolution is %lu ns\n",res.tv_nsec);
	
	// The it_value member sets the time until the timer first goes off (2.5 seconds).
	// The it_interval member sets the period of the timer after it first goes off (100 ms).
	struct itimerspec timer1_time;
	timer1_time.it_value.tv_sec = 0; // 2 seconds
	timer1_time.it_value.tv_nsec = 500000000; // 0.5 seconds (5e8 nanoseconds)
	timer1_time.it_interval.tv_sec = 0; // 0 seconds

	printf("Enter frequency (0 to 500000000.000000 Hz, other to quit):");
	scanf("%lu", &user_frequency);
	while ((user_frequency > 500000000 || user_frequency < 0)){
		printf("Enter frequency (0 to 500000000.000000 Hz, other to quit):");
		scanf("%lu", &user_frequency);
	}

	timer1_time.it_interval.tv_nsec = 500000000 / user_frequency;// *(1/2000000000);//* 50000;//5000000; // 10 milliseconds 

	// Schedule the timer.
	if (timer_settime(timer1, 0, &timer1_time, NULL) != 0){
		 //If there is an error, print out a message and exit.
		perror("timer_settime");
		exit(1);
	}
}

// Define a signal handler function. The function name can be anything, but
// it must accept a single integer.
void handler_function(int signum){
	//printf("SIGALARM catched.\n");
	
	state = !state;
	if (state){
		dac(posVol);
	} else {
		dac(negVol);
	}	
}

// This function associates the SIGALRM signal with the handler function.
void setup_signal_handler(){
	struct sigaction action;
	// Ensure that the entire structure is zeroed out.
	memset(&action, 0, sizeof(action));
	// Set the sa_handler member to point to the desired handler function.
	action.sa_handler = handler_function;
	// Call sigaction to change the action taken upon receipt of the SIGALRM signal.
	if (sigaction(SIGALRM, &action, NULL) != 0){
		// If there is an error, print out a message and exit.
		perror("sigaction");
		exit(1);
	}
}

