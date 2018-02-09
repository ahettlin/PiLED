#include <wiringPi.h>
#include "PiLED.h"

#define BRIGHTNESS         255
#define NUM_LEDS    300
CRGB leds[NUM_LEDS];


PiLED piLED;

/*
CRGB clr1;
CRGB clr2;
uint8_t speed;
uint16_t loc1;


void blendwave() {

  speed = beatsin8(6,0,255);

  clr1 = blend(CHSV(beatsin8(3,0,255),255,255), CHSV(beatsin8(4,0,255),255,255), speed);
  clr2 = blend(CHSV(beatsin8(4,0,255),255,255), CHSV(beatsin8(3,0,255),255,255), speed);

  loc1 = beatsin16(10,0,NUM_LEDS-1);
  
  fill_gradient_RGB(leds, 0, clr2, loc1, clr1);
  //fill_gradient_RGB(leds, loc1, clr2, NUM_LEDS-1, clr1);

} // blendwave()
 * */
/*
void blur() {
	uint8_t blurAmount = dim8_raw( beatsin8(3,64, 192) );       // A sinewave at 3 Hz with values ranging from 64 to 192.
	blur1d( leds, NUM_LEDS, blurAmount);                        // Apply some blurring to whatever's already on the strip, which will eventually go black.

	uint16_t  i = beatsin16(  9, 0, NUM_LEDS);
	uint16_t  j = beatsin16( 7, 0, NUM_LEDS);
	uint16_t  k = beatsin16(  5, 0, NUM_LEDS);

	// The color of each point shifts over time, each at a different speed.
	uint16_t ms = millis();  
	leds[(i+j)/2] = CHSV( ms / 29, 200, 255);
	leds[(j+k)/2] = CHSV( ms / 41, 200, 255);
	leds[(k+i)/2] = CHSV( ms / 73, 200, 255);
	leds[(k+i+j)/3] = CHSV( ms / 53, 200, 255);
}
*/

//confetti
unsigned long previousMillis;                                 // Store last time the strip was updated.


// Define variables used by the sequences.
uint8_t  thisfade = 8;                                        // How quickly does it fade? Lower = slower fade rate.
int       thishue = 50;                                       // Starting hue.
uint8_t   thisinc = 1;                                        // Incremental value for rotating hues
uint8_t   thissat = 200;                                      // The saturation, where 255 = brilliant colours.
uint8_t   thisbri = 255;                                      // Brightness of a sequence. Remember, max_bright is the overall limiter.
int       huediff = 256;                                      // Range of random #'s to use for hue
uint8_t	  thisdelay = 5;                                        // We don't need much delay (if any)


void ChangeMe() {                                             // A time (rather than loop) based demo sequencer. This gives us full control over the length of each sequence.
  uint8_t secondHand = (millis() / 1000) % 15;                // IMPORTANT!!! Change '15' to a different value to change duration of the loop.
  static uint8_t lastSecond = 99;                             // Static variable, means it's only defined once. This is our 'debounce' variable.
  if (lastSecond != secondHand) {                             // Debounce to make sure we're not repeating an assignment.
    lastSecond = secondHand;
    switch(secondHand) {
      case  0: thisinc=1; thishue=192; thissat=255; thisfade=2; huediff=256; break;  // You can change values here, one at a time , or altogether.
      case  5: thisinc=2; thishue=128; thisfade=8; huediff=64; break;
      case 10: thisinc=1; thishue=random16(255); thisfade=1; huediff=16; break;      // Only gets called once, and not continuously for the next several seconds. Therefore, no rainbows.
      case 15: break;                                                                // Here's the matching 15 for the other one.
    }
  }
} // ChangeMe()

void confetti() {                                             // random colored speckles that blink in and fade smoothly
	ChangeMe();                                                 // Check the demo loop for changes to the variables.

	EVERY_N_MILLISECONDS(thisdelay) {                           // FastLED based non-blocking delay to update/display the sequence.
    	fadeToBlackBy(leds, NUM_LEDS, thisfade);                    // Low values = slower fade.
		int pos = random16(NUM_LEDS);                               // Pick an LED at random.
		leds[pos] += CHSV((thishue + random16(huediff))/4 , thissat, thisbri);  // I use 12 bits for hue so that the hue increment isn't too quick.
		thishue = thishue + thisinc;                                // It increments here.
	}
} // confetti()


