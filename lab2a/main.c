/*
 * main.c
 *
 * Created on: Aug 6, 2014
 * Author: ewillcox
 */
#include "RBELib/RBELib.h" //RBELib

void initAll() {
	initRBELib();
	debugUSARTInit(115200);
	initSPI();
}

void triangleDAC() {
	for (int i = 0; i < 4096; ++i) {
		setDAC(0, i);
		setDAC(1, 4096 - i);
	}
	for (int i = 4095; i >= 0; --i) {
		setDAC(0, i);
		setDAC(1, 4096 - i);
	}
}


int main() {
	initAll();
	initADC(2, 1, 0);
	while (1) {
		triangleDAC();
		continue;
		int potangle = potAngle(2);
		printf("%d ,%d, %d\n", getADC(2), potangle, potVolts(2));
	}
	return 0;
}

