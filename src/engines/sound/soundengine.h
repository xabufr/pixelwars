#ifndef SOUNDENGINE_H
#define SOUNDENGINE_H
#include "../engine.h"
#include "soundbuffermanager.h"

struct SoundParams
{
    sf::Sound *sound;
    bool removeWhenFinished;
    bool spacialized, isFadeIn, isFadeOut;
    sf::Clock timerIn, timerOut;
    float timeIn, timeOut;
    float fadeOutDelta;
    SoundParams()
    {
        removeWhenFinished=true;
        spacialized=false;
        isFadeIn=false;
        isFadeOut=false;
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

        void FadeIn(SoundId, float);
        void FadeOut(SoundId, float);
        void Stop(SoundId);
        void Play(SoundId);

        sf::Sound* GetSound(SoundId);

        void RemoveWhenFinished(SoundId, bool=true);

        virtual void Work();
        virtual void HandleEngineEvent(EngineEvent*);

        virtual EngineType GetEngineId() const;

    protected:
    private:
        std::unordered_map<unsigned int, SoundParams*> m_sounds;
        unsigned int m_last;
};

#endif // SOUNDENGINE_H
