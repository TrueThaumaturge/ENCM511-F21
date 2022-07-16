/*
 * File:   ReadStuff.c
 * Author: Quentin J
 *
 * Created on December 7, 2021, 11:03 PM
 */

#include <string.h>
#include <stdio.h>
#include <xc.h>
#include <p24F16KA101.h>
#include "ReadStuff.h"
#include "ADC.h"
#include "UART2.h"
#include "CTMU.h"


extern void Delay_ms(int);

void ReadVoltage() {
    while(1) {
        double voltReading = (double)do_ADC(5) * 3.25 / 1024; //returns voltage from ADC in V
        char msg[50];
        sprintf(msg, "\rVOLTMETER V = %.4fV   || ", voltReading);
        Disp2String(msg);
        ReadFrequency();
        Delay_ms(200); //updates 5 times a second
    }
}

void ReadResistance() {
    while(1) {
        double voltReading = (double) do_ADC(11) * 3.25 / 1024; //returns voltage from ADC in V
        double resistance = 1000 / ((3.25 / voltReading) - 1); //calculates resistance using voltage division
        //R2 = R1 / ((Vin / Vout) - 1)
        char msg[50];
        sprintf(msg, "\rOHMMETER R = %.0f OHMS   || ", resistance); //prints reading
        Disp2String(msg);
        ReadFrequency();
        Delay_ms(1000); //updates every second
    }
}

void ReadCapacitance() {
    while(1) {
        double voltReading = (double) do_CTMU(12);
        double capacitance = 0.55 * 0.1 / voltReading; //calculates capacitance in uF
        //C = i * t / V
        char msg[50];
        sprintf(msg, "\rCAPACITANCE C = %.4f uF   || ", capacitance); //prints reading
        Disp2String(msg);
        ReadFrequency();
        Delay_ms(800); //updates every second, taking the delays inside do_CTMU in account
    }
}

void ReadFrequency() {
    double period = 1; //placeholder - we couldn't figure out how to do it
    //"its simple and straight forward i dont even need to explain it"
    double frequency = 1 / period; //calculates frequency from measured period
    //f = 1 / T
    char msg[50];
    sprintf(msg, "FREQUENCY f = %.2f Hz                            ", frequency); //prints frequency
    Disp2String(msg);
}
