/*
 * File         : main.c
 * Assignment   : ENCM Driver Project 2
 * Author       : Beau McCartney, Quentin Jennings, Apostolos Scondrianis
 * Section      : B03 - Group 7
 * Created On   : October 17, 2021, 11:25 AM
 * Completed On :
 * Submitted on :
 */

#include "xc.h"
#include <p24F16KA101.h>

// FBS
#pragma config BWRP = OFF // Table Write Protect Boot (Boot segment may be written)
#pragma config BSS = OFF  // Boot segment Protect (No boot program Flash segment)

// FGS
#pragma config GWRP = OFF // General Segment Code Flash Write Protection bit (General segment may be written)
#pragma config GCP = OFF  // General Segment Code Flash Code Protection bit (No protection)

// FOSCSEL
#pragma config FNOSC = FRCDIV // Oscillator Select (8 MHz FRC oscillator with divide-by-N (FRCDIV))
#pragma config IESO = ON      // Internal External Switch Over bit (Internal External Switchover mode enabled (Two-Speed Start-up enabled))

// FOSC
#pragma config POSCMOD = NONE   // Primary Oscillator Configuration bits (Primary oscillator disabled)
#pragma config OSCIOFNC = OFF   // CLKO Enable Configuration bit (CLKO output signal is active on the OSCO pin)
#pragma config POSCFREQ = HS    // Primary Oscillator Frequency Range Configuration bits (Primary oscillator/external clock input frequency greater than 8 MHz)
#pragma config SOSCSEL = SOSCHP // SOSC Power Selection Configuration bits (Secondary oscillator configured for high-power operation)
#pragma config FCKSM = CSDCMD   // Clock Switching and Monitor Selection (Both Clock Switching and Fail-safe Clock Monitor are disabled)

// FWDT
#pragma config WDTPS = PS32768 // Watchdog Timer Postscale Select bits (1:32,768)
#pragma config FWPSA = PR128   // WDT Prescaler (WDT prescaler ratio of 1:128)
#pragma config WINDIS = OFF    // Windowed Watchdog Timer Disable bit (Standard WDT selected; windowed WDT disabled)
#pragma config FWDTEN = ON     // Watchdog Timer Enable bit (WDT enabled)

// FPOR
#pragma config BOREN = BOR3  // Brown-out Reset Enable bits (Brown-out Reset enabled in hardware; SBOREN bit disabled)
#pragma config PWRTEN = ON   // Power-up Timer Enable bit (PWRT enabled)
#pragma config I2C1SEL = PRI // Alternate I2C1 Pin Mapping bit (Default location for SCL1/SDA1 pins)
#pragma config BORV = V18    // Brown-out Reset Voltage bits (Brown-out Reset set to lowest voltage (1.8V))
#pragma config MCLRE = ON    // MCLR Pin Enable bit (MCLR pin enabled; RA5 input pin disabled)

// FICD
#pragma config ICS = PGx2 // ICD Pin Placement Select bits (PGC2/PGD2 are used for programming and debugging the device)

// FDS
#pragma config DSWDTPS = DSWDTPSF // Deep Sleep Watchdog Timer Postscale Select bits (1:2,147,483,648 (25.7 Days))
#pragma config DSWDTOSC = LPRC    // DSWDT Reference Clock Select bit (DSWDT uses LPRC as reference clock)
#pragma config RTCOSC = SOSC      // RTCC Reference Clock Select bit (RTCC uses SOSC as reference clock)
#pragma config DSBOREN = ON       // Deep Sleep Zero-Power BOR Enable bit (Deep Sleep BOR enabled in Deep Sleep)
#pragma config DSWDTEN = ON       // Deep Sleep Watchdog Timer Enable bit (DSWDT enabled)

void IOcheck();

//Global Variables
int CNFlag = 0; //Input interrupt flag
int TMR2Flag = 0; //Timer 2 flag

/*
 * Function void IOinit() sets up our input and output pin
 * behavior for the program we are building.
 */
