#include "RBELib/Periph.h"
#include "RBELib/SlaveSelects.h"
#include "RBELib/SPI.h"
#include "RBELib/constants.h"

#include <math.h>

void encCmd(unsigned char enc, unsigned char cmd, unsigned char reg,
            unsigned char *bytes, unsigned char numBytes) {
  if (enc)
    ENCODER_SS_1 = 0;
  else
    ENCODER_SS_0 = 0;

  // See http://wiki.wpi.edu/images//images/f/f1/LS7366R.pdf
  // See page 3, set IR to send the command.
  unsigned char cmdByte = (cmd << 6) | (reg << 3);
  spiTransceive(cmdByte);

  // Iterate through and send/receive each byte.
  for (int i = 0; i < numBytes; ++i) {
    *(bytes + i) = spiTransceive(*(bytes + i));
  }

  if (enc)
    ENCODER_SS_1 = 1;
  else
    ENCODER_SS_0 = 1;
}

void encInit(int chan) {
  // Clear the counter channel.
  encCmd(chan, kClr, kCNTR, NULL, 0);
  // Set the MDR0 mode bits:
  // B1,B0: count mode (0=non-quad, 1=x1, 2=x2, 3=x4).
  // B3,B2: count mode (0=free-running).
  // B5,B4: Index (0=disable).
  // B6: Asynchronous index (0; 1=synchronous).
  // B7: Filter clock divison (keep default=1).
  // Current use the x4 mode.
  char mode = 3;
  encCmd(chan, kWr, kMDR0, &mode, 1);
  // Set the MDR1 mode bits:
  // B1,B0: num bytes (0=4bytes, 1=3, 2=2, 3=1).
  // B2: Enable count (0=enable).
  // B3: not used.
  // B4, 5, 6, 7: Whether to set certain flags.
  // Only use 2 bytes, because we are never going to exceed that.
  mode = 2;
  encCmd(chan, kWr, kMDR1, &mode, 1);
}

signed long encCount(int chan) {
  char buf[2];
  encCmd(chan, kRd, kCNTR, buf, 2);
  int ret = ((int)buf[0] << 8) | (buf[1]);
  return ret;
}

int encAngle(Link link) {
  int raw = encCount(link);
  // Multiply by the appropriate scalar to convert to degrees.
  return raw * (link ? kEncToDegL : kEncToDegH);
}

void resetEncCount(int chan) {
  encCmd(chan, kClr, kCNTR, NULL, 0);
}

void setEncCount(int chan, long cnt) {
  char buf[2] = {cnt >> 8, cnt};
  encCmd(chan, kWr, kCNTR, buf, 2);
}

signed int getAccelRaw(int axis) {
  // Pull CS low.
  SPARE_SS = 0;
  // The ADC works as follows:
  // The first 1 bit is a start bit.
  // The following 4 bits are the command:
  //   first: 1 = single channel, 0 = differential.
  //   remaining 3: select channel.
  // This is followed by two don't care bit for the ADC to start conversion.
  // The following 12 bits are the data, MSB first.
  // We will package this such that the most significant byte of the data
  // is in the second byte of the transaction.
  const char cmd = ((1 << 4) | (1 << 3) | axis) << 2;
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

int IRDist(int chan) {
  // Uses an exponential curve fit to convert between the raw Sharp IR sensor
  // values and the distance in meters.
  int raw = clipi(getADC(chan), 70, 700);
  const float kCoeff = 0.822;
  const float kBase = 0.416;
  const float volts = (float)raw * 5.0 / 1023;
  const float retval = kCoeff * pow(kBase, volts);
  return retval * 1000;
}
