/* 
 * File:   IOs.c
 * Author: MichaelDonCorleone
 *
 * Created on October 27, 2021, 3:12 PM
 */
#include <string.h>
#include <stdio.h>
#include <xc.h>
#include <p24F16KA101.h>
#include "IOs.h"
#include "UART2.h"
#include "ADC.h"
#include "ReadStuff.h"

extern int CNFlag;
extern int TMR2Flag;
extern int CLOCK_SPEED;
extern int PROG_STATE;

extern void NewClk(unsigned int);
extern void Delay_ms(unsigned int);

extern void ReadVoltage();
extern void ReadResistance();

int PB1_Pressed = 0;
int PB2_Pressed = 0;
int PB3_Pressed = 0;

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
}

//PB1 = RA2
//PB2 = RA4
//PB3 = RB4
//Works under the assumption that PB3 and PB1/2 won't be pressed at the same time

void IOCheck() {
    NewClk(CLOCK_SPEED);
    CheckPB1(); //Checks PB1 and deals with it.
    CheckPB2(); //Checks PB2 and deals with it
    CheckPB3(); //Checks PB3 and deals with it
}

void CheckPB1() {
    if(CNFlag == 1) {
        CNFlag = 0;
        //Handle button one.
        if(PORTAbits.RA2 == 0) {
            //PB1 is pressed
            if(PB1_Pressed == 0) {
                //PB1 wasn't pressed previously let's set PB1 globbal to pressed
                PB1_Pressed = 1;
            } else {
                CNFlag = 1;
            }
        } else {
            //PB1 isn't pressed.
            if(PB1_Pressed == 1) {
                //it was previously pressed though.
                PB1_Pressed = 0;
                ReadVoltage();
            } else {
                CNFlag = 1;
            }
        }
    }
}

void CheckPB2() {
    if(CNFlag == 1) {
        CNFlag = 0;
        //Handle button two.
        if(PORTAbits.RA4 == 0) {
            //PB3 is pressed
            if(PB2_Pressed == 0) {
                //PB2 wasn't pressed previously let's set PB2 globbal to pressed
                PB2_Pressed = 1;
            } else {
                CNFlag = 1;
            }
        } else {
            //PB2 isn't pressed.
            if(PB2_Pressed == 1) {
                PB2_Pressed = 0;
                //it was previously pressed though.
                ReadResistance();
            } else {
                CNFlag = 1;
            }
        }
    }
}
void CheckPB3() {
    if(CNFlag == 1) {
        CNFlag = 0;
        //Handle button three.
        if(PORTBbits.RB4 == 0) {
            //PB3 is pressed
            if(PB3_Pressed == 0) {
                //PB3 wasn't pressed previously let's set PB3 globbal to pressed
                PB3_Pressed = 1;
            } else {
                CNFlag = 1;
            }
        } else {
            //PB3 isn't pressed.
            if(PB3_Pressed == 1) {
                //it was previously pressed though.
                PB3_Pressed = 0;
                ReadCapacitance();
            } else {
                CNFlag =1;
            }
        }
    }
}