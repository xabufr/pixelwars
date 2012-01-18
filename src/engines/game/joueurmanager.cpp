#include "joueurmanager.h"
#include "carte.h"
#include "joueur.h"
#include "joueurhumain.h"

JoueurManager::JoueurManager(Carte& carte): m_carte(carte)
{
    m_joueurs[0]=0;
    m_joueurs[1]=0;
}

JoueurManager::~JoueurManager()
{
    if(m_joueurs[0]!=0)
        delete m_joueurs[0];
    if(m_joueurs[1]!=0)
        delete m_joueurs[1];
}
void JoueurManager::SetJoueur(int num, TypeJoueur type)
{
    switch(type)
    {
    case JOUEUR_LOCAL:
        m_joueurs[num] = new JoueurHumain(m_carte);
    case JOUEUR_IA:
    case JOUEUR_DISTANT:
        break;
    }
}
void JoueurManager::AjouterUnite(int id_j, sf::Uint32 id_u, Unite* unite)
{
    m_joueurs[id_j]->AjouterUnite(id_u, unite);
    m_unitesJoueur[unite]=m_joueurs[id_j];
}
void JoueurManager::SupprimerUnite(Unite* unit)
{
    auto it = m_unitesJoueur.find(unit);
    it->second->EnleverUnite(unit);
    m_unitesJoueur.erase(it);
}
void JoueurManager::Update()
{
    if(m_joueurs[0]!=0)
        m_joueurs[0]->Update();
    if(m_joueurs[1]!=0)
        m_joueurs[1]->Update();
}
Joueur* JoueurManager::GetJoueur(int id)
{
    return m_joueurs[id];
}
void JoueurManager::Input(int id, const UnitInput& input)
{
    m_joueurs[id]->SetInput(input);
}
