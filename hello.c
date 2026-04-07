#include <8051.h>
#include <stdio.h>
#include "stc8g.h"

// Define System Clock (Usually 22.1184MHz or 11.0592MHz on dev boards)
#define MAIN_FOSC 11059200UL
#define Baudrate1 115200UL
#define DELAY 1000 // 1000 /// 500
#define LED P3_6

#define SEG_PORT P1 // Define P2 to drive segments A-DP

// Digit Selection (Common Cathode - Active Low)
#define Digit1 P3_1
#define Digit2 P3_0
#define Digit3 P3_3

// Common Cathode 7-segment code for 0-9 e=0x10, c=0x04, g=0x40, dp=0x80
unsigned char table[] = {
    0x3f, 0x06, 0x5b, 0x4f, 0x66, 0x6d, 0x7d, 0x07, 0x7f, 0x6f
};

// UART1 Initialization
void uart_init()
{
    // 1. Switch Pin to P3.0/P3.1
    P_SW1 &= ~0xC0; // (P3.0/RxD, P3.1/TxD)
    // Configure UART1
    SCON = 0x50;        // 8-bit variable UART, enable receiver
    
    // Configure Timer 2 as Baud Rate Generator
    T2L = (65536 - (MAIN_FOSC / 4 / Baudrate1)) & 0xff; // Set baud rate
    T2H = (65536 - (MAIN_FOSC / 4 / Baudrate1)) >> 8;
    AUXR |= 0x14;       // Timer 2 clock is 1T (100% speed), Enable Timer 2
    AUXR |= 0x01;       // S1_S=1: Select Timer 2 as Baudrate Generator for UART1
}

void delay(unsigned int ms) {
	unsigned int i, j;
	for (i = 0; i < ms; i++)
		for (j = 0; j < 1000; j++)
			; // Crude delay for 8051
	}

// Redirect printf to UART
int putchar(int c) {
    SBUF = c;
    while (!TI)
			;
    TI = 0;
		return c;
	}

void Display(unsigned int num) {
    unsigned char d1, d2, d3;
    
    // Split 3-digit number
    d1 = num / 100;
    d2 = (num % 100) / 10;
    d3 = num % 10;
    
    // --- Display Digit 1 ---
    P0 = num; // table[d1]; // Send segment data to P0
		P3_4 = num & 2 ? 1 : 0;
    Digit1 = 0; Digit2 = 1; Digit3 = 1; // Turn on Digit 1, off others
    delay(200);
    
    // --- Display Digit 2 ---
    P0 = num; // table[d2];
		P3_4 = num & 2 ? 1 : 0;
    Digit1 = 1; Digit2 = 0; Digit3 = 1; // Turn on Digit 2, off others
    delay(200);
    
    // --- Display Digit 3 ---
    P0 =  num; // table[d3];
		P3_4 = num & 2 ? 1 : 0;
    Digit1 = 1; Digit2 = 1; Digit3 = 0; // Turn on Digit 3, off others
    delay(200);
}

void main() {
	int bit, segment_cntr = 0;

  WDT_CONTR |= 7;  // set to longest WDT
	// Setup IO Port Mode if necessary (e.g., P3.0/P3.1 for UART1)
	P3M0 |= (1 << 4); // Set P3M0.4  set TX to push-pull for segment in P3
	P3M1 &= ~(1 << 4);
//	P3M0 = 0x00; P3M1 = 0x00; // Default P3 to Quasi-bidirectional
	P3M0 |= (1 << 6); // Set P3M0.6 for LED
	P3M1 &= ~(1 << 6); // Clear P3M1.6
    
	// Set P1 except for P1.1 to Push-Pull mode for higher LED current (strongly recommended)
	P1M0 = 0xfd;
	P1M1 = 0x02;
	uart_init();
	// Configure Ports as Quasi-Bidirectional (standard 8051)
	P0M0 = 0x00; P0M1 = 0x00;
	P3M0 = 0x00; P3M1 = 0x00;

	// Enable interrupts
	ES = 1;             // Enable UART1 interrupt
	EA = 1;             // Enable global interrupts

	bit = 1;
	while (1) {
		WDT_CONTR |= 0x10;  /* tickle WDT */
		printf("table bit = 0x%x seg = 0x%x\r\n", bit, table[segment_cntr]);
		LED = 0; // Turn LED ON (assuming active low)
		delay(DELAY);
		LED = 1; // Turn LED OFF
		delay(DELAY);
		// Main loop
		SEG_PORT = bit; // table[segment_cntr]; // Send segment code
		Display(bit); // Display number 123
		bit <<= 1;
		if (bit >= (1 << 8)) {
			delay(2000);
			Display(0xff); // Display number 123
			bit = 1;
			}
		delay(2000);
		}
	}
