PiLED 0.0.1
===========

This is my attempt at porting the FastLED library for use on the RaspberryPi.  I understand the reasoning for not including devices like the Pi in FastLed because of timing issues and driving certain kinds of light strips.
I only wanted to drive APA102 (Dotstar) leds, which don't have strict timing requirements for their data signal.

Small disclaimer: This is not an exact/direct port.  I have copied as much as I can from FastLED verbatim, but there are some missing features.  This library is only set up to drive APA102 (Dotstar) LED strips, and it can only drive one strand at a time.
There are a few reasons for this: 1. I am only interested in that kind of light strip, 2. I have practically no development experience on the Pi, so this is also a project to help me learn to develop on the Pi. 3. I am not a C/C++ developer, so I'm trying to get just the basics working.
This might change in the future as I develop my skills or someone decides to contribute.

I have done my best to make this library work as close as possible to FastLED so that their documentation should be very helpful. The only difference so far is initializing the library. After that, all functions that manipulate colors will work the same.