/*
int   thisdelay =   10;                                       // A delay value for the sequence(s)
uint8_t fadeval = 245;                                        // Trail behind the LED's. Lower => faster fade.

uint8_t bpm = 8;


void dot_beat() {

  uint16_t inner = beatsin16(bpm, NUM_LEDS/4, NUM_LEDS/4*3);    // Move 1/4 to 3/4
  uint16_t outer = beatsin16(bpm, 0, NUM_LEDS-1);               // Move entire length
  uint16_t middle = beatsin16(bpm, NUM_LEDS/3, NUM_LEDS/3*2);   // Move 1/3 to 2/3

  leds[middle] = CRGB::Purple;
  leds[inner] = CRGB::Blue;
  leds[outer] = CRGB::Aqua;

  nscale8(leds,NUM_LEDS,fadeval);                             // Fade the entire array. Or for just a few LED's, use  nscale8(&leds[2], 5, fadeval);

} // dot_beat()
*/

/*
CRGBPalette16 currentPalette=LavaColors_p;
CRGBPalette16 targetPalette=OceanColors_p;
 
uint8_t maxChanges = 24;                                      // Value for blending between palettes.

long map(long x, long in_min, long in_max, long out_min, long out_max) {
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

void noise16_1() {                                            // moves a noise up and down while slowly shifting to the side

  uint16_t scale = 500;                                      // the "zoom factor" for the noise

  for (uint16_t i = 0; i < NUM_LEDS; i++) {

    uint16_t shift_x = beatsin8(5);                           // the x position of the noise field swings @ 17 bpm
    uint16_t shift_y = millis() / 200;                        // the y position becomes slowly incremented
    

    uint16_t real_x = (i + shift_x)*scale;                    // the x position of the noise field swings @ 17 bpm
    uint16_t real_y = (i + shift_y)*scale;                    // the y position becomes slowly incremented
    uint32_t real_z = millis() * 10;                          // the z position becomes quickly incremented
    
    uint8_t noise = inoise16(real_x, real_y, real_z) >> 8;   // get the noise data and scale it down

    uint8_t index = sin8(noise*3);                           // map LED color based on noise data
    uint8_t bri   = noise;

    leds[i] = ColorFromPalette(currentPalette, index, bri, LINEARBLEND);   // With that value, look up the 8 bit colour palette value and assign it to the current LED.
  }

} // noise16_1()

void noise16_2() {                                            // just moving along one axis = "lavalamp effect"

  uint8_t scale = 1000;                                       // the "zoom factor" for the noise

  for (uint16_t i = 0; i < NUM_LEDS; i++) {

    uint16_t shift_x = millis() / 10;                         // x as a function of time
    uint16_t shift_y = 0;

    uint32_t real_x = (i + shift_x) * scale;                  // calculate the coordinates within the noise field
    uint32_t real_y = (i + shift_y) * scale;                  // based on the precalculated positions
    uint32_t real_z = 4223;
    
    uint8_t noise = inoise16(real_x, real_y, real_z) >> 8;    // get the noise data and scale it down

    uint8_t index = sin8(noise*3);                            // map led color based on noise data
    uint8_t bri   = noise;

    leds[i] = ColorFromPalette(currentPalette, index, bri, LINEARBLEND);   // With that value, look up the 8 bit colour palette value and assign it to the current LED.

  }
  
} // noise16_2()

void noise16_3() {                                            // no x/y shifting but scrolling along 

  uint8_t scale = 1000;                                       // the "zoom factor" for the noise

  for (uint16_t i = 0; i < NUM_LEDS; i++) {

    uint16_t shift_x = 4223;                                  // no movement along x and y
    uint16_t shift_y = 1234;

    uint32_t real_x = (i + shift_x) * scale;                  // calculate the coordinates within the noise field
    uint32_t real_y = (i + shift_y) * scale;                  // based on the precalculated positions
    uint32_t real_z = millis()*2;                             // increment z linear

    uint8_t noise = inoise16(real_x, real_y, real_z) >> 7;    // get the noise data and scale it down

    uint8_t index = sin8(noise*3);                            // map led color based on noise data
    uint8_t bri   = noise;

    leds[i] = ColorFromPalette(currentPalette, index, bri, LINEARBLEND);   // With that value, look up the 8 bit colour palette value and assign it to the current LED.
  }
  
} // noise16_3()

void inoise16_fill() {
	EVERY_N_MILLISECONDS(50) {
		//nblendPaletteTowardPalette(currentPalette, targetPalette, maxChanges);  // Blend towards the target palette
	  }

	  EVERY_N_SECONDS(5) {             // Change the target palette to a random one every 5 seconds.
		//targetPalette = CRGBPalette16(CHSV(random8(), 255, random8(128,255)), CHSV(random8(), 255, random8(128,255)), CHSV(random8(), 192, random8(128,255)), CHSV(random8(), 255, random8(128,255)));
	  }

	  noise16_2();
} // noise16_1()
*/

