#include <Audio/AudioManager.hpp>

int main() {
    FWE::Audio::AudioManager* am = FWE::Audio::AudioManager::get();

    am->Init();
    auto audio = am->LoadAudio("bulletproof.mp3");
    auto track = am->CreateTrack();
    am->Play(audio, track);

    while (true) {}
}