/*
  FreqCounter.h - 
  Using Counter1 for counting Frequency on T1 / PD5 / digitalPin 5 
  Using Timer2 for Gatetime generation

  Martin Nawrath KHM LAB3
  Kunsthochschule f�r Medien K�ln
  Academy of Media Arts
  http://www.khm.de
  http://interface.khm.de/index.php/labor/experimente/	
  
  History:
  	Dec/08 - V1.0 
  	Oct/10 - V1.1    removed occasional glitches through interference with 
  	Jan/12 - V1.2    Arduino 1.0
  	timer0 
  	                 set intterrupt timebase to 1ms
  	                 works with atmega328 
  	

	

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/
#pragma once
#include <avr/interrupt.h>
#include "Arduino.h"

namespace FreqCounter {

	extern unsigned long f_freq;
	extern volatile unsigned char f_ready;
	extern volatile unsigned char f_mlt;
	extern volatile unsigned int f_tics;
	extern volatile unsigned int f_period;
	extern volatile unsigned int f_comp;
	
	void start(int ms);
	
}

unsigned long FreqCounter::f_freq;

volatile unsigned char FreqCounter::f_ready;
volatile unsigned char FreqCounter::f_mlt;
volatile unsigned int FreqCounter::f_tics;
volatile unsigned int FreqCounter::f_period;
volatile unsigned int FreqCounter::f_comp;

void FreqCounter::start(int ms) {
}
//******************************************************************
//  Timer2 Interrupt Service is invoked by hardware Timer2 every 1ms = 1000 Hz
//  16Mhz / 128 / 125 = 1000 Hz
//  here the gatetime generation for freq. measurement takes place: 

ISR(TIMER2_COMPA_vect) {
										// multiple 2ms = gate time = 100 ms
if (FreqCounter::f_tics >= FreqCounter::f_period) {         	
                            			// end of gate time, measurement ready

   										// GateCalibration Value, set to zero error with reference frequency counter
    //  delayMicroseconds(FreqCounter::f_comp); // 0.01=1/ 0.1=12 / 1=120 sec 
    delayMicroseconds(FreqCounter::f_comp);
    TCCR1B = TCCR1B & ~7;   			// Gate Off  / Counter T1 stopped 
    TIMSK0 |=(1<<TOIE0);     			// enable Timer0 again // millis and delay
    FreqCounter::f_ready=1;             // set global flag for end count period
    
                                        // calculate now frequeny value
    FreqCounter::f_freq=0x10000 * FreqCounter::f_mlt;  // mult #overflows by 65636
    FreqCounter::f_freq += TCNT1;      	// add counter1 value
    FreqCounter::f_mlt=0;
    
    }
    FreqCounter::f_tics++;            	// count number of interrupt events
    if (TIFR1 & 1) {          			// if Timer/Counter 1 overflow flag
    FreqCounter::f_mlt++;               // count number of Counter1 overflows
    TIFR1 =(1<<TOV1);        			// clear Timer/Counter 1 overflow flag
    }
    // PORTB = PORTB ^ 32;  				// int activity test
}
