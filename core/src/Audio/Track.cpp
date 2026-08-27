#include "Audio/Track.hpp"
#include "Audio/AudioManager.hpp"
#include "SDL3_mixer/SDL_mixer.h"

namespace FWE::Audio {
    Track::Track() {
        this->mixerTrack = MIX_CreateTrack(Audio::AudioManager::GetInstance()->GetMixer());
    }
    
    Track::Track(const Track& track) {
        this->audio = track.audio;
        this->volume = track.volume;
        this->mixerTrack = MIX_CreateTrack(Audio::AudioManager::GetInstance()->GetMixer());
    }

    Track::~Track() {
        MIX_DestroyTrack(this->mixerTrack);
    }

    void Track::Play(bool loop) {
        if(this->mixerTrack == nullptr){
            return;
        }
        MIX_SetTrackAudio(this->mixerTrack, this->audio);
        MIX_PlayTrack(this->mixerTrack, 0);
        MIX_SetTrackLoops(this->mixerTrack, -1 * loop);
    }

    void Track::SetAudio(MIX_Audio* audio) {
        this->audio = audio;
    }

    void Track::SetVolume(float volume) {
        this->volume = volume;
        MIX_SetTrackGain(this->mixerTrack, volume);
    }

    MIX_Track* Track::GetMixerTrack() {
        return this->mixerTrack;
    }
}