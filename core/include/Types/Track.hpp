#pragma once

#include <SDL3_mixer/SDL_mixer.h>

namespace FWE::Audio {
    class Track {
    public:
        Track(MIX_Mixer* mixer);
        void Play();

        void SetAudio(MIX_Audio* audio);
        void SetVolume(float volume);

        MIX_Track* GetMixerTrack();

    private:
        MIX_Track* mixerTrack;
        MIX_Audio* audio;
        float volume;
    };
}