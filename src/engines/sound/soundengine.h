#ifndef SOUNDENGINE_H
#define SOUNDENGINE_H
#include "../engine.h"
#include "soundbuffermanager.h"

struct SoundParams
{
    sf::Sound *sound;
    bool removeWhenFinished;
    bool spacialized;
    SoundParams()
    {
        removeWhenFinished=true;
        spacialized=false;
    }
};
typedef unsigned int SoundId;
class SoundEngine : public Engine
{
    public:
        SoundEngine();
        virtual ~SoundEngine();

        SoundId PlaySound(const std::string&);
        SoundId PlayLocalizedSound(const std::string&);

        void SetListenerPosition(sf::Vector2f pos);
        void SetListenerDirection(sf::Vector2f dir);
        void SetGlobalVolume(float);

        sf::Sound* GetSound(SoundId);

        void RemoveWhenFinished(SoundId, bool=true);

        virtual void Work();
        virtual void HandleEngineEvent(EngineEvent*);

        virtual EngineType GetEngineId() const;

    protected:
    private:
        std::unordered_map<unsigned int, SoundParams> m_sounds;
        unsigned int m_last;
};

#endif // SOUNDENGINE_H
