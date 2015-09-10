#include "RBELib/ADC.h"
#include <avr/io.h>
#include <avr/interrupt.h>

void initADC(int channel, int free, int interrupts) {
  // Turn off interrupts while we mess around with initializations.
  cli();
  // Only 7 ADC channel; higher values screw up ADMUX register.
  if (channel > 7) return;
  // Set the appropriate channel to input.
  DDRA &= ~(1 << channel);

  // Select channel and set REFS to use AVCC with capacitor at AREF (this will
  // give us 0 - Vcc). See p248 and board pinout diagram.
  // Because of the equals, we also implicitly set ADLAR to zero, forcing a
  // right-adjusted ADC result. If we set ADLAR, then we can just read the 8-bit
  // ADCH (without reading the whole ADC) to get a result because it will be
  // left adjusted.
  ADMUX = (1 << REFS0) | channel;  // Select Channel, see page 248.

  // Enable ADC, turn interrupts on/off, turn automatic triggering on/off (for
  // free running mode), and set the prescaler. See p249.
  // The prescaler options are: 0 or 1=2, 2-4, 3=8, 4=16, 5=32, 6=64, 7=128.
  // The ADC requires an input clock between 50 and 200 kHz for full 10-bit
  // resulotions (see p236). For our 18 MHz clock, this suggests that we should
  // use either the 64 or 128 times prescaling.
  ADCSRA |= (1 << ADEN) | (interrupts << ADIE) | (free << ADATE) | 7;

  // Set to free-running mode. Note that this only has meaning if the ADATE bit
  // in ADCSRA is set. See p252.
  ADCSRB = free ? 0 : ADCSRB;

  // If in free-running mode, initiate the first conversion.
  if (free) ADCSRA |= 1 << ADSC;

  // Re-enable interrupts.
  sei();
}

void clearADC(int channel) {
  ADCSRA &= ~(1 << ADEN); // Clear ADEN bit; disables ADC; see p249.
  ADC = 0; // Clear the ADC result register.
}

unsigned short getADC(int channel) {
  // Turn off automatic triggers and set to the appropriate channel. p250.
  ADCSRA &= ~(1 << ADATE);
  // Set channel appropriately.
  changeADC(channel);
  // Initiate conversion by setting ADSC. see p249.
  ADCSRA |= (1 << ADSC);
  // Conversion takes ~15 ADC clock cycles. (see p239).
  // Wait for ADSC to be cleared. p250.
  while ((ADCSRA & (1 << ADSC))) continue;

  // Return the actual ADC value. p251.
  // This is equivalent to ADCL | (ADCH << 8).
  return ADC;
}

unsigned short getADCFree() {
  return ADC;
}

void changeADC(int channel) {
  // Only 7 ADC channel; values 8-31 would be a comparator, higher would affect
  // other bits in ADMUX.
  if (channel > 7) return;
  // Clear channel bits to 0. p248.
  ADMUX &= ~31;
  ADMUX |= channel;  // Select Channel, see page 248.
}

/**
 * The callback to call from the ADC ISR.
 */
static Callback g_ADCCallback = 0;

void setADCInterrupt(Callback callback) {
  g_ADCCallback = callback;
}

ISR(ADC_vect) {
  if (g_ADCCallback) g_ADCCallback();
}
