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
    Logger::Log()<<chemin<<Logger::endl;
    m_sounds[m_last] = SoundParams();
    m_sounds[m_last].sound = new sf::Sound;
    m_sounds[m_last].sound->SetRelativeToListener(true);
    m_sounds[m_last].sound->SetPosition(0,0,0);
    m_sounds[m_last].sound->SetBuffer(*SoundBufferManager::GetInstance()->Get(chemin));
    m_sounds[m_last].sound->Play();
    return m_last++;
}
unsigned int SoundEngine::PlayLocalizedSound(const std::string& chemin)
{
    m_sounds[m_last] = SoundParams();
    m_sounds[m_last].spacialized=true;
    m_sounds[m_last].sound = new sf::Sound;
    m_sounds[m_last].sound->SetRelativeToListener(false);
    m_sounds[m_last].sound->SetPosition(0,0,0);
    m_sounds[m_last].sound->SetBuffer(*SoundBufferManager::GetInstance()->Get(chemin));
    m_sounds[m_last].sound->Play();
    return m_last++;
}
void SoundEngine::RemoveWhenFinished(unsigned int id, bool rem)
{
    auto it = m_sounds.find(id);
    if(it==m_sounds.end())
        return;
    it->second.removeWhenFinished=rem;
}
void SoundEngine::Work()
{
    for(auto it=m_sounds.begin();it!=m_sounds.end();)
    {
        if(it->second.sound->GetStatus()==sf::Sound::Status::Stopped&&it->second.removeWhenFinished)
        {
            delete it->second.sound;
            it = m_sounds.erase(it);
        }
        else
        {
            it++;
        }
    }
}
void SoundEngine::HandleEngineEvent(EngineEvent* event)
{
    Logger::Log()<<"handle"<<Logger::endl;
    switch(event->GetMessage())
    {
    case TypeMessage::JouerSon:
        Logger::Log()<<"son"<<Logger::endl;
        this->PlaySound(event->GetStrings().find(IndexMessages::Chemin)->second);
        break;
    }
}
EngineType SoundEngine::GetEngineId() const
{
    return EngineType::Audio_engine;
}
