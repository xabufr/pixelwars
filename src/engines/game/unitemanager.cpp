#include "unitemanager.h"
#include "joueurmanager.h"
#include "unite.h"
#include "joueur.h"
#include "projectile.h"

UniteManager::UniteManager(JoueurManager* j_manager): m_joueurManager(j_manager)
{
}

UniteManager::~UniteManager()
{
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
        }
    }
    for(Projectile *proj: m_projectiles)
    {
        proj->Update();
    }
    for(auto &it: m_unites)
    {
        it.second->Update();
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
            m_projectiles.erase(m_projectiles.begin()+i);
            delete proj;
        }
    }
}
void UniteManager::SetJoueurManager(JoueurManager *j_manager)
{
    m_joueurManager = j_manager;
}
