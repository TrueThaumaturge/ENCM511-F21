/* 
 * File:   TimeDelay.c
 * Author: MichaelDonCorleone
 *
 * Created on October 27, 2021, 3:12 PM
 */
#include <p24F16KA101.h>
#include <xc.h>

extern const int CLOCK_SPEED;

extern void UpdatePBFlag();
extern void IOcheck();

extern int TMR2Flag;
extern int CNFlag;

void Delay_ms(unsigned int time_ms) {
    //Configuring T2CON register
    T2CONbits.T32 = 0; //Operate as 16 bit timer
    T2CONbits.TCS = 0; // Set internal clock usage
    T2CONbits.TSIDL = 0; //Continue timer operation in idle mode
    
    //Prescaler settings of T2CON register
    T2CONbits.TCKPS0 = 1; //
    T2CONbits.TCKPS1 = 1; // These two lines set prescaling to 256x

    TMR2 = 0; //Clears TMR2
    
    //Interrupt Configuration for Timer 2
    IEC0bits.T2IE = 1; //Timer 2 interrupt enabled
    IFS0bits.T2IF = 0; //Clear Timer 2 Flag
    IPC1bits.T2IP = 3; //Set Priority level = 3
    
    //PR2 = time_ms * fclk (hz) / (prescaler * 1000 * 2)
    //PR2 = time_ms * fclk (hz) / (256 * 2000)
    //PR2 = time_ms * fclk (hz) / 512000
    
    if (CLOCK_SPEED == 8) //8MHz
    {
        PR2 = time_ms * 8000000 / 512000;
        //PR2 = time_ms * 125 / 16; //Number of clock cycles that need to elapse, simplified from formula
        
    }
    else if (CLOCK_SPEED == 500) // 500 kHz
    {
        PR2 = time_ms * 500000 / 512000;
        //PR2 = time_ms * 125 / 128; //Number of clock cycles that need to elapse
    }
    else if (CLOCK_SPEED == 32) //32 kHz
    {
        PR2 = time_ms * 32000 / 512000;
        //PR2 = time_ms / 16; //Number of clock cycles that need to elapse
    }

    T2CONbits.TON = 1; //Timer 2 Starts here

    Idle();
    return;
}

 void __attribute__((interrupt, no_auto_psv))_CNInterrupt(void)
 {
    IFS1bits.CNIF = 0; //Clear interrupt Register Flag
    Delay_ms(25); //debounce
    LATBbits.LATB8 = 0; //turns LED off
    T2CONbits.TON = 0; //Stops the timer
    CNFlag = 1; // Set our CN Global Flag to True/1
    IOcheck();
 }

 
void __attribute__((interrupt, no_auto_psv))_T2Interrupt(void) {
    IFS0bits.T2IF = 0; //Clear timer 2 interrupt flag
    T2CONbits.TON = 0; //Stop timer
    TMR2 = 0; //Clears Timer 2 register
    TMR2Flag = 1; //Enables Timer 2 global variable
    
    IOcheck();
} 
