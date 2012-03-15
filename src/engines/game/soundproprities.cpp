#include "soundproprities.h"
#include <tinyxml.h>
#include "uniteterrestre.h"
#include "projectile.h"
#include "core/exception.h"
#include "core/logger.h"

template<>
SoundProprities *Singleton<SoundProprities>::m_singleton = 0;

SoundProprities::SoundProprities()
{
    TiXmlDocument fichier("data/sons/param.xml");
    if(!fichier.LoadFile())
        throw Exception("Impossible de charger le fichier: data/sons/param.xml");

    TiXmlElement *rootNode = fichier.RootElement();
    TiXmlNode *tirsNode = rootNode->FirstChild("tirs");
    TiXmlNode *explosionNode = rootNode->FirstChild("explosions");
    TiXmlNode *currNode = 0;
    while((currNode=explosionNode->IterateChildren("intervalle",currNode)))
    {
        AddIntervalle(currNode, m_projectilesSounds);
    }
    currNode = 0;
    while((currNode=tirsNode->IterateChildren("intervalle",currNode)))
    {
        AddIntervalle(currNode, m_unitesTerrestreSounds);
    }
}

SoundProprities::~SoundProprities()
{
    //dtor
}
std::string SoundProprities::GetProjectileSound(Projectile* proj) const
{
    for(Intervalle intervalle: m_projectilesSounds)
    {
        if(intervalle.min <= proj->GetPuissance()&&intervalle.max>=proj->GetPuissance())
        {
            return intervalle.chemin;
        }
    }
    return "";
}
std::string SoundProprities::GetUniteTerrestreSound(UniteTerrestre* unit) const
{
    for(Intervalle intervalle: m_unitesTerrestreSounds)
    {
        if(intervalle.min <= unit->GetTailleCanon()&&intervalle.max>=unit->GetTailleCanon())
        {
            return intervalle.chemin;
        }
    }
    return "";
}
void SoundProprities::AddIntervalle(TiXmlNode* node, std::vector<Intervalle>& tableau)
{
    Intervalle tmp;
    double min, max;
    tmp.chemin = node->ToElement()->Attribute("son");
    node->ToElement()->Attribute("min", &min);
    node->ToElement()->Attribute("max", &max);
    tmp.min=min;
    tmp.max=max;
    tableau.push_back(tmp);
}
std::string SoundProprities::GetUniteTerrestreDestructionSound(UniteTerrestre* unit) const
{
    return "test.wav";
}
