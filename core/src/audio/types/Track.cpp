#include "Types/Track.hpp"

#include "Util/Logging.hpp"

namespace FWE::Audio {
    Track::Track(MIX_Mixer* mixer) {
        this->mixerTrack = MIX_CreateTrack(mixer);
    }

    void Track::Play() {
        MIX_SetTrackAudio(this->mixerTrack, this->audio);
        MIX_PlayTrack(this->mixerTrack, 0);
    }

    void Track::SetAudio(MIX_Audio* audio) {
        this->audio = audio;
    }

    void Track::SetVolume(float volume) {
        MIX_SetTrackGain(this->mixerTrack, volume);
    }

    MIX_Track* Track::GetMixerTrack() {
        return this->mixerTrack;
    }
}