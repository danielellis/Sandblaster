#ifndef AUDIO_H
#define AUDIO_H
/* Audio engine class. All audio is run through this class! Nowhere else
   should audio be even considered. This class contains all functions
   necessary to control sound effects and music */

#include <SDL\SDL.h>

// Defines the number of sounds that can be playing consecutively
#define CHANNELS 16
// Defines the volume percentage of sounds that are local (HUD, weapons, etc)
#define AUDIO_LOCAL 1.0

// Sets up audio and prepares for playback of sound effects and music
int setupaudio();

// Closes SDL audio upon close of program
void closeaudio();

// Callback unction that mixes audio and outputs it to the device
void mixaudio(void* unused, Uint8* stream, int len);

// Opens a sound file and plays the sound effect
void playSound(char *filename, double volume_percentage);

// Calculates the volume to pass into the mixer for each channel based
// on SDL defined maximum volume
int audio_calc_volume(double volume_percent);

// Calculates a volume percentage based on how far away the sound is coming from.
// Inputs a distance from the listener and returns a percentage of max
// volume (to be used in calculating the volume of the sound effect)
double audio_calc_percent_by_distance(float distance);
   
#endif