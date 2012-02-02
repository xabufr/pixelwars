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
class SoundEngine : public Engine
{
    public:
        SoundEngine();
        virtual ~SoundEngine();

        unsigned int PlaySound(const std::string&);
        unsigned int PlayLocalizedSound(const std::string&);

        void SetListenerPosition(sf::Vector2f pos);
        void SetListenerDirection(sf::Vector2f dir);
        void SetGlobalVolume(float);

        sf::Sound* GetSound(unsigned int);

        void RemoveWhenFinished(unsigned int, bool=true);

        virtual void Work();
        virtual void HandleEngineEvent(EngineEvent*);

        virtual EngineType GetEngineId() const;

    protected:
    private:
        std::unordered_map<unsigned int, SoundParams> m_sounds;
        unsigned int m_last;
};

#endif // SOUNDENGINE_H
