#include "joueur.h"
#include "inputmanager.h"
#include "unite.h"
#include "carte.h"
#include "uniteterrestremodel.h"

Joueur::Joueur(Carte &carte, int numero): m_carte(carte)
{
    m_selectedUnit = 0;
    m_tailleCamp = 250;
    if(numero==0)
    {
        m_xMinCamp = 0;
        m_xMaxCamp = m_tailleCamp;
    }
    else
    {
        m_xMaxCamp = m_carte.Width();
        m_xMaxCamp = m_carte.Width()-m_tailleCamp;
    }
}

Joueur::~Joueur()
{
}

void Joueur::SetInput(const UnitInput &input)
{
    if(m_selectedUnit!=0)
        m_selectedUnit->Deplacer(input);
}

void Joueur::AjouterUnite(sf::Uint32 id, Unite* unit)
{
    m_unites[id] = unit;
    if(m_selectedUnit==0)
        SelectFirstUnit();
}
void Joueur::EnleverUnite(sf::Uint32 id)
{
    auto it = m_unites.find(id);
    if(it==m_unites.end())
        return;
    m_unites.erase(it);
    if(m_idSelected==id)
    {
        m_idSelected=0;
        m_selectedUnit=0;
    }
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
Unite* Joueur::GetSelectedUnit() const
{
    return m_selectedUnit;
}
b2Vec2 Joueur::GetPositionNouvelleUnite() const
{
    b2Vec2 toReturn;
    toReturn.x=m_xMaxCamp-m_xMinCamp;
    float tailleUnite = UniteTerrestreModel::GetInstance()->GetTailleXTotale()/2.f;
    float tailleRoues = UniteTerrestreModel::GetInstance()->GetTailleRoue();
    toReturn.y = -(m_carte.YMin(toReturn.x-tailleUnite-tailleRoues, toReturn.x+tailleUnite+tailleRoues)-tailleRoues*40);
    std::cout << toReturn.y << std::endl;
    return 0.1*toReturn;
}
