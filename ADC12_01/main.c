#include <msp430fr5969.h>

// Functions prototyping
void ADC_init();

void ADC_init()
{
	//**** Select the Sample and Hold value ****//
	ADC12CTL0 = ADC12SHT0_2 | ADC12ON;
	/*
	 * ADC12ON : Enable the ADC
	 *
	 * ADC12SH0_0 : sample and hold 4 ADC clock cycle
	 * ADC12SH0_1 : sample and hold 8 ADC clock cycle
	 * ADC12SH0_2 : sample and hold 16 ADC clock cycle
	 * ADC12SH0_3 : sample and hold 32 ADC clock cycle
	 * ....
	 * ADC12SH0_15 : sample and hold 512 ADC clock cycle
	 */

	//**** Select the Source clock ****//
	ADC12CTL1 = ADC12SHP;
	/*
	 * ADC12_B sample-and-hold pulse-mode select.
	 * Source clock is sample timer
	 */

	//**** Select the resolution ****//
	ADC12CTL2 |= ADC12RES_2;
	/*
	 * ADC12_B resolution of 12 bit conversion.
	 * 00b = 8 bit (10 clock cycle conversion time)
	 * 01b = 10 bit (12 clock cycle conversion time)
	 * 10b = 12 bit (14 clock cycle conversion time)
	 */

	//**** Enable interrupts for specific channels ****//
	ADC12IER0 |= ADC12IE0;
	/*
	 * Enables or disables the interrupt request for ADC12IFG0 bit.
	 * 0b = Interrupt disabled
	 * 1b = Interrupt enabled
	 *
	 * ADC12IE0               (0x0001)     ADC12 Memory 0 Interrupt Enable.
	 * ADC12IE1               (0x0002)     ADC12 Memory 1 Interrupt Enable.
	 * ...
	 * ADC12IE15              (0x8000)     ADC12 Memory 15 Interrupt Enable
	 */

	//**** Select the references ****//
	ADC12MCTL0 |= ADC12INCH_1 | ADC12VRSEL_1;
	/*
	 * Select        A1 (P1.1)      Vref = 1.2V
	 *
	 * ADC12INCH_4: Input channel select.
	 * If ADC12DIF = 0: A4;
	 * If ADC12DIF = 1: Ain+ = A4, Ain- = A5
	 *
	 * ADC12VRSEL_x : Selects combinations of VR+ and VR- sources as well as the buffer selection.
	 * 0000b = VR+ = AVCC, VR- = AVSS
	 * 0001b = VR+ = VREF buffered, VR- = AVSS 0010b = VR+ = VeREF-, VR- = AVSS
	 * 0011b = VR+ = VeREF+ buffered, VR- = AVSS
	 * ...
	 */
}


int main(void) {
    WDTCTL = WDTPW | WDTHOLD;	// Stop watchdog timer

    // Configure P1.1 for the ADC
    // The Temp Sensor pin from the BoosterPack
    P1SEL1 |= BIT1;
    P1SEL0 |= BIT1;

    P1DIR |= 0x01;


    ADC_init();

    while(1){
		ADC12CTL0 |= ADC12ENC | ADC12SC; // Enabling and start conversion
	
		__bis_SR_register(LPM0_bits +GIE);
		// The__bis_SR_register() command enables the low power mode in the status register after returning
		// from the ADC interrupt routine, and enables the general interrupt to wake up the MSP430 upon another
		// ADC sampling.
		__no_operation();
    }
	return 0;
}

#pragma vector = ADC12_VECTOR
__interrupt void ADC12_ISR(void)
{
	  switch (__even_in_range(ADC12IV, ADC12IV_ADC12RDYIFG))
	  {
		case ADC12IV_ADC12IFG0:                 // Vector 12:  ADC12MEM0 Interrupt
		  if (ADC12MEM0 > 0x323)               // ADC12MEM = A1 > 0.5V?
			P1OUT |= BIT0;                      // P1.0 = 1
		  else
			P1OUT &= ~BIT0;                     // P1.0 = 0
		  __bic_SR_register_on_exit(LPM0_bits); // Exit active CPU
		  break;
		 default: break;
	  }
}
