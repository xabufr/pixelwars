#ifndef SOUNDBUFFERMANAGER_H
#define SOUNDBUFFERMANAGER_H

#include "../../core/singleton.h"
#include <SFML2/Audio.hpp>
#include <unordered_map>

class SoundBufferManager: public Singleton<SoundBufferManager>
{
    friend class Singleton<SoundBufferManager>;
    public:
        sf::SoundBuffer* Get(const std::string&);
    protected:
        SoundBufferManager();
        virtual ~SoundBufferManager();
    private:
        std::unordered_map<std::string, sf::SoundBuffer*> m_buffers;
};

#endif // SOUNDBUFFERMANAGER_H
