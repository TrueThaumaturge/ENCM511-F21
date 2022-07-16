/*
 * File:   CTMU.c
 * Author: TrueThaumaturge
 *
 * Created on December 8, 2021, 11:45 PM
 */


#include "xc.h"
#include <p24F16KA101.h>
#include "TimeDelay.h"
#include "ADC.h"

extern void Delay_ms(unsigned int);

void init_CTMU(void) { 
    //Process to initialize CTMU for capacitance measuring
    //Step 1 - Current Source Range
    CTMUICONbits.IRNG1 = 0; // 11 is 100x, and 01 is base current
    CTMUICONbits.IRNG0 = 1; //set current to base current (0.55mA)
    //00 disables the current source.
    //Step2 - Current source Trim Adjustment - We set this up as the Default - No Trim
    CTMUICONbits.ITRIM5 = 0;
    CTMUICONbits.ITRIM4 = 0;
    CTMUICONbits.ITRIM3 = 0;
    CTMUICONbits.ITRIM2 = 0;
    CTMUICONbits.ITRIM1 = 0;  //No Charge Amplification
    CTMUICONbits.ITRIM0 = 0; // -> Nominal current output specified by IRNG bits
    //Step 3 - Configuration of Edge input sources for Edge 1
    CTMUCONbits.EDG1SEL1 = 0; // We choose 00 for Timer1 Edge 1 module
    CTMUCONbits.EDG1SEL0 = 0; 
    CTMUCONbits.EDG2SEL1 = 0; // We choose 00 for Timer1 Edge 2 module
    CTMUCONbits.EDG2SEL0 = 0; 
    //We can use 01 -> OC1 module 10-> CTED2 pin, 11 -> CTED1;
    //Step 4 - Configure input Polarities
    CTMUCONbits.EDG1POL = 1; //Positive edge response Low to High transitions
    //0 is for High to Low which is default
    CTMUCONbits.EDG2POL = 1; // positive edge response Low to High transitions
    //Step 5 - Enable edge sequencing -> default is disabled
    CTMUCONbits.EDGSEQEN = 1;
    //STEP 6 - Select Operating mode -> Default Mode is Time/ Capacitance
    CTMUCONbits.TGEN = 1; //Time generation bit -> 1 ??
    //Step 7 - Trigger A/D conversion at second edge event -> Default is disabled
    CTMUCONbits.CTTRIG = 0; //disabled
    //Step 8 - Discharge the connected circuit
    CTMUCONbits.IDISSEN = 0; //Analog current source grounded
    Delay_ms(100); //100ms delay
    //Step 9 - Disable the module by clearing CTMUEN
    CTMUCONbits.CTMUEN = 0; //disable module
    //Step 10 - Clear edge status bits
    CTMUCONbits.EDG1STAT = 0; //Clear EDGE 1 Status bit
    CTMUCONbits.EDG2STAT = 0; //Clear EDGE 2 Status bit
    //Step 11 - Enable both edge inputs
    CTMUCONbits.EDGEN = 1; //EDGES not blocked.
    //STEP 12 - Enable module 
    CTMUCONbits.CTMUEN = 1;
}

unsigned int do_CTMU(int pin) {
    unsigned int Vread = 0;
    unsigned int Vavg = 0;
    unsigned int VTot = 0;
    init_ADC(pin);
    init_CTMU();
    int i;
    int j = 0; //counter
    int COUNT = 1;
    while(j < COUNT) { //takes 10 measurements
        CTMUCONbits.IDISSEN = 1; //drain charge on the circuit
        Delay_ms(100); //100ms delay
        CTMUCONbits.IDISSEN = 0; //end drain of circuit
        
        AD1CON1bits.SAMP = 1;  //Manual sampling start - move to after IDISSEN=0 if long time delay
           
        CTMUCONbits.EDG1STAT = 1; //Begin charging the circuit
        //using CTMU current source
        Delay_ms(100); //100ms delay
        CTMUCONbits.EDG1STAT = 0; //Stop charging circuit
        
        IFS0bits.AD1IF = 0; //make sure A/D Int not set
        AD1CON1bits.SAMP = 0; //and begin A/D conv.
        while(!AD1CON1bits.DONE) {}; //Wait for A/D convert complete
        Vread = ADC1BUF0; //Get the value from the A/D
        IFS0bits.AD1IF = 0; //Clear A/D Interrupt Flag
        VTot += Vread; //Add the reading to the total
        j++; //increments loop
    }
    AD1CON1bits.ADON = 0;//turn off ADC
    
    CTMUCONbits.IDISSEN = 1; //drain charge on the circuit
    Delay_ms(100); //100ms delay
    CTMUCONbits.IDISSEN = 0; //end drain of circuit
    
    //If you are using multiple loops, don't forget to divide to average
    Vavg = (VTot) / COUNT; //avg of COUNT Measurements
    
    return Vavg; //returns the capacitance's voltage reading
}