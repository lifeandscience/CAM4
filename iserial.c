/*
 *	ISERIAL.C
 *
 *  Interrupt-driven serial output for 16Cxx mid-range PIC devices.
 *
 *	Copyright (C) 2008 Museum of Life and Science.
 *	Author: Peter Reintjes <peter.reintjes@ncmls.org>	
 *	Comments:
 *	
 *   I'm using this code to send commands to remote cameras,
 *   based upon switch inputs so there is no need for serial
 *   input, allowing for higher baud rates than usual I/O.
 *   In fact, this is so fast that we have to set the timer
 *   twice from an 8-bit timer value (twice the baud rate).
 * 
 *   It also toggles RB4 on every interrupt to for the charge
 *   pump that creates a negative voltage for RS232/422/485.
 */

#include	"camera.h"

#define TxData    RA3      /* Transmit port bit */
#define	XTAL	  4000000  /* Crystal frequency (Hz). */
#define	BRATE	  2400	   /* Baud rate. */

/*
 * At 2400 baud, adding 6 to the *exact* timer calculation put
 * the timing halfway between the rates that dropped characters.
 */

#define HALF_TIMER_VALUE     (256-(XTAL/(8*BRATE)))+6

/*
 * Previously 13 bits 2-stop lots of margin, but this works
 */

#define TRANSMIT_NUM_BITS	13  // 1 start, 8 data, 1 stop + margin

static unsigned char	sendbuffer; // Output character buffer
static unsigned char	send_bitno; 

static bit 	        tx_next_bit;
static bit              nexttime;   // transmit bit every other interrupt

/*
 * Initialize I/O, serial port timings and camera switches (pullups)
 */

void
initialize(void)
{
  // INPUT OUTPUT PIN DIRECTIONS
   // CMCON = 0x07; // turn off comparator module(for 16F627A)
   // VRCON = 0;
	TRISA = 0x10;  // RA0-1 video cntl, RA2=pulse, RA3=xmit
	TRISB = 0x3F;  // RB0-5 are inputs: RB6-7 video cntl

  // DEFAULT VIDEO SWITCHER SETTINGS
	RA0 = RA1 = RB6 = 1; // Cameras 2,3,4 OFF
	RB7 = 0;             // Camera  1 ON

  // ACTIVATE PULL-UP RESISTORS ON PORTB
	RBPU = 0;
	T0CS = 0;               // Set timer mode for Timer0
	TMR0 = HALF_TIMER_VALUE;
        nexttime = 1;           // Interrupt will skip alt cycles

	T0IE = 1;	        // Enable the Timer0 interrupt.
	GIE = 1;
}

void
putch(char c)
{
	while(send_bitno)
		continue;
	tx_next_bit = 0;
	sendbuffer = c;
	send_bitno = TRANSMIT_NUM_BITS;
}

/*
 * Serial Interrupt Service Routine serial_isr
 * Transmits characters given to putchar();
 * 
 * The Interrupt runs at twice the BRATE
 * because the timer value is only eight bits.
 */

interrupt void
serial_isr(void)
{
    TMR0 = HALF_TIMER_VALUE;  /* (256 - computed # of cycles) */

    /* Re-enable timer */
    T0IF = 0;
    /* Pulses for the negative voltage generator/charge-pump */
    RA2 = !RA2;

    if (nexttime = !nexttime)  /* interrupt is twice the baud rate */
      return;
    
    /* TRANSMIT */
    if(send_bitno) {
      TxData = tx_next_bit;
      tx_next_bit = sendbuffer & 1;
      sendbuffer = (sendbuffer >> 1) | 0x80;
      send_bitno--;
    }
}
