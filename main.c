/* --------------------------- ucDude.wordpress.com ---------------------------
 *
 * Bytebeat with MSP430F5529 and a (Piezo) Speaker on P1.2
 * ucDude, January 2014
 *
 * Modified MSP430G2553 code from NatureTM to fit MSP430F5529
 * Original Code: Music with only a speaker, a LP, and 30 lines of code
 * http://forum.43oh.com/topic/..
 * 		   ..3141-music-with-only-a-speaker-a-lp-and-30-lines-of-code/
 *
 * Bytebeat Discovered and Explained by Ville-Matias Heikkilä (Viznut)
 * Discovering novel computer music techniques by
 *         exploring the space of short computer programs
 * http://arxiv.org/abs/1112.1368
 *
 * "Songs" aka Formulas with Title and Artist info from this text file
 * http://pelulamu.net/countercomplex/music_formula_collection.txt
 *
 * --------------------------- ucDude.wordpress.com -------------------------*/


#include "msp430f5529.h"

#define MCLK						1000000
#define OUTPUT_SAMPLES_PER_SECOND   8000

#define PIN_SPEAKER					BIT2

unsigned long t = 0;

unsigned char sample;

void main(void) {
	WDTCTL = WDTPW + WDTHOLD;
	
	P1SEL |= PIN_SPEAKER;
	P1DIR |= PIN_SPEAKER;

	TA0CTL = TASSEL_2 | MC_1;
	TA0CCR0 = (0x0001 << 8) - 1;
	TA0CCTL1 |= OUTMOD_7;

	TA1CTL = TASSEL_2 | MC_1;
	TA1CCR0 = MCLK / OUTPUT_SAMPLES_PER_SECOND - 1;
	TA1CCTL0 |= CCIE;

	_enable_interrupts();

	while(1){
		// "the 42 melody", separately discovered by several people on irc etc
		sample = t*(42&t>>10);

		// blueberry 2011-10-05 http://pouet.net/topic.php?which=8357&page=12
		//sample = t*(((t>>9)^((t>>9)-1)^1)%13);

		t++;
		LPM0;
	}
}

#pragma vector = TIMER1_A0_VECTOR
__interrupt void T1A0_ISR(void)
{
	TA0CCR1 = sample;
	LPM0_EXIT;
}
