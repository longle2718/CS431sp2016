#include <p33Fxxxx.h>
//do not change the order of the following 3 definitions
#define FCY 12800000UL 
#include <stdio.h>
#include <libpic30.h>

#include "lcd.h"
#include "led.h"

/* Initial configuration by EE */
// Primary (XT, HS, EC) Oscillator with PLL
_FOSCSEL(FNOSC_PRIPLL);

// OSC2 Pin Function: OSC2 is Clock Output - Primary Oscillator Mode: XT Crystal
_FOSC(OSCIOFNC_OFF & POSCMD_XT); 

// Watchdog Timer Enabled/disabled by user software
_FWDT(FWDTEN_OFF);

// Disable Code Protection
_FGS(GCP_OFF);  


void main(){
	//Init LCD
	__C30_UART=1;
        int count = 0;
        int DBcount = 0;
        int Tcount = 0;
        int DBNOcount = 0;
        int LED4Status = 0;
        int PrevStat = 0;
     
	lcd_initialize();
	led_initialize();

	lcd_clear();
	lcd_locate(0,0);
	
	while(1);
}
