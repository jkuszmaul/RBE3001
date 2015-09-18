/*
 * main.c
 *
 * Created on: Aug 6, 2014
 * Author: ewillcox
 */
#include "RBELib/RBELib.h" //RBELib

static const int ain = 2;


void triangleDAC() {
	for (int i = 0; i < 4096; i+=16) {
		setDAC(0, i);
		setDAC(1, 4095 - i);
	}
	for (int i = 4095; i >= 0; i-=16) {
		setDAC(0, i);
		setDAC(1, 4096 - i);
	}
}


float getP() {
	return 780;
	int raw = getADC(4); /*0 - 1023*/
	float kP = (float)raw * 8000.0 / 2.0 / 1023.0;
	//printf("P: %f ", kP);
	// Map into a range from -8000/-90 to +8000/90.
	return kP;
}

float getI() {
	return 5.1;
	int raw = getADC(5);
	float kI = (float)raw * 0.1;
	return kI;
}

float getD() {
	return 0.06;
	int raw = getADC(6);
	float kD = (float)raw * 0.0001;
	return kD;
}

static volatile int setpointl, setpointh, err;
static volatile long totall, totalh;
void runPID() {
  // Consider converting in some way to using interrupts or speeding up the ADC.
  int posl = potAngle(3);
  int posh = potAngle(2);
  setpointl = 30 * ((PINB & 0x06) >> 1);
  setpointh = setpointl;
  err = setpointl - posl;
  totall = calcPID(kL, setpointl, posl);// + calcFF(kH, setpoint, pos);
  totalh = -calcPID(kH, setpointh, posh);
  totall = clipi(totall, -kVMotor, kVMotor);
  totalh = clipi(totalh, -kVMotor, kVMotor);
   //printf("%d\n", getCurrent(0));
  //setMotorVolts(kH, -total);
	printf("%d %d\n", __LINE__, totall);
//  setMotorVolts(kL, totall); // TODO: Document invertedness of motors.
	printf("%d\n", __LINE__);
  setMotorVolts(kH, totalh);
	printf("%d\n", __LINE__);
//  PORTB ^= 1; // Toggle PORTB pin 0 for debugging.
	printf("s %d, p %d,m %ld, c %d\n", setpointh, posh,
	       totalh, getCurrent(0));
}

void initPID(int pos) {
  setConst(kL, getP(), getI(), getD());
  setConst(kH, getP(), getI(), getD());
  initTimer(0, CTC, 10000);
  DDRB = 0;//|= 0x01;
  //DDRB &= ~(0x06);
  setpointh = pos;
  setpointl = pos;
  setTimerInterrupt(0, runPID);
}

void printADC() {
	int potangle1 = potAngle(3);
	int potangle2 = potAngle(2);
	Coord pos = forward(potangle1, potangle2);
	printf("%d %d %2.2f %2.2f\n", potangle1, potangle2, pos.x, pos.y);
}

void printCur() {
	for (int i = -kVMotor; i < kVMotor; i+=10) {
		setMotorVolts(0, i);
	//	_delay_us(100);
	//	printf("%d, %d\n", i, getCurrent(0));
	}
}

void initAll() {
	initRBELib();
	debugUSARTInit(115200);
	initSPI();
	initADC(ain, 1, 0);
	//resetDAC();
}

int main() {
	initAll();
	printf("HELLO.\n");
	initPID(0);
	while (1) {
		//printf("\n");
	    //printCur();
		//triangleDAC();
		//printADC();
		//_delay_ms(10);
	}
	return 0;
}

