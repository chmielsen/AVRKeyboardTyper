// ***********************************************************
// Project:
// Author:
// Module description:
// ***********************************************************

#include <avr\io.h>              // Most basic include files
#include <avr\interrupt.h>       // Add the necessary ones
#include <avr\signal.h>          // here

#define F_CPU 8000000UL /* 8 MHz */

#include <util/delay.h>

#define INSTR 0
#define DATA  1

// Interrupt handler example for INT0
//

ISR(INT0_vect) {
	static uint8_t toggle = 0;
	volatile uint16_t t;
	
	for (t = 0; t < 2000; ++t)
		;
		
	if ((PIND & (1 << PD2)) == 0)
		toggle = !toggle;
	if (toggle)
		PORTA ^= 1 << PA0;

}

ISR(INT2_vect) {
	PORTA ^= 1 << PA2;
}

// It is recommended to use this coding style to
// follow better the mixed C-assembly code in the
// Program Memory window
//
void my_function(void) {  // Put the open brace '{' here

   asm("nop");          // Inline assembly example
}

void init_lcd(void) {
	PORTD &= ~(1 << PD0);
	PORTD &= ~(1 << PD1);
	_delay_ms(40);
	// wyslij 0x30
	PORTD |= 1 << PD1;
	PORTD |= 1 << PD2;
	PORTD |= 1 << PD3;
	PORTD &= ~(1 << PD4);
	PORTD &= ~(1 << PD5);
	asm("nop");
	asm("nop");
	asm("nop");
	PORTD &= ~(1 << PD1);
	_delay_ms(5);
	//wyslij 0x30 znowu
	PORTD |= 1 << PD1;
	asm("nop");
	asm("nop");
	asm("nop");
	PORTD &= ~(1 << PD1);
	_delay_us(100);
	
	// wyslij 0x30 znowu (osatni)
	PORTD |= 1 << PD1;
	asm("nop");
	asm("nop");
	asm("nop");
	PORTD &= ~(1 << PD1);
	_delay_us(100);
	
	// wyslij 0x20
	PORTD |= 1 << PD1;
	PORTD &= ~(1 << PD2);
	asm("nop");
	asm("nop");
	asm("nop");
	PORTD &= ~(1 << PD1);
	_delay_us(40);
}

// ustaw PD0 odpowiednio przed
void sendByte(char c, uint8_t t) {
	// clear PORTD
	PORTD = 0x00;
	if (t == DATA) {
		PORTD |= 1 << PD0;
	}
	
	PORTD |= 1 << PD1;
	// older half
	if (0b00010000 & c)
		PORTD |= 1 << PD2;
	if (0b00100000 & c)
		PORTD |= 1 << PD3;
	if (0b01000000 & c)
		PORTD |= 1 << PD4;
	if (0b10000000 & c)
		PORTD |= 1 << PD5;

	
	asm("nop");asm("nop");asm("nop");
	PORTD &= ~(1 << PD1);
	asm("nop");asm("nop");asm("nop");	
	PORTD |= 1 << PD1;
	
	//younger half
	if (0b00000001 & c)
		PORTD |= 1 << PD2;
	else
		PORTD &= ~(1 << PD2);
	if (0b00000010 & c)
		PORTD |= 1 << PD3;
	else
		PORTD &= ~(1 << PD3);
	if (0b00000100 & c)
		PORTD |= 1 << PD4;
	else
	   PORTD &= ~(1 << PD4);
	if (0b00001000 & c)
		PORTD |= 1 << PD5;
	else
		PORTD &= ~(1 << PD5);
	
	asm("nop");asm("nop");asm("nop");
	
	PORTD &= ~(1 << PD1);
	_delay_us(40);
	
}

// ***********************************************************
// Main program
//
int main(void) {

	DDRD = 0xFF;
	// Utaw PD2 na wejscie, bo jest na nim przycisk
	// DDRD &= ~(1 << PD2);
	DDRA = 1 << PA0 | 1 << PA2;
	init_lcd();
	// function set ( 2-linie w wyswietlaczu)
	sendByte(0b00101000, INSTR);
	_delay_us(40);
	// display control
	sendByte(0b00001111, INSTR);
	_delay_us(50);

	//sendByte('a', DATA);
	sendByte('b', DATA);
	
	// tmp = MCUCR;
	// tmp &= ~(1 << ISC00);
	// tmp |= 1 << ISC01;
	// MCUCR = tmp;
	// MCUCSR &= ~(1 << ISC2);
	// GICR |= 1 << INT0 | 1 << INT2;
	// GIFR |= 1 << INT0 | 1 << INT2;
	// sei();
	
	for(;;)
		;
   //while(1) {             // Infinite loop; define here the
   //
   //  my_function();      // system behaviour
   //}

}




