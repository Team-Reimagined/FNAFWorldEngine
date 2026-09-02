#pragma once

#include "Audio/Track.hpp"
#include "SDL3_mixer/SDL_mixer.h"
#include <string>
#include <unordered_map>

namespace FWE::ResourceLoader
{
    class AudioLoader
    {
    public:
        static AudioLoader *GetInstance();
        Audio::Track LoadAudio(const char *filePath);
    private:
        std::unordered_map<std::string, MIX_Audio *> tracks;
    };
}