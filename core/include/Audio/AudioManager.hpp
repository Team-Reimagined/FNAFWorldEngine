#pragma once

#include <SDL3_mixer/SDL_mixer.h>

namespace FWE::Audio {
    class AudioManager {
    public:
        static AudioManager* GetInstance() {
            static AudioManager instance;
            return &instance;
        }

        bool Init();

        MIX_Mixer* GetMixer();

    private:
        MIX_Mixer* mixer;
    };
}