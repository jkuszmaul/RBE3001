#include "RBELib/DAC.h"
#include "RBELib/SPI.h"
#include "RBELib/SlaveSelects.h"

void setDAC(int DACn, int SPIVal) {
  if (DACn < 0 || DACn > 4) return;
  const char cmd = 3 << 4; // Write and Update DAC register.
  // Pull DAC_SS low.
  DAC_SS = 0;
  // Command and address byte.
  spiTransceive(cmd | DACn);
  // Most significant 8 bits.
  spiTransceive((unsigned char)(SPIVal >> 4));
  // Least significant 4 bits.
  spiTransceive((unsigned char)SPIVal << 4);
  // Reset DAC_SS high.
  DAC_SS = 1;
}
