#include "gameengine.h"
#include "engines/graphics/graphicalengine.h"
#include "core/logger.h"
#include <SFML2/System.hpp>
#include <Box2D/Box2D.h>
#include "uniteterrestre.h"
#include "projectile.h"

GameEngine::GameEngine()
{
    //ctor
}

GameEngine::~GameEngine()
{
    GraphicalEngine::Kill();
}
EngineType GameEngine::GetEngineId() const
{
    return EngineType::Game_engine;
}
void GameEngine::Start()
{
    GraphicalEngine* gengine = GraphicalEngine::GetInstance();
    sf::RenderWindow *app = gengine->CreateRenderWindow(sf::VideoMode(600,400), "Test");


    b2Vec2 gravity;
	gravity.Set(0.0f, -10.0f);
	m_world = new b2World(gravity);
	m_world->SetContactListener(&m_listner);
	m_carte = new Carte(m_world, sf::Vector2i(600,400), 0);


    UniteTerrestre unit1(m_world);
    InputManager imanager;
    Projectile proj(m_world, b2Vec2(20,-10), b2Vec2(0.2,0.2), 1,0.1,100);

    sf::Event event;
    while(app->IsOpened())
    {
        while(app->PollEvent(event))
        {
            gengine->GetGuiManager()->HandleEvent(event);
            if(event.Type==sf::Event::Closed)
                app->Close();
        }
        m_world->Step(1.f/60.f, 8, 3);
        unit1.Update();
        gengine->DrawScene();
        GererExplosions();
        unit1.Deplacer(imanager.GetAll());
        proj.Update();
    }
}
void GameEngine::GererExplosions()
{
    const std::vector<ExplosionPosition> &explosions = m_listner.GetExplosions();
    m_carte->DemarrerDestruction();
    for(const ExplosionPosition &exp : explosions)
    {
        m_carte->AjouterExplosion(exp.position, exp.radius);
    }
    m_carte->FinirDestruction();

    m_listner.Clear();
}
