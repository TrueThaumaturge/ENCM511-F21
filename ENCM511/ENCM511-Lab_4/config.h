/* 
 * File:   config.h
 * Author: MichaelDonCorleone
 *
 * Created on October 27, 2021, 3:22 PM
 */

#ifndef CONFIG_H
#define	CONFIG_H
// FBS


#ifdef	__cplusplus
extern "C" {
#endif




#ifdef	__cplusplus
}
#endif
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
//#pragma config FCKSM = CSDCMD   // Clock Switching and Monitor Selection (Both Clock Switching and Fail-safe Clock Monitor are disabled)
#pragma config FCKSM = CSECMD // Clock switching is enabled, clock monitor disabled
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
/*
// MACROS for Idle, Sleep modes
#define Nop() {__asm__ volatile ("nop");}
#define ClrWdt() {__asm__ volatile ("clrwdt");}
#define Sleep() {__asm__ volatile ("pwrsav #0");}   //Sleep() - put MCU in sleep mode - CPU and some peripherals off
#define Idle() {__asm__ volatile ("pwrsav #1");}    //Idle() - put MCU in idle mode - only CPU off
#define dsen() {__asm__ volatile ("BSET DSCON, #15");} //
 */
#endif	/* CONFIG_H */

