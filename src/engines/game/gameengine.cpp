#include "gameengine.h"
#include "engines/graphics/graphicalengine.h"
#include "core/logger.h"
#include <SFML2/System.hpp>
#include <Box2D/Box2D.h>
#include "uniteterrestre.h"
#include "projectile.h"
#include "uniteterrestremodel.h"

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
    UniteTerrestreModel::GetInstance();

    UniteTerrestre *unit1 = new UniteTerrestre(m_world, b2Vec2(10,0));
    InputManager imanager;

    sf::Event event;
    bool viv=true;
    while(app->IsOpened())
    {
        while(app->PollEvent(event))
        {
            gengine->GetGuiManager()->HandleEvent(event);
            if(event.Type==sf::Event::Closed)
                app->Close();
        }


        m_world->Step(1.f/60.f, 8, 3);
        if(viv)
        unit1->Update();
        gengine->DrawScene();
        GererExplosions();
        if(viv)
        unit1->Deplacer(imanager.GetAll());

        MoveProjectiles();
        if(viv)
        if(unit1->PeutTirer())
        {
            m_projectiles.push_back(unit1->Tirer());
        }
        if(viv&&!unit1->EstVivant())
        {
            viv=false;
            delete unit1;
        }
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
    const std::vector<Projectile*> &toDestroy = m_listner.GetProjectileToDestroy();
    for(Projectile *projectile : toDestroy)
    {
        DeleteProjectile(projectile);
    }

    m_listner.Clear();
}
void GameEngine::DeleteProjectile(Projectile* toDelete)
{
    /*const size_t taille = m_projectiles.size();
    for(size_t i=0;i<taille;++i)
    {
        if(toDelete==m_projectiles[i])
        {
            delete toDelete;
            m_projectiles.erase(m_projectiles.begin()+i);
            return;
        }
    }*/
    for(auto it = m_projectiles.begin();it!=m_projectiles.end();it++)
    {
        if(toDelete==(*it))
        {
            delete toDelete;
            m_projectiles.erase(it);
            return;
        }
    }
}
void GameEngine::MoveProjectiles()
{
    for(Projectile *proj : m_projectiles)
        proj->Update();
}
