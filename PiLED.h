/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   PiLED.h
 * Author: Andrew
 *
 * Created on December 30, 2017, 2:03 PM
 */

#ifndef PILED_H
#define PILED_H

#include <stdint.h>
#include <stdio.h>


#include "FastLED/lib8tion.h"
#include "FastLED/pixeltypes.h"
#include "FastLED/hsv2rgb.h"
#include "FastLED/colorutils.h"
#include "FastLED/colorpalettes.h"
#include "FastLED/noise.h"


/// High level controller interface for PiLED.  This class manages controllers, global settings and trackings
/// such as brightness, and refresh rates, and provides access functions for driving led data to controllers
/// via the show/showColor/clear methods.
/// @nosubgrouping
class PiLED {
	uint8_t  m_Scale; 				///< The current global brightness scale setting
	struct CRGB *data;
	int nLeds;
	int spiFileDescriptor;
	
private:
	CRGB colorCorrection;
	CRGB colorTemperature;
	
	CRGB getAdjustment(uint8_t scale);
	void show(CRGB scale);
	
	static CRGB computeAdjustment(uint8_t scale, const CRGB & colorCorrection, const CRGB & colorTemperature) {
		CRGB adj(0,0,0);

		if(scale > 0) {
			for(uint8_t i = 0; i < 3; i++) {
				uint8_t cc = colorCorrection.raw[i];
				uint8_t ct = colorTemperature.raw[i];
				if(cc > 0 && ct > 0) {
					uint32_t work = (((uint32_t)cc)+1) * (((uint32_t)ct)+1) * scale;
					work /= 0x10000L;
					adj.raw[i] = work & 0xFF;
				}
			}
		}

		return adj;
	}
	
public:
	PiLED();
	PiLED(CRGB data[], int nLeds);

	/// Set the global brightness scaling
	/// @param scale a 0-255 value for how much to scale all leds before writing them out
	void setBrightness(uint8_t scale) { m_Scale = scale; }

	/// Get the current global brightness setting
	/// @returns the current global brightness value
	uint8_t getBrightness() { return m_Scale; }
	
	/// Update all our controllers with the current led colors, using the passed in brightness
	/// @param scale temporarily override the scale
	void show(uint8_t scale);

	/// Update all our controllers with the current led colors
	void show() { show(m_Scale); }

	/// clear the leds, wiping the local array of data, optionally black out the leds as well
	/// @param writeData whether or not to write out to the leds as well
	void clear(bool writeData = false);

	/// clear out the local data array
	void clearData();

	/// Set all leds to the given color/scale
	/// @param color what color to set the leds to
	/// @param scale what brightness scale to show at
	void showColor(const struct CRGB & color, uint8_t scale);

	/// Set all leds to the given color
	/// @param color what color to set the leds to
	void showColor(const struct CRGB & color) { showColor(color, m_Scale); }

	/// Set a global color temperature.
	/// @param temp A CRGB structure describing the color temperature
	void setTemperature(const struct CRGB & temp);

	/// Set a global color correction.  
	/// @param correction A CRGB structure describing the color correction.
	void setCorrection(const struct CRGB & correction);
};

#endif /* PILED_H */