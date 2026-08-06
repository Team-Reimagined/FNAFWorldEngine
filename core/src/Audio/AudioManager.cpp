#include "Audio/AudioManager.hpp"

#include "Util/Logging.hpp"

namespace FWE::Audio {
    bool AudioManager::Init() {
        if (!MIX_Init()) {
            Util::Logging::error("Couldn't initialize SDL_mixer: {}", SDL_GetError());
            return false;
        }

        this->mixer = MIX_CreateMixerDevice(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, NULL);
        if (!this->mixer) {
            Util::Logging::error("Couldn't create mixer: {}", SDL_GetError());
            return false;
        }

        return true;
    }

    MIX_Mixer* AudioManager::GetMixer() {
        return mixer;
    }
}