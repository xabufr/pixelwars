#include "game.h"
#include "engines/engineevent.h"

Game::Game()
{
    m_game = new GameEngine;
    m_engines.push_back(m_game);
    EngineEvent::SetGame(this);
}

Game::~Game()
{
    delete m_game;
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
void Game::Start()
{
    m_run=true;
    m_game->Start();
    while(m_run)
    {
        m_game->Work();
        this->Work();
    }
}
void Game::SetTailleCarte(int taille)
{
    m_largeurCarte=taille;
    m_game->SetTailleMap(taille);
}
void Game::Work()
{
    for(Engine* engine: m_engines)
    {
        const std::vector<EngineEvent*>& events = engine->GetEvents();
        for(EngineEvent* event : events)
        {
            if(event->GetMessage() == TypeMessage::Quitter)
            {
                m_run=false;
            }
            else
            {

            }
        }
    }
}
