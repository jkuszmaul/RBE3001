#include "RBELib/ports.h"
#include <stdarg.h>
#include <avr/io.h>

void setPinsDir(char port, int dir, char numPins, ...) {
  va_list valist;
  // Init valist for number of arguments.
  va_start(valist, numPins);

  unsigned char mask = 0;
  // Go through each pin and set its direction appropriately.
  for (int i = 0; i < numPins; ++i) {
    int pin = va_arg(valist, int);
    mask |= 1 << pin;
  }

  va_end(valist);

  switch (port) {
    case 'A':
      if (dir) DDRA &= ~mask;
      else DDRA |= mask;
      break;
    case 'B':
      if (dir) DDRB &= ~mask;
      else DDRB |= mask;
      break;
    case 'C':
      if (dir) DDRC &= ~mask;
      else DDRC |= mask;
      break;
    case 'D':
      if (dir) DDRD &= ~mask;
      else DDRD |= mask;
      break;
  }
}

unsigned char getPinsVal(char port, char numPins, ...) {
  va_list valist;
  // Init valist for number of arguments.
  va_start(valist, numPins);

  unsigned char mask = 0;
  // Go through each pin and set its direction appropriately.
  for (int i = 0; i < numPins; ++i) {
    int pin = va_arg(valist, int);
    mask |= 1 << pin;
  }

  va_end(valist);

  switch (port) {
    case 'A':
      return PINA & mask;
    case 'B':
      return PINB & mask;
    case 'C':
      return PINC & mask;
  }
  return 0;
}

void setPinsVal(char port, int val, char numPins, ...) {
  va_list valist;
  // Init valist for number of arguments.
  va_start(valist, numPins);

  unsigned char mask = 0;
  // Go through each pin and set its direction appropriately.
  for (int i = 0; i < numPins; ++i) {
    int pin = va_arg(valist, int);
    mask |= 1 << pin;
  }

  va_end(valist);

  switch (port) {
    case 'A':
      if (val) PORTA &= ~mask;
      else PORTA |= mask;
      break;
    case 'B':
      if (val) PORTB &= ~mask;
      else PORTB |= mask;
      break;
    case 'C':
      if (val) PORTC &= ~mask;
      else PORTC |= mask;
      break;
    case 'D':
      if (val) PORTD &= ~mask;
      else PORTD |= mask;
      break;
  }
}
