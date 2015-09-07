#include "RBELib/timer.h"
#include <avr/io.h>
#include <avr/interrupt.h>

static const int prescale = k1024;
static const unsigned long kClk = CLK / 1024; // Hz

/**
 * Timer 0: 8-bit, PWM, Chapter 13 page 86.
 * Timer 1: 16-bit, PWM, Chapter 14, page 104; page 125 register descriptions.
 * Timer 2: 8-bit, PWM, Chapter 15, page 132.
 */
void initTimer(int timer, int mode, unsigned long comp) {
  // Disable interrupts until setup is done.
  cli();
  // Because comp is in microseconds, we must divide by a net of 1000000, while
  // trying to avoid allowing overflow in the multiplication or excessive
  // truncation in the division.
  comp = ((kClk / 1000) * comp) / 1000 - 1;
  // The only differences between cases is the numbers in the register names.
  switch (timer) {
    case 0:
      // Set timer to CTC mode as appropriate, p97.
      TCCR0A = mode << COM0A0 | mode << WGM00;
      // Set pre-scaling to CLKI/O / 1024. p101.
      TCCR0B = prescale << CS00;
      // Set comparison.
      OCR0A = comp;
      // Enable Interrupt on COMPA.
      TIMSK0 |= 1 << OCIE0A;
      break;
    case 1:
      // Set timer to toggle OC1A on compare match, p125.
      TCCR1A = mode << COM1A0;
      // Set pre-scaling to CLKI/O / 1024. p128.
      // Also, set mode (WGM1) to CTC using OCR1A. p127.
      TCCR1B = prescale << CS10 | 1 << WGM12;
      // Set comparison.
      OCR1A = comp;
      // Enable Interrupt on COMPA.
      TIMSK1 |= 1 << OCIE1A;
      break;
    case 2:
      // Set timer to CTC mode as appropriate, p146.
      TCCR2A = mode << COM2A0 | mode << WGM20;
      // Set pre-scaling to CLKI/O / 1024. p150.
      TCCR2B = prescale << CS20;
      // Set comparison.
      OCR2A = comp;
      // Enable Interrupt on COMPA.
      TIMSK2 |= 1 << OCIE2A;
      break;
    default:
      // Fall through.
      break;
  }

  // Re-enable interrupts.
  sei();
}

unsigned getTimer(int timer) {
  switch (timer) {
    case 0:
      return TCNT0; // p101.
      break;
    case 1:
      return TCNT1; // p129.
      break;
    case 2:
      return TCNT2; // p150.
      break;
    default:
      return 0;
      break;
  }
}

void initPWM(unsigned char timer) {
  // Set to toggle OCnA on match and set WGMn to Phase Correct PWM Mode with
  // OCRA as TOP.
  // WGM is stored in both TCCRnA and TCCRnB.
  switch (timer) {
    case 0:
      OC0B_DDR |= 1 << OC0B_BIT; // See page 75.
      // See pages 97-101.
      TCCR0A = (1 << COM0A0) | (1 << COM0B1) | (1 << WGM00);
      TCCR0B = (1 << WGM02) | (prescale << CS00);
      break;
    case 1:
      OC1B_DDR |= 1 << OC1B_BIT; // See page 80.
      // See pages 125-128.
      TCCR1A = (1 << COM1A1) | (1 << COM1B1) | (3 << WGM10);
      TCCR1B = (1 << WGM13) | (prescale << CS10);
      break;
    case 2:
      OC2B_DDR |= 1 << OC2B_BIT; // See page 80.
      // See pages 146-150.
      TCCR2A = (1 << COM2A0) | (1 << COM2B1) | (1 << WGM20);
      TCCR2B = (1 << WGM22) | (prescale << CS20);
      break;
  }
}

void setPWM(unsigned char timer, unsigned int top, unsigned int comp) {
  switch (timer) {
    case 0:
      OCR0A = top;
      OCR0B = comp;
      break;
    case 1:
      OCR1A = top;
      OCR1B = comp;
      break;
    case 2:
      OCR2A = top;
      OCR2B = comp;
      break;
    default:
      // Fall through.
      break;
  }
}

void setCompValue(unsigned char timer, unsigned long comp) {
  comp = ((kClk / 1000) * comp) / 1000 - 1;
  // Set comparison register appropriately.
  switch (timer) {
    case 0:
      OCR0A = comp;
      break;
    case 1:
      OCR1A = comp;
      break;
    case 2:
      OCR2A = comp;
      break;
    default:
      // Fall through.
      break;
  }
}

static Callback timer0 = 0, timer1 = 0, timer2 = 0;

void setTimerInterrupt(unsigned char timer, Callback callback) {
  // Set appropriate interrupt vector, depending on the timer in question.
  switch (timer) {
    case 0:
      timer0 = callback;
      break;
    case 1:
      timer1 = callback;
      break;
    case 2:
      timer2 = callback;
      break;
    default:
      // Fall through.
      break;
  }
}

// Declare the ISRs that the hardware will actually call for each comparison
// vector. Within each ISR, we check to see if a callback has been set and then
// call it if it has.
ISR(TIMER0_COMPA_vect) {
  if (timer0) timer0();
}

ISR(TIMER1_COMPA_vect) {
  if (timer1) timer1();
}

ISR(TIMER2_COMPA_vect) {
  if (timer2) timer2();
}
