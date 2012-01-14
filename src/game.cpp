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
    case EngineType::Network_engine:
        return m_network;
    case EngineType::Game_engine:
        return m_game;
    case EngineType::Audio_engine:
        return m_sound;
    }
    return NULL;
}
