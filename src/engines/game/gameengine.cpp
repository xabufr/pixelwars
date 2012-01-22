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
    m_lastId = 0;
    m_running=true;
}

GameEngine::~GameEngine()
{
    delete joueurManager;
    delete uniteManager;
    delete m_carte;
    delete m_world;
}
EngineType GameEngine::GetEngineId() const
{
    return EngineType::Game_engine;
}
void GameEngine::Start()
{
    GraphicalEngine* gengine = GraphicalEngine::GetInstance();
    sf::RenderWindow *app = gengine->GetRenderWindow();

    uniteManager = new UniteManager;
    m_listner = new ContactListenner(uniteManager->GetListe());
    b2Vec2 gravity;
	gravity.Set(0.0f, -10.0f);
	m_world = new b2World(gravity);
	m_world->SetContactListener(m_listner);
	m_carte = new Carte(m_world, sf::Vector2i(800,600), 400, 100, 0);
    UniteTerrestreModel::GetInstance();


    InputManager imanager;
    joueurManager = new JoueurManager(*m_carte);
    uniteManager->SetJoueurManager(joueurManager);

    joueurManager->SetJoueur(0, TypeJoueur::JOUEUR_LOCAL);
    joueurManager->SetJoueur(1, TypeJoueur::JOUEUR_LOCAL);
    joueurManager->GetJoueur(0)->SetColor(sf::Color(255,255,0));
    joueurManager->GetJoueur(1)->SetColor(sf::Color(0,0,255));


    sf::Event event;
    GuiWindowNode *windowJ1 = GraphicalEngine::GetInstance()->GetGuiManager()->GetRootNode()->AddWindow();
    windowJ1->SetWindowTitle("Joueur 1");
    LoadGuiModels(windowJ1, joueurManager->GetJoueur(0));
    windowJ1->SetAbsolutePosition(0,0);

    GuiWindowNode *windowJ2 = GraphicalEngine::GetInstance()->GetGuiManager()->GetRootNode()->AddWindow();
    windowJ2->SetWindowTitle("Joueur 2");
    LoadGuiModels(windowJ2, joueurManager->GetJoueur(1));
    windowJ2->SetAbsolutePosition(300,0);

    while(m_running)
    {
        while(app->PollEvent(event))
        {
            gengine->GetGuiManager()->HandleEvent(event);
            if(event.Type==sf::Event::Closed)
                m_running=false;
            if(event.Type==sf::Event::KeyReleased)
            {
                if(event.Key.Code == sf::Keyboard::Key::Escape)
                {
                    m_running=false;
                }
            }
            imanager.HandleEvent(event);
        }


        m_world->Step(1.f/60.f, 8, 3);
        gengine->DrawScene();
        GererExplosions();
        uniteManager->Update();
        joueurManager->Input(0, imanager.GetAll());
        joueurManager->Input(1, imanager.GetAll(1));
        joueurManager->Update();
    }
    DesalouerModel(windowJ1->GetContener());
    DesalouerModel(windowJ2->GetContener());
    gengine->GetGuiManager()->RemoveNode(windowJ1);
    gengine->GetGuiManager()->RemoveNode(windowJ2);
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
void GameEngine::LoadGuiModels(GuiWindowNode* window, Joueur* joueur)
{
    GuiWindowContener* contener = window->GetContener();
    TiXmlDocument fichier("data/units/land.xml");
    if(!fichier.LoadFile())
        throw Exception("Impossible de charger le fichier: data/units/land.xml");
    TiXmlElement *rootNode = fichier.RootElement();
    TiXmlNode *model = 0;
    while((model=rootNode->IterateChildren("model",model)))
    {
        AddModel(contener, joueur, model);
    }
}
void GameEngine::AddModel(GuiWindowContener* contener, Joueur* joueur, TiXmlNode* node)
{
    char *modelUid;
    const char* nodeUid = node->ToElement()->Attribute("uid");
    const size_t taille = strlen(nodeUid);
    modelUid=new char[taille];
    strcpy(modelUid, nodeUid);

    GuiButtonItem* btn = new GuiButtonItem;
    btn->SetText(node->ToElement()->Attribute("name"));
    btn->SetNormalColor(sf::Color(0,0,0), sf::Color(0,0,0,0));
    btn->SetMouseOverColor(sf::Color(255,0,0), sf::Color(0,0,0,0));
    contener->AjouterItem(btn, 0, 999);
    btn->SetData("game", this);
    btn->SetData("joueur", joueur);
    btn->SetData("model", modelUid);
    btn->SetCallBack("clicked", CallbackAjoutUnite);
}
void GameEngine::DesalouerModel(GuiWindowContener* contener)
{
    const std::vector<SceneNodeItem*>& btns = contener->GetChildItems();
    for(SceneNodeItem* item : btns)
    {
        delete[] (char*)((GuiButtonItem*)item)->GetData("model");
    }
}
void GameEngine::CallbackAjoutUnite(GuiItem* item)
{
    GameEngine* game = (GameEngine*)item->GetData("game");
    Joueur* joueur = (Joueur*)item->GetData("joueur");
    sf::Uint32 id = game->m_lastId++;
    game->uniteManager->AjouterUniteTerrestre(game->joueurManager->GetId(joueur), id, game->m_world, std::string((char*)item->GetData("model")));
}
