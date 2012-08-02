#include "Audio.h"

struct sample {
   Uint8 *data;
   Uint32 dpos;
   Uint32 dlen;
   int volume;
} sounds [CHANNELS];

int setupaudio() {

   // Audio format structure - needs to be created before we open audio
   SDL_AudioSpec format;

   // Set up AudioSpec structure
   format.freq = 44100;
   format.format = AUDIO_S16;
   format.channels = 2;
   format.samples = 512;   /* Good for games, according to SDL docs */
	format.callback = mixaudio;
   format.userdata = NULL;

   // Open SDL audio with the settings stored in format
   if (SDL_OpenAudio(&format, NULL) < 0) {
      fprintf(stderr, "Unable to open audio: %s\n", SDL_GetError());
      return -1;
   }

   // You need to call this before any sound playback
   SDL_PauseAudio(0);

   return 0;
}

void closeaudio() {
   SDL_CloseAudio();
}

void mixaudio(void* unused, Uint8* stream, int len) {
	int i;
	Uint32 amount;

	for (i=0; i<CHANNELS; ++i) {
		amount = (sounds[i].dlen - sounds[i].dpos);
		if (amount > (Uint32)len)
			amount = len;
		SDL_MixAudio(stream, &sounds[i].data[sounds[i].dpos], amount, sounds[i].volume);
		sounds[i].dpos += amount;
	}
}

void playSound(char *filename, double volume_percentage) {
	/* This function is derived directly from the SDL online example */
	int index;
	SDL_AudioSpec wave;
	Uint8 *data;
	Uint32 dlen;
	SDL_AudioCVT cvt;

	/* Look for and empty (or finished) sound slot */
	for (index=0; index<CHANNELS; ++index) {
		if (sounds[index].dpos == sounds[index].dlen) {
			break;
		}
	}
	if (index == CHANNELS)
		return;

	/* Load the sound file and convert it to a 16-bit stereo at 44kHz */
	if (SDL_LoadWAV(filename, &wave, &data, &dlen) == NULL) {
		fprintf(stderr, "Couldn't load %s: %s\n", filename, SDL_GetError());
		return;
	}

	SDL_BuildAudioCVT(&cvt, wave.format,	wave.channels,		wave.freq,
									AUDIO_S16,		2,						44100);
	cvt.buf = (Uint8*)malloc(dlen*cvt.len_mult);
	memcpy(cvt.buf, data, dlen);
   cvt.len = dlen;
	SDL_ConvertAudio(&cvt);
	
   /* We're done with the wave data... let it go */
   SDL_FreeWAV(data);

	/* Put the sound data in the slot (it starts playing immediately */
	if (sounds[index].data) {
		free(sounds[index].data);
	}
	SDL_LockAudio();
	sounds[index].data = cvt.buf;
	sounds[index].dlen = cvt.len_cvt;
	sounds[index].dpos = 0;
   sounds[index].volume = audio_calc_volume(volume_percentage);
	SDL_UnlockAudio();
}

int audio_calc_volume(double volume_percent) {
   if (volume_percent >= 1) {
      return SDL_MIX_MAXVOLUME;
   }
   else if (volume_percent <= .10) {
      return (int)(0.10 * SDL_MIX_MAXVOLUME);
   }
   else {
      return (int)(volume_percent * SDL_MIX_MAXVOLUME);
   }
}

double audio_calc_percent_by_distance(double distance) {
   if (distance < 5.0) {
      return 1.0;
   }
   else if (distance > 55.0) {
      return 0.0;
   }
   else {
      return (1.0f - (distance * 0.1f));
   }
}