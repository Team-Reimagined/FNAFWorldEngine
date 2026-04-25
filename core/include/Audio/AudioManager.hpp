#pragma once

#include <string>

#include <SDL3_mixer/SDL_mixer.h>

#include "Audio/types/Track.hpp"

namespace FWE::Audio {
    class AudioManager {
    public:
        static AudioManager* GetInstance() {
            static AudioManager instance;
            return &instance;
        }

        bool Init();
        MIX_Audio* LoadAudio(std::string filePath);

        Track* CreateTrack();
        Track* CreateTrack(MIX_Audio* audio);

    private:
        MIX_Mixer* mixer;
    };
}