#include <p24F16KA101.h>
#include <xc.h>
#include "ADC.h"
#include "UART2.h"

extern void Delay_ms(unsigned int);
extern void XmitUART2(char, unsigned int);
extern void Disp2Hex(unsigned int);

void init_ADC(void) {
    //This function configures the ADC Module
    AD1CON1bits.ADON = 1; //Power On the ADC module
    AD1CON1bits.FORM = 0; //Set the Data output format as Integer
    //AD1CON1bits.ADSIDL = 1; //Discontinue module operation when device enters idle mode.
    AD1CON1bits.SSRC0 = 1; //Internal counter ends sampling and starts conversion (auto converts) SSRC bits = 111;
    AD1CON1bits.SSRC1 = 1; 
    AD1CON1bits.SSRC2 = 1;
    AD1CON1bits.ASAM = 0; //Sampling begins when SAMP is set.
    //AD1CON1bits.SAMP = 1; //A/D sample/hold amplifier is sampling input
    //AD1CON1bits.DONE //BIT indicates whether conversion is done or not. Auto set when ADC is done converting.
    //Vref selection
    AD1CON2bits.VCFG = 0; //Set reference as AVdd and AVss
    AD1CON2bits.CSCNA = 0; //Don't scan inputs.
    //AD1CON2bits.SMPI = 0; //Use this only if interrupts are used. -> Interrupts at the completion of conversion for each sample/convert sequence
    AD1CON2bits.BUFM = 0; //Buffer configured as one 16-word buffer
    AD1CON2bits.ALTS = 0; //Use MUX A input multiplexer settings
    AD1CON3bits.ADRC = 0; //Clock is derived from our system clock
    AD1CON3bits.SAMC0 = 0; //Auto-Sample Time bits.
    AD1CON3bits.SAMC1 = 0;
    AD1CON3bits.SAMC2 = 0;
    AD1CON3bits.SAMC3 = 0;
    AD1CON3bits.SAMC4 = 1; 
    //We Chose Auto Sample bits value 31 decimal ie 1000 for the slowest Sampling time = 16*2/ClockFrequency
    //This results in a 0.001 s per sample.
    //AD1CON3bits.ADCS -> Clock Prescaler in the case you chose ADRC = 1;
    AD1CHSbits.CH0NA = 0; //Channel negative input is VR-
    AD1CHSbits.CH0SA0 = 1; //Set as Positive input PIN 8 AKA AN5. If we have errors check this here again. not sure
    AD1CHSbits.CH0SA1 = 0;
    AD1CHSbits.CH0SA2 = 1;
    AD1CHSbits.CH0SA3 = 0;
    AD1PCFGbits.PCFG5 = 0; //Pin configured in Analog mode; I/O port read is disabled, A/D samples pin voltage.
    //Input Scan Setup configuration. For this project we are omitting analog channel from input scan.
    AD1CSSLbits.CSSL0 = 0;
    AD1CSSLbits.CSSL1 = 0;
    AD1CSSLbits.CSSL2 = 0;
    AD1CSSLbits.CSSL3 = 0;
    AD1CSSLbits.CSSL4 = 0;
    AD1CSSLbits.CSSL5 = 0;
    AD1CSSLbits.CSSL10 = 0;
    AD1CSSLbits.CSSL11 = 0;
    AD1CSSLbits.CSSL12 = 0;
    //Interrupt Setup (optional)
    //IPC3bits.AD1IP PRIORITY BITS 1->7
    //IEC0bits.AD1IE = 1;// ENABLE ADC Interrupt
    //IFS0bits.AD1F = 0; //Clear interrupt flag in void__attribute__((interrupt, no_auto_psv))_ADC1Interrupt(void) {} function
    //Implement stuff in interrupt or set global function
}

void RunADC(void) {
    uint32_t avg_value; //declare average_value variable
    int i = 0; // initialize i guard to 0
    while(i < 10) { //loop till we get 10 measurements in 10 secs averaged over 1000 measurements per second.
        avg_value = do_ADC(); //find an average measurement over 1 minute
        //Delay_ms(1000); //WHY DOESNT THIS WORK???
        
        Disp2String("\n\r"); //new line/return carriage
        XmitUART2('|', avg_value/ 15); //print bar graph
        Disp2Hex(avg_value); //print value
        
        i++; // increase our guard by 1
    }
    CNEN1bits.CN1IE = 1; //Enable input change Notif on RB4/PB3
    CNEN1bits.CN0IE = 1; //Enable input change Notif on RA4/PB2
    CNEN2bits.CN30IE = 1; //Enable input change Notif on RA2/PB1
}

uint16_t do_ADC(void) {
    int measured_count = 0; //Counter for the 1000 measurements initialized to 0
    uint32_t sum = 0; //sum of measurements initialized to zero
    
    init_ADC(); //initialize the ADC unit
    while(measured_count < 1000) { //start measuring ADC values 100 times
        AD1CON1bits.SAMP = 1; //enable the sampler
        while(AD1CON1bits.DONE == 0) {
        } //wait for sampling + conversion to finish.
        sum += ADC1BUF0; //take the converted value and added to the sum
        AD1CON1bits.SAMP = 0; //turn off samplers
        measured_count++; //increment the measured values by one.
        
        //power inefficient but it works for now HAHA TAKE THAT DELAY_MS
        //We'll figure it out later right? Right????
        int j = 0; //busy loop to emulate delay. We had issues with our delay MS cooperating with the ADC.
        while(j < 21){
            j++;
        }
    }
    uint16_t avg_value = sum / 1000; //calculate average value as sum_of_values_measured/1000
    AD1CON1bits.ADON = 0;//turn off ADC
    
    return avg_value; //RETURN THE average of 1000 measurements.
}
