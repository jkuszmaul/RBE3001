#include "RBELib/DAC.h"
#include "RBELib/SPI.h"
#include "RBELib/SlaveSelects.h"
#include <util/delay.h>

void setDAC(int DACn, int SPIVal) {
  if (DACn < 0 || DACn > 4) return;
  const char cmd = 3 << 4; // Write and Update DAC register.
  // Pull DAC_SS low.
  DAC_SS = 0;
  printf("In setDAC %d\n", __LINE__);
  // Command and address byte.
  spiTransceive(cmd | DACn);
  // Most significant 8 bits.
  spiTransceive((unsigned char)(SPIVal >> 4));
  // Least significant 4 bits.
  spiTransceive((unsigned char)SPIVal << 4);
  printf("In setDAC %d\n", __LINE__);
  // Reset DAC_SS high.
  DAC_SS = 1;
}

void resetDAC() {
  // Pull DAC_SS low.
  DAC_SS = 0;
  unsigned char cmd = 4 << 4;
  // Command to turn off all DACs.
  spiTransceive(cmd | 0x0F);
  // Fill in remaining 16 bits.
  spiTransceive(0);
  spiTransceive(0);
  // Reset DAC_SS high.
  DAC_SS = 1;
  _delay_us(1); // Allow DAC_SS to be high for a time.
  // Pull DAC_SS low.
  DAC_SS = 0;
  cmd = 1 << 4;
  // Command to turn on all DACs.
  spiTransceive(cmd | 0x0F);
  // Fill in remaining 16 bits.
  spiTransceive(0);
  spiTransceive(0);
  // Reset DAC_SS high.
  DAC_SS = 1;
}
