#ifndef AUDIO_MANAGER_H
#define AUDIO_MANAGER_H

#include <SDL/SDL.h>

// Audio engine class. All audio is run through this class. This class contains
// all functions necessary to control sound effects and music.
class AudioManager {
public:
    // Sets up audio and prepares for playback of sound effects and music
    bool Initialize();

    // Closes SDL audio upon close of program
    void Shutdown();

    // Opens a sound file and plays the sound effect
    void Play(char *filename, double volume_percentage);

    // Function used by the SDL audio callback function to generate samples
    void GenerateSamples(Uint8* stream, int len);

    // Defines the volume percentage of sounds that are local (HUD, weapons, etc)
    static const float AUDIO_LOCAL;

    // Defines the number of sounds that can be playing consecutively
    static const Uint8 CHANNELS = 16;

protected:
    // Calculates the volume to pass into the mixer for each channel based
    // on SDL defined maximum volume
    int CalculateVolume(double volume_percent);

    // Calculates a volume percentage based on how far away the sound is coming from.
    // Inputs a distance from the listener and returns a percentage of max
    // volume (to be used in calculating the volume of the sound effect)
    double CalculatePercentByDistance(double distance);
};

// Callback function that mixes audio and outputs it to the device
void AudioCallback(void *_audioManager, Uint8 *stream, int len);
#endif