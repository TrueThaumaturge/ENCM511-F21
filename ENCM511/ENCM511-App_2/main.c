/*
 * File:   main.c
 * Author: Quentin J
 *
 * Created on September 26, 2020, 9:17 PM
 */

// MPLAB header libraries
#include <xc.h>

#include <p24F16KA101.h>
#include <stdio.h>
#include <math.h>
#include <errno.h>

// User header files
#include "config.h"
#include "IOs.h"
#include "TimeDelay.h"
#include "ChangeClk.h"
#include "UART2.h"

//global variable flags
int CNFlag = 0;
int TMR2Flag = 0;

//global time
int min = 0; //minutes on timer
int sec = 0; //seconds on timer
int PROG_STATE = 0;
//0: program started
//1: voltmeter
//2: ohmmeter
//3: capacitance

const int CLOCK_SPEED = 500; //500kHz

//MAIN
int main(void) {
    NewClk(32); // Initializes clock to 32 to save power
    IOinit(); // Initialize IOs and IO Change Interrupt
    while(1) {
        Sleep(); //saves power when not in use - clocks don't run until interrupt
    }
    return 0;
    
}