void IOinit() {
    TRISBbits.TRISB8 = 0; //SET GPIO RB8 as a digital output.
	
    TRISAbits.TRISA2 = 1; //SET GPIO RA2 as a digital input.
    TRISAbits.TRISA4 = 1; //SET GPIO RA4 as a digital input.
    TRISBbits.TRISB4 = 1; //SET GPIO RB4 as a digital input.
	
    CNPU2bits.CN30PUE = 1; //Enables Pull up resistor on RA2 / CN30
    CNPU1bits.CN0PUE = 1; //Enables Pull up resistor on RA4 / CN0
    CNPU1bits.CN1PUE = 1; //Enables Pull up resistor on RB4 / CN1
	
    AD1PCFG = 0xFFFF; // Turn all analog pins as digital
	
    IPC4bits.CNIP = 7; //sets priority for Input Change Notification
	IFS1bits.CNIF = 0; //Clear the CNI flag
    IEC1bits.CNIE = 1; //Input Change Notification Interrupts Enabled
    CNEN1bits.CN1IE = 1; //Enable input change Notif on RB4
    CNEN1bits.CN0IE = 1; //Enable input change Notif on RA4
    CNEN2bits.CN30IE = 1; //Enable input change Notif on RA2
}

void Delay_ms(unsigned int time_ms) {
    //Configuring T2CON register
    T2CONbits.T32 = 0; //Operate as 16 bit timer
    T2CONbits.TCS = 0; // Set internal clock usage

    //Prescaler settings of T2CON register
    T2CONbits.TCKPS0 = 1; //
    T2CONbits.TCKPS1 = 1; // These two lines set prescaling to 256x

    //Interrupt Configuration for Timer 2
    IEC0bits.T2IE = 1; //Timer 2 interrupt enabled
    IFS0bits.T2IF = 0; //Clear Timer 2 Flag

    PR2 = ((time_ms / 1000) * 15625)/4; //Sets number of clock cycles needed
    T2CONbits.TON = 1; //Timer 2 Starts here
    TMR2Flag = 1; //Timer 2 Global variable is enabled
    Idle(); //Puts the processor in idle mode while timer goes down
    return;
}

void LED_Cycle(unsigned int time_ms) {
    LATBbits.LATB8 = 1; //turns light on
    Delay_ms(time_ms); //starts a timer that delays for time_ms
    LATBbits.LATB8 = 0; //turns light off
    Delay_ms(time_ms); //starts a timer that delays for time_ms
}

void IOcheck() {
    if(CNFlag == 1 || TMR2Flag == 1) {
        //no buttons pressed
        if(PORTAbits.RA2 == 1 && PORTAbits.RA4 == 1 && PORTBbits.RB4 == 1) {
            CNFlag = 0; // Set our CN Global Flag to False after we handle the interrupt
            LATBbits.LATB8 = 0; //turn LED off in case no button is pressed
            Idle(); //puts processor in idle mode
        //first button pressed (RA2)
        } else if((PORTAbits.RA2 == 0 && PORTAbits.RA4 == 1 && PORTBbits.RB4 == 1)) {
            CNFlag = 0; // Set our CN Global Flag to False after we handle the interrupt
            LED_Cycle(1000); //1s cycle
        //second button pressed (RA4)
        } else if((PORTAbits.RA2 == 1 && PORTAbits.RA4 == 0 && PORTBbits.RB4 == 1)) {
            CNFlag = 0; // Set our CN Global Flag to False after we handle the interrupt
            LED_Cycle(2000); //2s cycle
        //third button pressed (RB4)
        } else if((PORTAbits.RA2 == 1 && PORTAbits.RA4 == 1 && PORTBbits.RB4 == 0)) {
            CNFlag = 0; // Set our CN Global Flag to False after we handle the interrupt
            LED_Cycle(3000); //3s cycle
        //multiple buttons pressed
        } else {
            CNFlag = 0; // Set our CN Global Flag to False after we handle the interrupt
            LATBbits.LATB8 = 1; //turn LED on if multiple buttons are pressed
            Idle(); //puts processor in idle mode
        }
    }
    else
        Idle();
}

 void __attribute__((interrupt, no_auto_psv))_CNInterrupt(void)
 {
    TMR2Flag = 0; //Reset the timer Flag to indicate an input interrupted 
                  //the LED Flashing
    IFS1bits.CNIF = 0; //Clear interrupt Register Flag
    CNFlag = 1; // Set our CN Global Flag to True/1
 }

 
void __attribute__((interrupt, no_auto_psv))_T2Interrupt(void) {
    IFS0bits.T2IF = 0; //Clear timer 2 interrupt flag
    T2CONbits.TON = 0; //Stop timer
    TMR2 = 0; //Reset the TMR2 register after the interrupt occurs.
} 


int main(void) {
    IOinit();
    while(1) {
        IOcheck();
    }
    return 0;
}