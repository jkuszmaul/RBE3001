#include "RBELib/Periph.h"
#include "RBELib/SlaveSelects.h"

void encCmd(unsigned char enc, unsigned char cmd, unsigned char reg,
            unsigned char *bytes, unsigned char numBytes) {
  if (enc)
    ENCODER_SS_1 = 0;
  else
    ENCODER_SS_0 = 0;

  // See http://wiki.wpi.edu/images//images/f/f1/LS7366R.pdf
  // See page 3, set IR.
  unsigned char cmdByte = (cmd << 6) | (reg << 3);
  spiTransceive(cmdByte);

  for (int i = 0; i < numBytes; ++i) {
    *(bytes + i) = spiTransceive(*(bytes + i));
  }

  if (enc)
    ENCODER_SS_1 = 1;
  else
    ENCODER_SS_0 = 1;
}

void encInit(int chan) {
  encCmd(chan, kClr, kCNTR, NULL, 0);
  char mode = 3;
  encCmd(chan, kWr, kMDR0, &mode, 1);
  mode = 0;
  encCmd(chan, kWr, kMDR1, &mode, 1);
}

signed long encCount(int chan) {
  char buf[4];
  encCmd(chan, kRd, kCNTR, buf, 4);
  long ret = ((long)buf[0] << 24) | ((long)buf[1] << 16) | ((long)buf[2] << 8) |
             (buf[3]);
  return ret;
}

void resetEncCount(int chan) {
  encCmd(chan, kClr, kCNTR, NULL, 0);
}

void setEncCount(int chan, long cnt) {
  char buf[4] = {cnt >> 24, cnt >> 16, cnt >> 8, cnt};
  encCmd(chan, kWr, kCNTR, buf, 4);
}

signed int getAccelRaw(int axis) {
  // Pull CS low.
  SPARE_SS = 0;
  // The ADC works as follows:
  // The first 1 bit is a start bit.
  // The following 4 bits are the command:
  //   first: 1 = single channel, 0 = differential.
  //   remaining 3: select channel.
  // This is followed by one don't care bit for the ADC to start conversion.
  // The following 12 bits are the data, MSB first.
  // We will package this such that the most significant byte of the data
  // is in the second byte of the transaction.
  const char cmd = ((1 << 5) | (1 << 4) | (axis << 1)) << 1;
  spiTransceive(cmd);
  // Remaining bytes sent don't matter.
  unsigned char high = spiTransceive(0);
  unsigned char low = spiTransceive(0);
  SPARE_SS = 1;
  int retval = ((int)high << 4) | ((low >> 4) & 0x0F);
  return retval;
}

static volatile int vref = 0;
void resetAccelRef() {
  vref = getAccelRaw(3);
}

signed int getAccel(int axis) {
  int rawaxis = getAccelRaw(axis);
  float mgs = (rawaxis - vref) * 2.2;
  return mgs;
}
