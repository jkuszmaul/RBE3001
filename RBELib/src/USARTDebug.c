#include "RBELib/USARTDebug.h"
#include "RBELib/RBELib.h"

void debugUSARTInit(unsigned long baudrate) {
  // Assumes USART1 and Asynchronous mode, with U2X1 = 0.
  // See page 167 for formulas for other modes.
  UBRR1 = CLK / (16 * baudrate) - 1;
  // Enable Rx/Tx.
  UCSR1B = (1 << RXEN1) | (1 << TXEN1);
  // Frame format, see page 184
  // Bits 7:6: Asynchronous=0/Synchronous/Master SPI.
  // Bits 5:4: Parity (0=disabled)
  // Bit  3:   Stop Bit (0=1 bit, 1=2 bits)
  // Bits 2:1: Character Size (11=8-bit).
  // Bit  0:   Clock Polarity.
  UCSR1C = (1 << UCSZ11) | (1 << UCSZ10);
}

void putCharDebug(char byteToSend) {
  //Wait on UDRE0 bit (see page 182).
  while(!(UCSR1A & 1 << UDRE0));
  //Fill the buffer
  UDR1 = byteToSend;
}

unsigned char getCharDebug() {
  unsigned char res;
  // Wait on nRXC1 bit to signal; page 182.
  while (!(UCSR1A & (1 << RXC1))) continue;
  // Used for 9-bit characters; unused for us.
  //resh = UCSR1B;
  // Filter the 9th bit
  // resh = ((resh >> 1) & 0x01) << 8;
  res = UDR1;
  // Ignore Error handling for now.
  //status = UCSR1A;
  /* If error, return -1 */
  //if ( status & (1<<FE1)|(1<<DOR1)|(1<<UPE1) )
  //  return -1;

  return res;
}
