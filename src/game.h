#ifndef GAME_H
#define GAME_H
#include "engines/engine.h"
#include "engines/network/networkengine.h"
#include "engines/type.h"

class Game
{
    public:
        Game();
        virtual ~Game();
        Engine* GetEngineFromId(EngineType id);
    protected:
    private:
        NetworkEngine* m_network;
};

#endif // GAME_H
