#include "PiLED.h"

#include <unistd.h>
#include <cerrno>
#include <wiringPiSPI.h>

PiLED::PiLED() {
	m_Scale = 255;
	colorCorrection = CRGB::White;
	colorTemperature = CRGB::White;
	
	spiFileDescriptor = wiringPiSPISetup(0, 8000000); //Channel 0, 8MHz clock
	if (spiFileDescriptor < 0) {
		fprintf(stderr, "SPI Setup failed: %s\n", strerror(errno));
	}
}

PiLED::PiLED(struct CRGB *data, int nLeds) : PiLED() {
	this->data = data;
	this->nLeds = nLeds;
}

void PiLED::show(uint8_t brightness) {
	//show(CRGB(50,50,50));
	show(getAdjustment(brightness));
}

//void PiLED::showColor(const struct CRGB & color, uint8_t brightness) {
//	for (int i=0; i<nLeds; i++) {
//		int idx = 4*(i+1);
//		buf[idx]	 = 255;
//		buf[idx + 1] = color.b;
//		buf[idx + 2] = color.g;
//		buf[idx + 3] = color.r;
//	}
//	
//	writeBufferToStrand();
//}

void PiLED::clear(bool writeData) {
	if (writeData) {
		//showColor(CRGB(0,0,0), 0);
	}
    clearData();
}

void PiLED::clearData() {
	if (data) {
		memset8((void*)data, 0, sizeof(struct CRGB) * nLeds);
	}
}

void PiLED::setTemperature(const struct CRGB & temp) {
	colorTemperature = temp;
}

void PiLED::setCorrection(const struct CRGB & correction) {
	colorCorrection = correction;
}

void PiLED::show(CRGB scale) {
	write(spiFileDescriptor, (const uint8_t[]){0, 0, 0, 0}, 4); //start boundary
	if (data) {
		uint8_t ledBuffer[4];

		for (int i=0; i<nLeds; i++) {
			ledBuffer[0]	 = 255;
			ledBuffer[1] = scale8(data[i].b, scale.b);
			ledBuffer[2] = scale8(data[i].g, scale.g);
			ledBuffer[3] = scale8(data[i].r, scale.r);
//			ledBuffer[1] = data[i].b;
//			ledBuffer[2] = data[i].g;
//			ledBuffer[3] = data[i].r;

			write(spiFileDescriptor, ledBuffer, 4);
		}
	}
}

/// Get the combined brightness/color adjustment
CRGB PiLED::getAdjustment(uint8_t scale) {
	return computeAdjustment(scale, colorCorrection, colorTemperature);
}