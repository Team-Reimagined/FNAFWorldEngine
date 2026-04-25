#pragma once

#include <string>

#include <SDL3_mixer/SDL_mixer.h>

namespace FWE::Audio {
    class AudioManager {
    public:
        static AudioManager* get() {
            static AudioManager instance;
            return &instance;
        }

        bool Init();
        MIX_Audio* LoadAudio(std::string filePath);
        MIX_Track* CreateTrack();
        void Play(MIX_Audio* audio, MIX_Track* track);
    
    private:
        MIX_Mixer* mixer;
    };
}