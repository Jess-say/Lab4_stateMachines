/*	Author: Jessie Lu
 *  Partner(s) Name: 
 *	Lab Section: 23
 *	Assignment: Lab #4  Exercise #2
 *	Exercise Description: Output for PORTC is initially 7, pressing PA0 increments PORC once and stops at 9. Pressing PA1 decrements PORTC once and stops at 0. If both buttons are depressed PORTC is set to 0. 
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */
#include <avr/io.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif

unsigned char C = 0x07; // initially set to 7
unsigned char A;

enum States { Start, Wait, Inc, Dec, Depress, Reset } State;

void Button() {
	switch(State) { //Transistion
		case Start: // initial transition
			State = Wait;
			break;

		case Wait:
			// if A0 is pressed - increment
			if (A == 0x01) {
				State = Inc;
			}
			// decrement if A1 is pressed
			else if (A == 0x02) {
				State = Dec;
			}
			// if both A0 and A1 are pressed - reset
			else if (A == 0x03) {
				State = Reset;
			}
			// if no buttons are pressed
			else {
				State = Wait;
			}
			break;

		case Inc:
			if (A == 0x01) { 
				State = Inc;
			}
			else if (A == 0x02) {
				State = Dec;
			}
			else if (A == 0x03) {
				State = Reset;
			}
			else {
				State = Depress;
			}
			break;

		case Dec:
			if (A == 0x01) {
				State = Inc;
			}
			else if (A == 0x02) {
				State = Dec;
			}
			else if (A == 0x03) {
				State = Reset;
			}
			else {
				State = Depress;
			}
			break;

		case Reset:
			if (A == 0x00) {
				State = Depress;
			}
			else if (A == 0x01) {
				State = Inc;
			}
			else if (A == 0x02) {
				State = Dec;
			}

			else {
				State = Wait;
			}
			break;

		case Depress:
			if (A == 0x01) {
				State = Inc;
			}
			else if (A == 0x02) {
				State = Dec;
			}
			else if (A == 0x03) {
				State = Reset;
			}
			else {
				State = Depress;
			}
			break;

		default:
			break;
	}

	switch(State) {
		case Start:
			C = 0x07; // Initially
			break;

		case Inc:
			if (C < 9) {
				C = C + 1;
			}
			break;

		case Dec:
			if (C > 0) {
				C = C - 1;
			}
			break;

		case Reset:
			C = 0x00;
			break;

		default:
			break;

	}
}

int main(void) {
	DDRA = 0x00; PORTA = 0xFF;
	DDRC = 0xFF; PORTC = 0x00;
 
	State = Start;

	while (1) {
		A = 0x03; // gets first and second bit
		Button();
		PORTC = C;
    	}
	

    	return 0;
}
