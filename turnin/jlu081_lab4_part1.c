/*	Author: Jessie Lu
 *  Partner(s) Name: 
 *	Lab Section: 23
 *	Assignment: Lab #4  Exercise #1
 *	Exercise Description: PB0's light is initially on and pressing PA0 turns it off and turns on PB1's light, it stays this way even when button(PA0) is released. If button is pressed again PB1's light turns off and PB0's light turns on.
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */
#include <avr/io.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif

unsigned char B = 0x01;
unsigned char A0;

enum States { Start, ON_Release, OFF_Release, ON_Press, OFF_Press } State;

void Button() {
	switch(State) { //Transistion
		case Start: // initial transition
			B = 0x01; // B0 = 1 and B1 = 0
			State = OFF_Release;
			break;

		case OFF_Release:
			if (A0) {
				B = 0x02;
				State = ON_Press;
			}
			else {
				State = OFF_Release;
			}
			break;

		case ON_Press:
			State = A0 ? ON_Press : ON_Release;
			break;

		case ON_Release:
			if (A0) {
				B = 0x01;
				State = OFF_Press;
			}
			else {
				State = ON_Release;
			}
			break;

		case OFF_Press:
			State = A0 ? OFF_Press : OFF_Release;
			break;

		default:
			break;
	}

	// no state actions
}

int main(void) {
	DDRA = 0x00; PORTA = 0xFF;
	DDRB = 0xFF; PORTB = 0x00;

	//B = 0x01;
	//A0 = PINA & 0x01; // only want last bit 
	State = Start;

	while (1) {
		A0 = PINA & 0x01; // only want last bit
		Button();
		PORTB = B;
    	}

	//PORTB = B;

    	return 0;
}
