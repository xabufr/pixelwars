#include "unitemanager.h"
#include "joueurmanager.h"
#include "unite.h"
#include "joueur.h"
#include "projectile.h"
#include "uniteterrestre.h"
#include <Box2D/Box2D.h>
#include "engines/type.h"
#include "engines/engineevent.h"
#include "gameengine.h"
#include "soundproprities.h"

UniteManager::UniteManager(GameEngine* g_engine, JoueurManager* j_manager): m_joueurManager(j_manager), m_engine(g_engine)
{
}

UniteManager::~UniteManager()
{
    for(auto &it: m_unites)
        delete it.second;
    for(Projectile *p: m_projectiles)
        delete p;
    SoundProprities::Kill();
}
void UniteManager::EnleverUnite(sf::Uint32 id)
{
    m_joueurManager->SupprimerUnite(m_unites[id]);
    m_unites.erase(m_unites.find(id));
}
void UniteManager::EnleverUnite(Unite* unit)
{
    for(auto &it: m_unites)
    {
        if(it.second==unit)
        {
            EnleverUnite(it.first);
            return;
        }
    }
}
void UniteManager::DetruireUnite(sf::Uint32 id)
{
    delete m_unites[id];
    EnleverUnite(id);
}
void UniteManager::DetruireUnite(Unite* unit)
{
    for(auto it : m_unites)
    {
        if(it.second==unit)
        {
            DetruireUnite(it.first);
            return;
        }
    }
}

void UniteManager::AjouterUnite(int joueur, sf::Uint32 id, Unite* unit)
{
    AjouterUnite(id,unit);
    m_joueurManager->AjouterUnite(joueur, id, unit);
}
void UniteManager::AjouterUnite(sf::Uint32 id, Unite* unit)
{
    m_unites[id] = unit;
}
void UniteManager::AjouterUniteTerrestre(int joueur, sf::Uint32 id, b2World* world, const std::string& uid)
{
    UniteTerrestre *nouvelleUnit = new UniteTerrestre(world, m_joueurManager->GetJoueur(joueur)->GetPositionNouvelleUnite()+b2Vec2(0, 10), uid);
    AjouterUnite(joueur, id, nouvelleUnit);
}
Unite* UniteManager::GetUnite(sf::Uint32 id)
{
    return m_unites[id];
}
void UniteManager::Update()
{
    for(int i=0;i<2;++i)
    {
        Unite* unit = m_joueurManager->GetJoueur(i)->GetSelectedUnit();
        if(unit&&unit->PeutTirer())
        {
            m_projectiles.push_back(unit->Tirer());
            EngineEvent *eventSon = new EngineEvent;
            eventSon->To(EngineType::Audio_engine);
            eventSon->AddString(IndexMessages::Chemin, SoundProprities::GetInstance()->GetUniteTerrestreSound((UniteTerrestre*)unit));
            eventSon->SetMessage(TypeMessage::JouerSon);
            m_engine->AddEvent(eventSon);
        }
    }
    for(Projectile *proj: m_projectiles)
    {
        proj->Update();
    }
    for(auto it=m_unites.begin();it!=m_unites.end();)
    {
        if(it->second->EstVivant())
        {
            it->second->Update();
            it++;
        }
        else
        {
            m_joueurManager->SupprimerUnite(it->second);
            delete it->second;
            it = m_unites.erase(it);
        }
    }
}
const std::unordered_map<sf::Uint32, Unite*>& UniteManager::GetListe() const
{
    return m_unites;
}

void UniteManager::DeleteProjectile(Projectile* proj)
{
    size_t taille = m_projectiles.size();
    for(size_t i=0;i<taille;++i)
    {
        if(m_projectiles[i]==proj)
        {
            EngineEvent *eventSon = new EngineEvent;
            eventSon->To(EngineType::Audio_engine);
            eventSon->AddString(IndexMessages::Chemin, SoundProprities::GetInstance()->GetProjectileSound(proj));
            eventSon->SetMessage(TypeMessage::JouerSon);
            m_engine->AddEvent(eventSon);

            m_projectiles.erase(m_projectiles.begin()+i);
            delete proj;

            return;
        }
    }
}
void UniteManager::SetJoueurManager(JoueurManager *j_manager)
{
    m_joueurManager = j_manager;
}
