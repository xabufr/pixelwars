#include "gameengine.h"
#include "engines/graphics/graphicalengine.h"
#include "core/logger.h"
#include <SFML2/System.hpp>
#include <Box2D/Box2D.h>
#include "uniteterrestre.h"
#include "projectile.h"
#include "uniteterrestremodel.h"
#include "joueurhumain.h"

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

    uniteManager = new UniteManager;
    m_listner = new ContactListenner(uniteManager->GetListe());
    b2Vec2 gravity;
	gravity.Set(0.0f, -10.0f);
	m_world = new b2World(gravity);
	m_world->SetContactListener(m_listner);
	m_carte = new Carte(m_world, sf::Vector2i(800,200), 0);
    UniteTerrestreModel::GetInstance();


    InputManager imanager;
    joueurManager = new JoueurManager(*m_carte);
    uniteManager->SetJoueurManager(joueurManager);


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

        gengine->DrawScene();
        GererExplosions();
        uniteManager->Update();
    }
}
void GameEngine::GererExplosions()
{
    const std::vector<ExplosionPosition> &explosions = m_listner->GetExplosions();
    m_carte->DemarrerDestruction();
    for(const ExplosionPosition &exp : explosions)
    {
        m_carte->AjouterExplosion(exp.position, exp.radius);
    }
    m_carte->FinirDestruction();
    const std::vector<ExplosionImpusle>& impulses = m_listner->GetImpulsions();
    for(const ExplosionImpusle &impulse : impulses)
    {
        impulse.object->ApplyLinearImpulse(impulse.impulse, impulse.pts);
    }

    const std::vector<Projectile*> &toDestroy = m_listner->GetProjectileToDestroy();
    for(Projectile *projectile : toDestroy)
    {
        DeleteProjectile(projectile);
    }

    m_listner->Clear();
}
void GameEngine::DeleteProjectile(Projectile* toDelete)
{
    uniteManager->DeleteProjectile(toDelete);
}

void GameEngine::EnleverUnit(Unite* unite)
{
    uniteManager->DetruireUnite(unite);
}
void GameEngine::EnleverUnit(sf::Uint32 id)
{
    uniteManager->DetruireUnite(id);
}
