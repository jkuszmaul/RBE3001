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
	int raw = getADC(4); /*0 - 1023*/
	float kP = (float)raw * 8000.0 / 2.0 / 1023.0;
	//printf("P: %f ", kP);
	// Map into a range from -8000/-90 to +8000/90.
	return kP;
}

float getI() {
	int raw = getADC(5);
	float kI = (float)raw * 0.1;
	return kI;
}

float getD() {
	int raw = getADC(6);
	float kD = (float)raw * 0.0001;
	return kD;
}

static volatile int setpoint, err;
static volatile long total;
void runPID() {
  // Consider converting in some way to using interrupts or speeding up the ADC.
  int pos = potAngle(ain);
  setpoint = 30 * ((PINB & 0x06) >> 1);
  err = setpoint - pos;
  total = calcPID(kH, setpoint, pos) + calcFF(kH, setpoint, pos);
  if (total > kVMotor) total = kVMotor;
  if (total < -kVMotor) total = -kVMotor;
   //printf("%d\n", getCurrent(0));
  setMotorVolts(kH, -total); // TODO: Document invertedness of motors.
  PORTB ^= 1; // Toggle PORTB pin 0 for debugging.
  setConst(kH, getP(), getI(), getD());
	printf("s %d, p %d,m %ld, c %d, P %3.0f, I %1.1f, D %0.3f\n", setpoint, pos,
	       total, getCurrent(0), pidConsts.Kp_H, pidConsts.Ki_H, pidConsts.Kd_H);
}

void initPID(int pos) {
  initTimer(0, CTC, 10000);
  DDRB |= 0x01;
  DDRB &= ~(0x06);
  setpoint = pos;
  setTimerInterrupt(0, runPID);
}

void printADC() {
	int potangle = potAngle(2);
	printf("%d ,%d, %d\n", getADC(2), potangle, potVolts(2));
}

void printCur() {
	for (int i = -kVMotor; i < kVMotor; i+=10) {
		setMotorVolts(0, i);
		printf("%d, %d\n", i, getCurrent(0));
	}
}

void initAll() {
	initRBELib();
	debugUSARTInit(115200);
	initSPI();
	initADC(ain, 1, 0);
}

int main() {
	initAll();
	initPID(0);
	while (1) {
		//printf("\n");
	    //printCur();
		//triangleDAC();
		//printADC()
	}
	return 0;
}

