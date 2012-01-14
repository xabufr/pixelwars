#ifndef GAME_H
#define GAME_H
#include "engines/engine.h"
#include "engines/network/networkengine.h"
#include "engines/sound/soundengine.h"
#include "engines/game/gameengine.h"
#include "engines/type.h"

class Game
{
    public:
        Game();
        ~Game();
        Engine* GetEngineFromId(EngineType id);
    protected:
    private:
        NetworkEngine* m_network;
        GameEngine *m_game;
        SoundEngine *m_sound;
};

#endif // GAME_H
