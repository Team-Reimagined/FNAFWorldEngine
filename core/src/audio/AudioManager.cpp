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

    MIX_Audio* AudioManager::LoadAudio(std::string filePath) {
        MIX_Audio* audio = MIX_LoadAudio(this->mixer, filePath.c_str(), true);
        if (!audio) {
            Util::Logging::error("Couldn't load audio from %s: %s", filePath, SDL_GetError());
            return nullptr;
        }

        return audio;
    }

    Track* AudioManager::CreateTrack() {
        Track* track = new Track(this->mixer);

        return track;
    }

    Track* AudioManager::CreateTrack(MIX_Audio* audio) {
        Track* track = new Track(this->mixer);
        track->SetAudio(audio);

        return track;
    }
}