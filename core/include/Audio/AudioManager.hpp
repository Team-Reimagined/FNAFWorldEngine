#pragma once

#include <SDL3_mixer/SDL_mixer.h>

namespace FWE::Audio {
    class AudioManager {
    public:
        static AudioManager* get() {
            static AudioManager instance;
            return &instance;
        }

        void Init();
    
    private:
        MIX_Mixer* mixer;
    };
}