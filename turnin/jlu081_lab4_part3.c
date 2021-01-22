/*	Author: Jessie Lu
 *  Partner(s) Name: 
 *	Lab Section: 23
 *	Assignment: Lab #4  Exercise #3
 *	Exercise Description: Unlocking the doorway with a set combination and locking by pressing one button.
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */
#include <avr/io.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif

unsigned char B;
unsigned char A, A7;

enum States { Start, Lock, Unlock, Press2, Release2, PressY } State;

void Door() {
	switch(State) { //Transistion
		case Start: // initial transition
			B = 0x00;
			State = Lock;
			break;

		case Lock:
			B = 0x00;

			// if A2(#) and not A7
			if ((A == 0x04) & !A7) {
				State = Press2;
			}
			else {
				State = Lock;
			}
			break;

		case Unlock:
			B = 0x01;
			// if inside the house
			if (A7) {
				State = Lock;
			}
			else {
				State = Unlock;
			}
			break;

		case Press2:
			// if A2 and not A7
			if ((A == 0x04) & !A7) {
				State = Press2;
			}
			// if A is 0 and not A7
			else if ((A == 0x00) & !A7) {
				State = Release2;
			}
			else {
				State = Lock;
			}
			break;

		case Release2:
			// if A1 and not A7
			if ((A == 0x02) & !A7) {
				State = PressY;
			}
			else if ((A == 0x00) & !A7) {
				State = Release2;
			}
			else {
				State = Lock;
			}
			break;
			
		case PressY:
			State = Unlock;
			break;

		default:
			State = Start;
			break;
	}

	// no state actions
}

int main(void) {
	DDRA = 0x00; PORTA = 0xFF;
	DDRB = 0xFF; PORTB = 0x00;
 
	State = Start;

	while (1) {
		A = PINA & 0x07; // gets buttons X, Y and #
		A7 = PINA & 0x80; // inside house button
		Door();
		PORTB = B;
    	}

    	return 0;
}
