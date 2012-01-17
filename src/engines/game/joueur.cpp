#include "joueur.h"
#include <engines/graphics/graphicalengine.h>
#include "inputmanager.h"
#include "unite.h"

Joueur::Joueur(const sf::FloatRect &portionEcran)
{
    m_selectedUnit = 0;
    m_camera = GraphicalEngine::GetInstance()->GetCameraManager()->AddCamera();
    m_camera->SetViewport(portionEcran);
}

Joueur::~Joueur()
{
    GraphicalEngine::GetInstance()->GetCameraManager()->RemoveCamera(m_camera);
}

void Joueur::SetInput(const UnitInput &input)
{
    if(m_selectedUnit!=0)
        m_selectedUnit->Deplacer(input);
}

void Joueur::AjouterUnite(sf::Uint32 id, Unite* unit)
{
    m_unites[id] = unit;
}

void Joueur::EnleverUnite(sf::Uint32 id)
{
    auto it = m_unites.find(id);
    if(it==m_unites.end())
        return;
    m_unites.erase(it);
}

void Joueur::EnleverUnite(Unite* unite)
{
    for(auto &u : m_unites)
    {
        if(u.second==unite)
        {
            EnleverUnite(u.first);
            return;
        }
    }
}
void Joueur::SelectedFollowingUnit()
{
    if(m_selectedUnit==0)
    {
        SelectFirstUnit();
    }
    else
    {
        auto it = m_unites.find(m_idSelected);
        it++;
        if(it==m_unites.end())
            SelectFirstUnit();
        else
        {
            m_selectedUnit= it->second;
            m_idSelected  = it->first;
        }
    }
}
void Joueur::SelectFirstUnit()
{
    m_selectedUnit=m_unites.begin()->second;
    m_idSelected = m_unites.begin()->first;
}
void Joueur::SelectedUnit(sf::Uint32 id)
{
    for(auto &it : m_unites)
    {
        if(it.first==id)
        {
            m_selectedUnit = it.second;
            m_idSelected = it.first;
        }
    }
}
