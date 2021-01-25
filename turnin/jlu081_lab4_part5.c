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
unsigned char i, prev;
unsigned char combo[] = { 0x04, 0x01, 0x02, 0x01 }; // array of size 3

enum States { Start, Lock, Unlock, Inc, Wait_Lock, Wait_Unlock } State;

void Door() {
	switch(State) { //Transistion
		case Start: // initial transition
			B = 0x00;
			i = 0;
			State = Lock;
			break;

		case Lock:
			B = 0x00;
			i = 0; // MUST include in state
			// if combo matches and not A7
			if ((A == combo[i]) && !A7) {
				prev = Lock;
				State = Inc;
			}
			else if ((A == 0x00) || !A7) {
				State = Lock;
			}
			else {
				State = Lock;
			}
			break;

		case Unlock:
			B = 0x01;
			i = 0; // MUST include
			// if no buttons are pressed and not A7
			if ((A == 0x00) && !A7) {
				State = Unlock;
			}
			// if inside the house
			else if (A7) {
				State = Lock;
			}
			// if A matched combo
			else if ((A == combo[i]) && !A7){
				prev = Unlock;
				State = Inc;
			}
			else {
				State = Unlock;
			}
			break;

		case Inc:
			i = i + 1; // increment i for combo
			// need two one for lock and one for unlock
			if (prev == Lock) {
				State = Wait_Unlock;
			}
			else {
				State = Wait_Lock;
			}
			break;

		case Wait_Lock:
			if (i == 4) {
				State = Lock;
			}
			if (A == 0x00) {
				State = Wait_Lock;
			}
			else if (A == combo[i]) {
				//prev = Unlock; // keep track so it goes to right inc
				State = Inc;
			}
			else if ((A == combo[i - 1])) {
				State = Wait_Lock;
			}
			else {
				State = Lock;
			}
			break;
			
		case Wait_Unlock:
			if (i == 4) {
				State = Unlock;
			}
			else if (A == 0x00) {
				State = Wait_Unlock;
			}
			else if (A == combo[i]) {
				//prev = Lock;
				State = Inc;
			}
			else if (A == combo[i - 1]) {
				State = Wait_Unlock;
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
		Door();
		PORTB = B;
    	}

    	return 0;
}
