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
#include "core/conversion.h"
#include "playerparameters.h"
#include "uniteaerienne.h"

GameEngine::GameEngine()
{
    m_lastId = 0;
    m_running=true;
    m_tailleCarte=100;
    m_seed = 0;
}

GameEngine::~GameEngine()
{
    DesalouerModel(m_windowJ1->GetContener());
    DesalouerModel(m_windowJ2->GetContener());
    m_gengine->GetGuiManager()->RemoveNode(m_windowJ1);
    m_gengine->GetGuiManager()->RemoveNode(m_windowJ2);

    delete m_joueurManager;
    delete m_uniteManager;
    delete m_carte;
    delete m_world;
}
void GameEngine::SetSeed(int seed)
{
    m_seed=seed;
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


    m_uniteManager = new UniteManager(this);
    m_listner = new ContactListenner(m_uniteManager->GetListe());
    b2Vec2 gravity;
	gravity.Set(0.0f, -10.0f);
	m_world = new b2World(gravity);
	m_world->SetContactListener(m_listner);
	m_carte = new Carte(m_world, sf::Vector2i(m_tailleCarte,600), 400, 100, m_seed);
    UniteTerrestreModel::GetInstance();
    m_gengine->GetSceneManager()->GetParticleManager()->SetWorld(m_world);


    m_joueurManager = new JoueurManager(*m_carte);
    m_uniteManager->SetJoueurManager(m_joueurManager);

    m_joueurManager->SetJoueur(0, TypeJoueur::JOUEUR_LOCAL);
    m_joueurManager->SetJoueur(1, TypeJoueur::JOUEUR_LOCAL);
    m_joueurManager->GetJoueur(0)->SetColor(PlayerParameters::GetInstance()->GetParam(0).couleur);
    m_joueurManager->GetJoueur(1)->SetColor(PlayerParameters::GetInstance()->GetParam(1).couleur);

    m_windowJ1 = m_gengine->GetGuiManager()->GetRootNode()->AddWindow();
    m_windowJ1->SetWindowTitle(PlayerParameters::GetInstance()->GetPlayerName(0));

    m_windowJ1->SetAbsolutePosition(0,0);
    m_scoreText[0] = new GuiTextItem;
    m_scoreText[1] = new GuiTextItem;
    m_scoreText[0]->SetText("Score: 10      ");
    m_scoreText[1]->SetText("Score: 10      ");
    m_score[0]=10;
    m_score[1]=10;

    m_windowJ2 = m_gengine->GetGuiManager()->GetRootNode()->AddWindow();
    m_windowJ2->SetWindowTitle(PlayerParameters::GetInstance()->GetPlayerName(1));

    m_windowJ1->GetContener()->AjouterItem(m_scoreText[0],1,0);
    m_windowJ2->GetContener()->AjouterItem(m_scoreText[1],1,0);

    LoadGuiModels(m_windowJ1, m_joueurManager->GetJoueur(0));
    LoadGuiModels(m_windowJ2, m_joueurManager->GetJoueur(1));
    m_windowJ2->SetAbsolutePosition(300,0);

    GuiButtonItem *b1, *b2;
    b1 = new GuiButtonItem;
    b2 = new GuiButtonItem;

    b1->SetText("Avion");
    b2->SetText("Avion");

    b1->SetData("this", this);
    b2->SetData("this", this);

    b1->SetData("joueur", m_joueurManager->GetJoueur(0));
    b2->SetData("joueur", m_joueurManager->GetJoueur(1));

    b1->SetCallBack("clicked", CallbackAjoutAvion);
    b2->SetCallBack("clicked", CallbackAjoutAvion);

    b1->SetNormalColor(sf::Color(0,0,0), sf::Color(0,0,0,0));
    b1->SetMouseOverColor(sf::Color(255,0,0), sf::Color(0,0,0,0));
    b2->SetNormalColor(sf::Color(0,0,0), sf::Color(0,0,0,0));
    b2->SetMouseOverColor(sf::Color(255,0,0), sf::Color(0,0,0,0));

    m_windowJ1->GetContener()->AjouterItem(b1, 0, 999);
    m_windowJ2->GetContener()->AjouterItem(b2, 0, 999);

}
void GameEngine::GererExplosions()
{
    const std::vector<ExplosionPosition> &explosions = m_listner->GetExplosions();
    m_carte->DemarrerDestruction();
    for(const ExplosionPosition &exp : explosions)
    {
        m_carte->AjouterExplosion(exp.position, exp.radius);
        ParticleParameters params;
        params.colorFromList=true;
        params.colorList.push_back(sf::Color(0,255,0));
        params.colorList.push_back(sf::Color(128,128,128));
        params.colorList.push_back(sf::Color(108,65,0));
        params.position=exp.position;
        params.number=exp.radius;
        params.timeToLive=1000;
        params.level=-1;
        params.minSize=1;
        params.maxSize=2;
        params.minPower=250;
        params.maxPower=500;
        params.minAngle=exp.angle-10;
        params.maxAngle=exp.angle+10;
        GraphicalEngine::GetInstance()->GetSceneManager()->GetParticleManager()->AddParticleSystem(params);
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
    m_uniteManager->DeleteProjectile(toDelete);
}

void GameEngine::EnleverUnit(Unite* unite)
{
    m_uniteManager->DetruireUnite(unite);
}
void GameEngine::EnleverUnit(sf::Uint32 id)
{
    m_uniteManager->DetruireUnite(id);
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
    game->m_uniteManager->AjouterUniteTerrestre(game->m_joueurManager->GetId(joueur), id, game->m_world, std::string((char*)item->GetData("model")));
}
void GameEngine::CallbackAjoutAvion(GuiItem *item)
{
    GameEngine *t = (GameEngine*)item->GetData("this");
    Joueur *j = (Joueur*)item->GetData("joueur");
    t->m_uniteManager->AjouterUnite(t->m_joueurManager->GetId(j), t->m_lastId++, new UniteAerienne(t->m_world, j->GetPositionNouvelleUnite(), t->m_joueurManager->GetId(j)));
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
    m_carte->Work();
    m_world->Step(1.f/60.f, 8, 3);
    m_gengine->DrawScene();
    GererExplosions();
    m_uniteManager->Update();
    m_joueurManager->Input(0, m_inputManager.GetAll());
    m_joueurManager->Input(1, m_inputManager.GetAll(1));
    m_joueurManager->Update();
}
void GameEngine::HandleEngineEvent(EngineEvent*)
{

}
void GameEngine::SendEndMessage()
{
    m_engineEvents.push_back(new EngineEvent);
    m_engineEvents.back()->SetMessage(TypeMessage::Quitter);
}
void GameEngine::ChangerScores()
{
    for(int i=0;i<2;++i)
    {
        m_scoreText[i]->SetText("Score: "+int2string(m_score[i]));
    }
}
void GameEngine::AddScore(int add, int index)
{
    m_score[index]+=add;
    m_score[(index==0?1:0)]-=add;
    ChangerScores();
}
bool GameEngine::CanWork() const
{
    return m_score[0]>=0&&m_score[1]>=0;
}
std::string GameEngine::GetWinner() const
{
    if(m_score[0]<m_score[1])
        return PlayerParameters::GetInstance()->GetPlayerName(1);
    return PlayerParameters::GetInstance()->GetPlayerName(0);
}
