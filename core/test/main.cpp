#include <Audio/AudioManager.hpp>

int main() {
    FWE::Audio::AudioManager* am = FWE::Audio::AudioManager::GetInstance();

    am->Init();
    auto audio = am->LoadAudio("bulletproof.mp3");
    auto track = am->CreateTrack(audio);
    track->SetVolume(1.75f);
    track->Play();

    while (true) {}
}