#include "RBELib/ADC.h"
#include <avr/io.h>
#include <avr/interrupt.h>

void initADC(int channel, int free) {
  cli();
  // Only 7 ADC channel; higher values screw up ADMUX register.
  if (channel > 7) return;
  ADMUX |= (1 << REFS0) | channel;  // Select Channel, see page 248.
  ADCSRA |=
      (1 << ADEN) | (0 << ADIE) | (free << ADATE) | 4;  // Enable ADC, see page 249.
  ADCSRB = free ? 0 : ADCSRB;  // Free Running Mode, see page 251.
  ADC = 0;
  if (free) ADCSRA |= 1 << ADSC;
  sei();
  printf("Initialize ADC, ADMUX=%d, ADCSRA=%d, ADCSRB=%d\n", ADMUX, ADCSRA, ADCSRB);
}

/*
ISR(ADC_vect) {
  globalADC = ADC;
  //printf("In ISR\n");
}
*/

void clearADC(int channel) {
  ADCSRA &= ~(1 << ADEN); // Clear ADEN bit; disables ADC; see p249.
}

unsigned short getADC(int channel) {
  // Initiate conversion by setting ADSC. see p249.
  ADC = 0;
  ADCSRA |= (1 << ADSC);
  // Conversion takes order 15 cycles. (see p239).
  // Wait for ADIF to be set.
  //printf("Low bits are %d, ADCSRA=%d\n", (int)ADCL, ADCSRA);
  while ((ADCSRA & (1 << ADSC))) continue;
  //printf("Low bits are %d, ADCSRA=%d\n", (int)ADCL, ADCSRA);
  // Clear ADIF. Note that, per p250, this can interfere with interrupts.
  //ADCSRA |= 1 << ADIF;
  // See p251; ADCL contains lowest 8 bits, ADCH contains highest 2.
  // Must read ADCL before reading ADCH.
  // To right-adjust everything (and so you could just read ADCH for a mere 8
  // bits of precision), see p248 and the ADLAR bit of ADMUX.
  //printf("ADC is %d, ADCSRA=%d, ADMUX=%d\n", ADC, ADCSRA, ADMUX);
  return ADC;
}

void changeADC(int channel) {
  // Only 7 ADC channel; values 8-31 would be a comparator, higher would affect
  // other bits in ADMUX.
  if (channel > 7) return;
  ADMUX |= channel;  // Select Channel, see page 248.
}
