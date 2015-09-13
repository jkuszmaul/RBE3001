#include "RBELib/SPI.h"
#include <avr/io.h>
#include "RBELib/SlaveSelects.h"

void initSPI() {
  // For examples, see p157.
  // Set MOSI and SCK output, all other input.
  DDRB |= (1 << 5) | (1 << 7) | (1 << 4);
  // Enable SPI, MAster, set clock rate at fck/16. See p161.
  SPCR = (1 << SPE) | (1 << MSTR) | (3 << SPR0);

  // Go through and enable/set every SS line to high.
  ENCODER_SS_0_ddr = 1;
  ENCODER_SS_1_ddr = 1;
  ENCODER_SS_2_ddr = 1;
  ENCODER_SS_3_ddr = 1;
  DAC_SS_ddr = 1;
  AUX_DAC_SS_ddr = 1;
  ENCODER_SS_0 = 1;
  ENCODER_SS_1 = 1;
  ENCODER_SS_2 = 1;
  ENCODER_SS_3 = 1;
  DAC_SS = 1;
  AUX_DAC_SS = 1;
}

unsigned char spiTransceive(unsigned char data) {
  // For examples, see p158.
  // Start transmission. See p163.
  SPDR = data;

  // Wait for transmission complete. See p162.
  while (!(SPSR & (1 << SPIF))) continue;

  return SPDR;
}
