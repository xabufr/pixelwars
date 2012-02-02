#include "gameengine.h"
#include "engines/graphics/graphicalengine.h"
#include "core/logger.h"
#include <SFML2/System.hpp>
#include <Box2D/Box2D.h>
#include "uniteterrestre.h"
#include "projectile.h"
#include "uniteterrestremodel.h"
#include "joueurhumain.h"
#include "../engineevent.h"

GameEngine::GameEngine()
{
    m_lastId = 0;
    m_running=true;
    m_tailleCarte=100;
}

GameEngine::~GameEngine()
{
    DesalouerModel(m_windowJ1->GetContener());
    DesalouerModel(m_windowJ2->GetContener());
    m_gengine->GetGuiManager()->RemoveNode(m_windowJ1);
    m_gengine->GetGuiManager()->RemoveNode(m_windowJ2);

    delete joueurManager;
    delete uniteManager;
    delete m_carte;
    delete m_world;
}
EngineType GameEngine::GetEngineId() const
{
    return EngineType::Game_engine;
}
void GameEngine::SetTailleMap(int taille)
{
    m_tailleCarte=taille;
}
void GameEngine::Start()
{
    m_gengine = GraphicalEngine::GetInstance();
    m_app = m_gengine->GetRenderWindow();


    uniteManager = new UniteManager(this);
    m_listner = new ContactListenner(uniteManager->GetListe());
    b2Vec2 gravity;
	gravity.Set(0.0f, -10.0f);
	m_world = new b2World(gravity);
	m_world->SetContactListener(m_listner);
	m_carte = new Carte(m_world, sf::Vector2i(m_tailleCarte,600), 400, 100, 0);
    UniteTerrestreModel::GetInstance();


    joueurManager = new JoueurManager(*m_carte);
    uniteManager->SetJoueurManager(joueurManager);

    joueurManager->SetJoueur(0, TypeJoueur::JOUEUR_LOCAL);
    joueurManager->SetJoueur(1, TypeJoueur::JOUEUR_LOCAL);
    joueurManager->GetJoueur(0)->SetColor(sf::Color(255,255,0));
    joueurManager->GetJoueur(1)->SetColor(sf::Color(0,0,255));

    m_windowJ1 = m_gengine->GetGuiManager()->GetRootNode()->AddWindow();
    m_windowJ1->SetWindowTitle("Joueur 1");
    LoadGuiModels(m_windowJ1, joueurManager->GetJoueur(0));
    m_windowJ1->SetAbsolutePosition(0,0);

    m_windowJ2 = m_gengine->GetGuiManager()->GetRootNode()->AddWindow();
    m_windowJ2->SetWindowTitle("Joueur 2");
    LoadGuiModels(m_windowJ2, joueurManager->GetJoueur(1));
    m_windowJ2->SetAbsolutePosition(300,0);

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

void GameEngine::Work()
{
    sf::Event event;
    while(m_app->PollEvent(event))
    {
        m_gengine->GetGuiManager()->HandleEvent(event);
        if(event.Type==sf::Event::Closed)
            SendEndMessage();
        if(event.Type==sf::Event::KeyReleased)
        {
            if(event.Key.Code == sf::Keyboard::Key::Escape)
            {
                SendEndMessage();
            }
        }
        m_inputManager.HandleEvent(event);
    }


    m_world->Step(1.f/60.f, 8, 3);
    m_gengine->DrawScene();
    GererExplosions();
    uniteManager->Update();
    joueurManager->Input(0, m_inputManager.GetAll());
    joueurManager->Input(1, m_inputManager.GetAll(1));
    joueurManager->Update();
}
void GameEngine::HandleEngineEvent(EngineEvent*)
{

}
void GameEngine::SendEndMessage()
{
    m_engineEvents.push_back(new EngineEvent);
    m_engineEvents.back()->SetMessage(TypeMessage::Quitter);
}
