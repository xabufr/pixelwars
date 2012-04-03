#include "soundengine.h"
#include "../engineevent.h"
#include "core/logger.h"

SoundEngine::SoundEngine()
{
    m_last = 0;
}

SoundEngine::~SoundEngine()
{
    //dtor
}
unsigned int SoundEngine::PlaySound(const std::string& chemin)
{
    m_sounds[m_last] = new SoundParams;
    sf::Sound *s = new sf::Sound;
    m_sounds[m_last]->sound = s;
    s->SetRelativeToListener(true);
    s->SetPosition(0,0,0);
    s->SetBuffer(*SoundBufferManager::GetInstance()->Get(chemin));
    s->Play();

    return m_last++;
}
unsigned int SoundEngine::PlayLocalizedSound(const std::string& chemin)
{
    m_sounds[m_last] = new SoundParams;
    m_sounds[m_last]->spacialized=true;
    m_sounds[m_last]->sound = new sf::Sound;
    m_sounds[m_last]->sound->SetRelativeToListener(false);
    m_sounds[m_last]->sound->SetPosition(0,0,0);
    m_sounds[m_last]->sound->SetBuffer(*SoundBufferManager::GetInstance()->Get(chemin));
    m_sounds[m_last]->sound->Play();
    return m_last++;
}
void SoundEngine::RemoveWhenFinished(unsigned int id, bool rem)
{
    auto it = m_sounds.find(id);
    if(it==m_sounds.end())
        return;
    it->second->removeWhenFinished=rem;
}
void SoundEngine::Work()
{
    for(auto it=m_sounds.begin();it!=m_sounds.end();)
    {
        if(it->second->sound->GetStatus()==sf::Sound::Status::Stopped&&it->second->removeWhenFinished)
        {
            delete it->second->sound;
            it = m_sounds.erase(it);
        }
        else
        {
            if(it->second->isFadeIn&&it->second->sound->GetStatus()==sf::Sound::Status::Playing)
            {
                if(it->second->timerIn.GetElapsedTime().AsSeconds()>=it->second->timeIn)
                {
                    it->second->isFadeIn = false;
                    it->second->sound->SetVolume(100.f);
                }
                else
                {
                    float ratio = 100*it->second->timerIn.GetElapsedTime().AsSeconds()/it->second->timeIn;
                    it->second->sound->SetVolume(ratio);
                }
            }
            if(it->second->isFadeOut&&it->second->sound->GetStatus()==sf::Sound::Status::Playing)
            {
                if(it->second->timerOut.GetElapsedTime().AsSeconds()>=it->second->timeOut)
                {
                    it->second->isFadeOut = false;
                    it->second->sound->Stop();
                }
                else
                {

                    float ratio = it->second->fadeOutDelta-(100*it->second->timerOut.GetElapsedTime().AsSeconds()/it->second->timeOut);
                    if(ratio<=0.f)
                    {
                        it->second->isFadeOut = false;
                        it->second->sound->Stop();
                    }
                    else
                    {
                        it->second->sound->SetVolume(ratio);
                    }
                }
            }
            it++;
        }
    }
}
void SoundEngine::HandleEngineEvent(EngineEvent* event)
{
    switch(event->GetMessage())
    {
    case TypeMessage::JouerSon:
        this->PlaySound(event->GetStrings().find(IndexMessages::Chemin)->second);
        break;
    }
}
EngineType SoundEngine::GetEngineId() const
{
    return EngineType::Audio_engine;
}
sf::Sound* SoundEngine::GetSound(SoundId id)
{
    auto it = m_sounds.find(id);
    if(it==m_sounds.end())
        return 0;
    return it->second->sound;
}
void SoundEngine::FadeIn(SoundId id, float time)
{
    m_sounds[id]->isFadeIn=true;
    m_sounds[id]->isFadeOut=false;
    m_sounds[id]->timerIn.Restart();
    m_sounds[id]->timeIn=time;
    m_sounds[id]->sound->SetVolume(0.f);
    m_sounds[id]->sound->Play();
}
void SoundEngine::Stop(SoundId id)
{
    m_sounds[id]->sound->Stop();
}
void SoundEngine::Play(SoundId id)
{
    m_sounds[id]->sound->Play();
    m_sounds[id]->isFadeIn=false;
    m_sounds[id]->isFadeOut=false;
}
void SoundEngine::FadeOut(SoundId id, float time)
{
    if(m_sounds[id]->isFadeOut)
        return;
    m_sounds[id]->isFadeOut=true;

    m_sounds[id]->timerOut.Restart();
    m_sounds[id]->timeOut=time;
    if(m_sounds[id]->isFadeIn)
    {
        m_sounds[id]->fadeOutDelta=m_sounds[id]->sound->GetVolume();
        m_sounds[id]->isFadeIn=false;
    }
    else
        m_sounds[id]->fadeOutDelta=100.f;
}

