/*	Author: Jessie Lu
 *  Partner(s) Name: 
 *	Lab Section: 23
 *	Assignment: Lab #4  Exercise #5
 *	Exercise Description: Continues from part 4, door also allows for 4-button sequence #-X-Y-X 
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
unsigned char prev;
unsigned char code, i, ACheck, j;
unsigned char combo[] = { 0x04, 0x00, 0x01, 0x00 }; // array of 4 sequence

enum States { Start, Lock, Unlock, Press, Release } State;

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
				prev = State;
				State = Press;
			}
			else {
				State = Lock;
			}
			break;

		case Unlock:
			B = 0x01;
			if ((A == 0x04) & !A7) {
				prev  = State;
				State = Press;
			}
			// if inside the house
			else if (A7) {
				State = Lock;
			}
			else {
				State = Unlock;
			}
			break;

		case Press:
			if ((A == 0x04) & !A7) {
				State = Press;
			}
			else if (((A == 0x00) & !A7) || ((A == 0x02) & !A7)) {
				State = Release;
			}
			else {
				if (prev == Lock) {
					State = Lock;
				}
				else {
					State = Unlock;
				}
			}
			break;

		case Release:
			// if A1 and not A7
			if ((A == 0x01) & !A7) {
				State = PressY;
			}
			else if ((A == 0x00) & !A7) {
				State = Release;
			}
			else {
				if (prev == Lock) {
					State = Lock;
				}
				else {
					State = Unlock;
				}
			}
			break;
			
		case PressY:
			if (prev == Lock) {
				State = Unlock;
			}
			else {
				State = Lock;
			}
			break;

		default:
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
		//Door();
		PORTB = B;
    	}

    	return 0;
}
