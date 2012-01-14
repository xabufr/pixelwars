#include "game.h"

Game::Game()
{
    //ctor
}

Game::~Game()
{
    //dtor
}
Engine* Game::GetEngineFromId(EngineType id)
{
    switch(id)
    {
    case Network_engine:
        return m_network;
    default:;
    }
    return 0;
}
