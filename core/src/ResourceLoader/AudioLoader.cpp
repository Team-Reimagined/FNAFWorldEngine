#include "ResourceLoader/AudioLoader.hpp"
#include "Audio/AudioManager.hpp"
#include "Util/Logging.hpp"

namespace FWE::ResourceLoader
{
    AudioLoader::~AudioLoader()
    {
        tracks.clear();
    }

    Audio::Track AudioLoader::LoadAudio(const char *filePath)
    {
        Audio::AudioManager *manager = Audio::AudioManager::GetInstance();
        if(tracks.find(filePath) != tracks.end())
        {
            Audio::Track track;
            track.SetAudio(tracks.at(filePath));
            return track;
        }
        else
        {
            Audio::Track track;
            MIX_Audio *audio = MIX_LoadAudio(manager->GetMixer(), filePath, true);
            if(audio != nullptr)
            {
                track.SetAudio(audio);
                tracks.insert({filePath, audio});
            }
            else
            {
                Util::Logging::error("Unable to load audio at path: {}", filePath);
            }
            return track;
        }
    }

    AudioLoader *AudioLoader::GetInstance()
    {
        static AudioLoader loader;
        return &loader;
    }
}