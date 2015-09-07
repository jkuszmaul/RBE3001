/*
 * main.c
 *
 * Created on: Aug 6, 2014
 * Author: ewillcox
 */
#include "RBELib/RBELib.h" //RBELib

/**
 * Log ADC values continuously with no strict timings.
 * Used for part 2 of the lab.
 */
void logADC() {
	const int timer = 1;
	const int analog = 4;
	DDRA &= ~(1 << analog);
	// Set timer to use for timestamping; just continuously count up.
	initTimer(timer, NORMAL, 0);
	// Initialize ADC for free-running mode without interrupts.
	initADC(analog, 1, 0);
	while (1) {
		printf("%u %u\n", getTimer(timer), ADC);
		// Brief delay to avoid excessive spamming of debugging output.
		for (volatile int i = 0; i < 10000; ++i) continue;
	}
}

/**
 * Runs
 */
void PWM() {
	DDRB = 0x20; // Set all portb as input except for a single pin for debugging.
	unsigned long period = 10000; // in microseconds.
	const unsigned char pwmchannel = 1;
	const unsigned char kSwitch0 = 1, kSwitch1 = 1 << 1, kSwitch2 = 1 << 2;
	initPWM(pwmchannel);
	// Initialize ADC for free-running mode without interrupts.
	initADC(4, 1, 0);
	while (1) {
		if (PINB & kSwitch0) period = 1000000UL; // 1 million us => 1 Hz.
		else if (PINB & kSwitch1) period = 50000UL; // 50000 us => 20 Hz.
		else if (PINB & kSwitch2) period = 10000UL; // 10000 us => 100 Hz.
		// Calculate raw period value by converting us to sec (by dividing by 1000
		// twice) and multiplying by the prescaled clk frequency (CLK / 2048).
		// In order to minimuze loss of precision due to truncation or
		// multiplication overflow, spread out multiplies/divides.
		int periodraw = period / 1000 * (CLK / 2048000UL);
		// Scale the raw ADC value to range from 0 - periodraw.
		unsigned adc = (float)ADC / 1023.0 * periodraw;
		// Print out useful debugging info. Note that, for no apparent reason, the
		// third printed value (first ADC print) always prints a 0, no matter what.
		// TODO: Debug.
		printf("%f %uHz %u %u\n", (float)adc / periodraw, 1000000UL / period, ADC, ADC);
		setPWM(pwmchannel, periodraw, adc);
	}
}

// Use these two to signal back and forth between ISR and main loop.
// printADC access shouldn't be an issue because the ISR is only reading for 0/1
// and the writes should (a) be atomic and (b) even if read in the middle of a
// write, it doesn't really matter.
// In theory, accesses to count could cause similar minor issues (especially if
// int accesses aren't atomic), but nothing of significance and we didn't notice
// any issues.
volatile static char printADC = 0;
volatile static int count = 0;
void ledFlopAndLog() {
	PORTB ^= 0x20;
	++count;
	// At faster rates, creating a separate buffer and sending output to the
	// USART would be necessary. However, even at the 225Hz in the lab, there was no
	// discernable change in the oscilliscope readings of appropriate pin on PORTB.
	if (printADC) printf("%d %d\n", count, ADC);
}

void runSampler() {
	initADC(4, 1, 0);
	unsigned char timer = 1;
	initTimer(timer, CTC, 4444UL/*225Hz*/);
	setTimerInterrupt(timer, ledFlopAndLog);
	DDRB = 0x20;
	while (1) {
		while (!(PINB & 1)) continue;
		printf("Starting new capture.\n");
		printADC = 1;
		count = 0;
		// Precise timing of how long we sample is unimportant; just run for roughly
		// a second.
		_delay_ms(1000);
		printADC = 0;
	}
}

void testSingleADC() {
	initADC(4, 1, 1);
	printADC = 1;
	setADCInterrupt(ledFlopAndLog);
}

int main() {
	initRBELib(); //Setup printf() and setServo()
	debugUSARTInit(115200); //Initialize UART
	printf("Enter any character to continue...\n");
	getCharDebug();
//	testSingleADC();
	runSampler();
	//PWM();
	logADC();
	return 0;
}

