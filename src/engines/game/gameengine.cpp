#include "gameengine.h"
#include "engines/graphics/graphicalengine.h"
#include "core/logger.h"
#include <SFML2/System.hpp>
#include <Box2D/Box2D.h>
#include "uniteterrestre.h"
#include "projectile.h"
#include "uniteterrestremodel.h"
#include "joueurhumain.h"

GameEngine::GameEngine(): m_listner(m_unites)
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
    m_unites[0]=unit1;

    InputManager imanager;
    m_joueurLocal[0] = new JoueurHumain;
    m_joueurLocal[0]->AjouterUnite(0,unit1);

    m_unitesJoueurs[unit1]=m_joueurLocal[0];

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
        m_joueurLocal[0]->Update();
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
            EnleverUnit(unit1);
        }

    }
    delete m_joueurLocal[0];
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
    const std::vector<ExplosionImpusle>& impulses = m_listner.GetImpulsions();
    for(const ExplosionImpusle &impulse : impulses)
    {
        impulse.object->ApplyLinearImpulse(impulse.impulse, impulse.pts);
    }

    const std::vector<Projectile*> &toDestroy = m_listner.GetProjectileToDestroy();
    for(Projectile *projectile : toDestroy)
    {
        DeleteProjectile(projectile);
    }

    m_listner.Clear();
}
void GameEngine::DeleteProjectile(Projectile* toDelete)
{
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
void GameEngine::EnleverUnit(Unite* unite)
{
    m_unitesJoueurs[unite]->EnleverUnite(unite);
    for(auto it = m_unites.begin();it!=m_unites.end();it++)
    {
        if(it->second==unite)
        {
            EnleverUnit(it->first);
            return;
        }
    }
}
void GameEngine::EnleverUnit(sf::Uint32 id)
{
    auto it = m_unites.find(id);
    delete it->second;
    m_unites.erase(it);
}
