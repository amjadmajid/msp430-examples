/*
This example sets Timer0_A0 in compare mode.
The TA0CCR0 CCIFG flag has the highest Timer_A interrupt priority and has a dedicated interrupt vector
 */


#include <msp430fr5969.h>

int main(void)
{
  WDTCTL = WDTPW | WDTHOLD;                 // Stop WDT

  // Configure GPIO
  P1DIR |= BIT0;
  P1OUT |= BIT0;

  // Disable the GPIO power-on default high-impedance mode to activate
  // previously configured port settings
  PM5CTL0 &= ~LOCKLPM5;


  TA0CCTL0 = CCIE;                          // TACCR0 interrupt enabled
  	  	  	  	  	  	  	  	  	  	   // CAP=0: Timer0_A is used in the compare mode
/*
 TA0CCTL0 : Timer0_A Capture/Compare Control n Register
 [15-14 CM | 13-12 CCIS | 11 SCS | 10 SCCI | 9 Res. | 8 CAP | 7-5 OUTMOD | 4 CCIE | 3 CCI | 2 OUT | 1 COV | 0 CCIFG ]

 M: Capture mode
 CCIS: Capture/compare input select.
 SCS: Synchronize capture source. This bit is used to synchronize the capture input signal with the timer clock.
 SCCI: Synchronized capture/compare input.
 CAP: Capture mode
 OUTMOD: Output Mode
 CCIE: Capture/compare interrupt enable.
 CCI: Capture/compare input. The selected input signal can be read by this bit.
 OUT: Output
 COV: Capture overflow.
 CCIFG: Capture/compare interrupt flag.
 */

  // Timer_A Capture/Compare n Register
  TA0CCR0 = 50000;     						// The value to compare Timer0_A against

  TA0CTL = TASSEL__SMCLK | MC__CONTINOUS;   // SMCLK, continuous mode

  __bis_SR_register(LPM0_bits + GIE);       // Enter LPM0 w/ interrupt
  __no_operation();                         // For debugger
}

//  All CCIFG flags request an interrupt when their corresponding CCIE bit and the GIE bit are set.
#pragma vector = TIMER0_A0_VECTOR
__interrupt void Timer0_A0_ISR (void)
{
  P1OUT ^= BIT0;
  TA0CCR0 += 50000;                         // Add Offset to TA0CCR0
}
