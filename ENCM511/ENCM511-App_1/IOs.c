/* 
 * File:   IOs.c
 * Author: MichaelDonCorleone
 *
 * Created on October 27, 2021, 3:12 PM
 */
#include <string.h>
#include <xc.h>
#include <p24F16KA101.h>
#include "IOs.h"
#include "UART2.h"

extern int CNFlag;
extern int TMR2Flag;
extern int min;
extern int sec;

extern void NewClk(unsigned int);
extern void Delay_ms(unsigned int);

int RESET_ATTEMPT = 0; //flag that keeps track of a timer reset attempt

void IOinit() {
    TRISBbits.TRISB8 = 0; //SET GPIO RB8 as a digital output.
	
    TRISAbits.TRISA2 = 1; //SET GPIO RA2 as a digital input.
    TRISAbits.TRISA4 = 1; //SET GPIO RA4 as a digital input.
    TRISBbits.TRISB4 = 1; //SET GPIO RB4 as a digital input.
	
    CNPU2bits.CN30PUE = 1; //Enables Pull up resistor on RA2/ CN30
    CNPU1bits.CN0PUE = 1; //Enables Pull up resistor on RA4/ CN0
    CNPU1bits.CN1PUE = 1; //Enables Pull resistor on RB4 / CN1
	
    AD1PCFG = 0xFFFF; // Turn all analog pins as digital
	
    IPC4bits.CNIP = 4; //sets priority for Input Change Notification
	IFS1bits.CNIF = 0; //Clear the CNI flag
    IEC1bits.CNIE = 1; //Input Change Notification Interrupts Enabled
    CNEN1bits.CN1IE = 1; //Enable input change Notif on RB4
    CNEN1bits.CN0IE = 1; //Enable input change Notif on RA4
    CNEN2bits.CN30IE = 1; //Enable input change Notif on RA2
    LATBbits.LATB8 = 0; //Turn off LED on start up.
}

//PB1 = RA2
//PB2 = RA4
//PB3 = RB4
void IOCheck() {
    if(PROG_STATE == 2) //clears an ALARM flag if there is one
        PROG_STATE = 0;
    
    if(PROG_STATE == 0) //lets you increase the time if a countdown isn't running
        TimeIncrease(); //calls function that deals with PB1 and PB2
    PB3Check(); //calls function that deals with PB3
    
    if(PROG_STATE == 1) //decreases the time if a countdown is running
        TimeDecrease();
}

void TimeIncrease() {
    while(PORTABITS.RA2 == 0 || PORTABITS.RA4 == 0) {  
        //If PB1 is pressed, increases minutes
        if(PORTABITS.RA2 == 0 && min < 60)
            min++;

        //If PB2 is pressed, increases seconds
        if(PORTABITS.RA4 == 0 && sec < 60)
            sec++;

        DisplayTime(); //displays the updated time
        Delay_ms(200); //creates a 200ms timer to wait
        TMR2Flag = 0; //sets flag to 0, not needed here
    }
}

void PB3Check() {
    //if we just pressed the button down, start a timer to check if it's held for 3s
    if(PORTBBITS.RB4 == 0) {
        RESET_ATTEMPT = 1; //flags that a reset attempt is occurring
        Delay_ms(3000); //creates a 3 second timer
    }
    
    //if the timer succeeds, reset the countdown time
    if(RESET_ATTEMPT == 1 && TMR2Flag == 1) {
        RESET_ATTEMPT = 0; //clears reset attempt flag
        TMR2Flag = 0; //clears TMR2 flag
        PROG_STATE = 0; //resets program state flag
        
        min = 0; //resets minutes
        sec = 0; //resets seconds
        DisplayTime(); //displays the new, reset time
    }
    
    //if the button is released after a reset attempt, instead starts the timer
    else if(PORTBBITS.RB4 == 1 && RESET_ATTEMPT == 1 && TMR2Flag == 0) {
        RESET_ATTEMPT = 0; //clears reset attempt flag
        PROG_STATE = 1; //sets program state flag to countdown
    }
}

void TimeDecrease() {
    if(sec == 0) { //if sec = 0, rolls over and decreases minute by 1
        if(min > 0){
            min--; //decreases minute
            sec = 59; //sets seconds to 59 (60 - 1)
        }
        else { //if sec = 0 and min = 0, timer is up
            PROG_STATE = 2;
        }
    }
    else
        sec--; //decreases second
    
    
}

void DisplayTime() {
    char str[18]; //string to be displayed as: ##m : ##s
    sprintf(str, "\r%02dm : %02ds         ", min, sec); //converts min and sec into formatted string
    if(PROG_STATE == 2) //if alarm is active, prints ALARM
        strcat(str, " -- ALARM");
    else //otherwise prints blank space
        strcat(str, "         ");
    Disp2String(str); //prints string to console
}