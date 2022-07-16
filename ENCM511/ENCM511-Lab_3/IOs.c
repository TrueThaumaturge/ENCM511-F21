/* 
 * File:   IOs.c
 * Author: MichaelDonCorleone
 *
 * Created on October 27, 2021, 3:12 PM
 */
#include <xc.h>
#include <p24F16KA101.h>
#include "IOs.h"
#include "UART2.h"

extern int CNFlag;
extern int TMR2Flag;
extern void NewClk(unsigned int);
extern void Delay_ms(unsigned int);

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

void LED_Cycle(unsigned int time_ms) {
    LATBbits.LATB8 = 0;
    Delay_ms(time_ms);
    LATBbits.LATB8 = 1;
    Delay_ms(time_ms);
}

//PB1 = RA2
//PB2 = RA4
//PB3 = RB4
void IOcheck() {
    if(CNFlag == 1 || TMR2Flag == 1) {
        LATBbits.LATB8 = 0; //turns LED off
        if(CNFlag == 1)
            DisplayPBs();
        
        CNFlag = 0;
        TMR2Flag = 0;
        
        //If no PBs are pressed
        if(PORTAbits.RA2 == 1 && PORTAbits.RA4 == 1 && PORTBbits.RB4 == 1) {
            LATBbits.LATB8 = 0; //turn LED off
        } 
        //If PB1 is pressed
        else if((PORTAbits.RA2 == 0 && PORTAbits.RA4 == 1 && PORTBbits.RB4 == 1)) {
            LED_Cycle(500); //Cycle every 500ms
        }
        //If PB2 is pressed
        else if((PORTAbits.RA2 == 1 && PORTAbits.RA4 == 0 && PORTBbits.RB4 == 1)) {
            LED_Cycle(2000); //Cycle every 2000ms
        }
        //if PB3 is pressed
        else if((PORTAbits.RA2 == 1 && PORTAbits.RA4 == 1 && PORTBbits.RB4 == 0)) {
            LED_Cycle(3000); //Cycle every 3000ms
        } 
        //if multiple PBs are pressed
        else {
            LATBbits.LATB8 = 1; //turn LED on
        }
    }
}

void DisplayPBs(){
    if(PORTAbits.RA2 == 1 && PORTAbits.RA4 == 1 && PORTBbits.RB4 == 1) {
        Disp2String    ("\r Nothing pressed        ");
    } 
    //If PB1 is pressed
    else if((PORTAbits.RA2 == 0 && PORTAbits.RA4 == 1 && PORTBbits.RB4 == 1)) {
        Disp2String    ("\r PB1 pressed            ");
    }
    //If PB2 is pressed
    else if((PORTAbits.RA2 == 1 && PORTAbits.RA4 == 0 && PORTBbits.RB4 == 1)) {
        Disp2String    ("\r PB2 pressed            ");
    }
    //if PB3 is pressed
    else if((PORTAbits.RA2 == 1 && PORTAbits.RA4 == 1 && PORTBbits.RB4 == 0)) {
        Disp2String    ("\r PB3 pressed            ");
    } 
    else {
        if(PORTAbits.RA2 == 1)
            Disp2String("\r PB2 and PB3 are pressed");
        else if(PORTAbits.RA4 == 1)
            Disp2String("\r PB1 and PB3 are pressed");
        else if(PORTBbits.RB4 == 1)
            Disp2String("\r PB1 and PB2 are pressed");
        else
            Disp2String("\r All PBs pressed        ");
    }
}