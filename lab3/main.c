/*
 * main.c
 *
 * Created on: Aug 6, 2014
 * Author: ewillcox
 */
#include "RBELib/RBELib.h" //RBELib

volatile int enc0, enc1;
void isr() {
	cli();
	enc0 = encCount(0);
	enc1 = encCount(1);
	sei();
    static int p1, p2;
    int p1tmp = PINB & 0x4;
    int p2tmp = PINB & 0x2;
    int angle = 0;
    angle = 9000 * (1 - (p1tmp >> 2));
    //	printf("Set gotoangles %d\n", (p1tmp >> 2) * 90);
    if (p2tmp) angle *= -1;
	if (p1 != p1tmp || p2 != p2tmp)
		gotoAngles(angle, -angle);
    p1 = p1tmp;
    p2 = p2tmp;
    int volts = 0;
return;
    if (!p1 && !p2) volts = 6000;
    else if (!p1) volts = 3000;
    else if (!p2) volts = -3000;
    setMotorVolts(0, volts);
}

// Call various init functions.
void initAll() {
  initRBELib();
  debugUSARTInit(115200);
  initSPI();
  initADC(kPotHigh, 0, 0);
  setSPIClkDivide(kSPI4);
  DDRB &= ~0x6;
  encInit(0);
  encInit(1);
  setSPIClkDivide(kSPI128);
  resetAccelRef();
  setSPIClkDivide(kSPI4);
 // initTimer(2, CTC, 10000);
 // setTimerInterrupt(2, isr);
 // _delay_ms(5); // Don't start timer ISRs too close to one another.
  initMotors();
}

int main() {
  initAll();
  printf("HELLO.\n");
  // Initialize PID loops and start timers.
  while (1) {
#ifdef ACCEL
    // Don't interrupt SPI transmissions...
    cli();
    int x = getAccel(0), y = getAccel(1), z = getAccel(2);
    sei();
   // setSPIClkDivide(kSPI4);
    int vref = getAccelRaw(3);
#endif
   // printf("%d\n", volts);
   // printf("%d\t%d\t%d\n", x, y, z);
    // _delay_ms(5);
    while (!isrdone) continue;
    isrdone = 0;
    isr();
	int ench = encAngle(kH);
	int poth = potAngle(kPotHigh);
	int pothraw = ADC;
	int encl = encAngle(kL);
	int potl = potAngle(kPotLow);
	int potlraw = ADC;
	printf("%d, %d, %d, %d, %d, %d\n", encl, ench,
		   poth, pothraw, potl, potlraw);
  }
  return 0;
}