/*
#include <cstdlib>

CRGBPalette16 currentPalette;
CRGBPalette16 targetPalette;
TBlendType    currentBlending;                                // NOBLEND or LINEARBLEND

uint8_t colour;                                               // Ripple colour is randomized.
int center = 0;                                               // Center of the current ripple.
int step = -1;                                                // -1 is the initializing step.
uint8_t myfade = 255;                                         // Starting brightness.
#define maxsteps 16                                           // Case statement wouldn't allow a variable.
uint8_t fadeval = 64;

uint8_t bgcol = 0;                                            // Background colour rotates.
int thisdelay = 60;                                          // Standard delay value.


void ripple() {
 EVERY_N_MILLISECONDS(100) {
    uint8_t maxChanges = 24; 
    nblendPaletteTowardPalette(currentPalette, targetPalette, maxChanges);   // AWESOME palette blending capability.
  }

  EVERY_N_SECONDS(3) {
      targetPalette = CRGBPalette16(CHSV(random8(), 255, 32), CHSV(random8(), random8(64)+192, 255), CHSV(random8(), 255, 32), CHSV(random8(), 255, 255)); 
  }

  EVERY_N_MILLISECONDS(thisdelay) {                                   // FastLED based non-blocking delay to update/display the sequence.
    	
  fadeToBlackBy(leds, NUM_LEDS, fadeval);                             // 8 bit, 1 = slow, 255 = fast
  
  switch (step) {

    case -1:                                                          // Initialize ripple variables.
      center = random()%NUM_LEDS;
      colour = random8();
      step = 0;
      break;

    case 0:
      leds[center] = ColorFromPalette(currentPalette, colour, myfade, currentBlending);
      
      step ++;
      break;

    case maxsteps:                                                    // At the end of the ripples.
      step = -1;
      break;

    default:                                                          // Middle of the ripples.
      leds[(center + step + NUM_LEDS) % NUM_LEDS] += ColorFromPalette(currentPalette, colour, myfade/step*2, currentBlending);       // Simple wrap from Marc Miller
      leds[(center - step + NUM_LEDS) % NUM_LEDS] += ColorFromPalette(currentPalette, colour, myfade/step*2, currentBlending);
      step ++;                                                         // Next step.
      break;  
  } // switch step
  }
	

  
} // ripple()
 */

int main() {
	wiringPiSetup();
	
	piLED = PiLED(leds, NUM_LEDS);
  
	// set master brightness control
	piLED.setBrightness(BRIGHTNESS);
	
	piLED.setCorrection(0xFCFFF0);

	for (;;) {
		//blendwave();
		//blur();
		confetti();
		//dot_beat();
		//inoise16_fill();
		//ripple();
		piLED.show();
	}
	
}


uint16_t XY( uint8_t x, uint8_t y) {
  return 0; 
}