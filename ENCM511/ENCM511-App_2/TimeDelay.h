/* 
 * File:   TimeDelay.h
 * Author: MichaelDonCorleone
 *
 * Created on October 27, 2021, 4:32 PM
 */

#ifndef TIMEDELAY_H
#define	TIMEDELAY_H

#ifdef	__cplusplus
extern "C" {
#endif




#ifdef	__cplusplus
}
#endif
void Delay_ms(unsigned int);
//interrupt implementations
void __attribute__((interrupt, no_auto_psv))_CNInterrupt(void);
void __attribute__((interrupt, no_auto_psv))_T2Interrupt(void);
#endif	/* TIMEDELAY_H */

