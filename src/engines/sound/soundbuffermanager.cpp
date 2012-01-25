#include "soundbuffermanager.h"
#include "core/logger.h"
template<>
SoundBufferManager* Singleton<SoundBufferManager>::m_singleton=NULL;
SoundBufferManager::SoundBufferManager()
{
    Logger::Log()<<"Création du SoundBufferManager" << Logger::endl;
}

SoundBufferManager::~SoundBufferManager()
{
    for(auto &it : m_buffers)
    {
        delete it.second;
    }
    Logger::Log()<<"Destruction du SoundBufferManager" << Logger::endl;
}
sf::SoundBuffer* SoundBufferManager::Get(const std::string& chemin)
{
    auto it = m_buffers.find(chemin);
    if(it!=m_buffers.end())
        return it->second;
    sf::SoundBuffer *buffer = new sf::SoundBuffer;
    Assert(buffer->LoadFromFile(chemin));
    m_buffers[chemin] = buffer;
    return buffer;
}
