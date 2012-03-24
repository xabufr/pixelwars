#include "nuage.h"
#include "../../core/random.h"
#include <vector>
#include <string>

Nuage::Nuage(const sf::Vector2f& pos, float maxY, int nb, float charge): m_size(0.f,0.f), m_charge(charge), m_maxY(maxY)
{
    m_node = GraphicalEngine::GetInstance()->GetSceneManager()->GetRootNode()->AddSceneNode();
    m_node->SetLevel(-9);
    m_node->SetAbsolutePosition(pos);

    m_nbParties = nb;
    m_partiesNuage = new SceneNodeSpriteItem*[nb];

    std::vector<std::string> listeParticules = std::vector<std::string>({"data/nuage_1.png",
                                                                        "data/nuage_2.png",
                                                                        "data/nuage_3.png",
                                                                        "data/nuage_4.png",
                                                                        "data/nuage_5.png"});
    size_t nbPartiePos = listeParticules.size();
    float last_x=0;
    for(int i(0);i<nb;++i)
    {
        m_partiesNuage[i] = new SceneNodeSpriteItem;
        m_node->AddItem(m_partiesNuage[i]);
        m_partiesNuage[i]->SetImage(listeParticules[Random::Rand(0, nbPartiePos)]);
        m_partiesNuage[i]->SetRelativePosition(last_x, -m_partiesNuage[i]->GetSize().y*0.5);
        last_x+= m_partiesNuage[i]->GetSize().x*0.5;
        if(m_partiesNuage[i]->GetSize().y>m_size.y)
            m_size.y = m_partiesNuage[i]->GetSize().y;
    }
    m_size.x = last_x + m_partiesNuage[nb-1]->GetSize().x*0.5;
    charge = (1-charge)*255;
    SetColor(sf::Color(charge, charge, charge));
    m_transitionVent = false;
    m_vitesse = 0.f;
    m_vent = 0;
    m_diffVent = 0;
    m_timerPluie.Restart();
    m_coefPluie=0.08;
}

Nuage::~Nuage()
{
    GraphicalEngine::GetInstance()->GetSceneManager()->RemoveNode(m_node);
}

void Nuage::Move(const sf::Vector2f& m)
{
    m_node->SetAbsolutePosition(m_node->GetAbsoluteInformations().position+m);
}
void Nuage::SetColor(const sf::Color& col)
{
    for(int i(0);i<m_nbParties;++i)
        m_partiesNuage[i]->SetColor(col);
}

const sf::Vector2f& Nuage::GetPosition() const
{
    return m_node->GetAbsoluteInformations().position;
}

const sf::Vector2f& Nuage::GetSize()
{
    return m_size;
}
void Nuage::SetPosition(const sf::Vector2f& pos)
{
    m_node->SetAbsolutePosition(pos);
}
void Nuage::SetX(float x)
{
    m_node->SetAbsolutePosition(x, m_node->GetAbsoluteInformations().position.y);
}
void Nuage::Work()
{
    if(m_transitionVent)
    {
        m_vitesse -= m_diffVent*m_timerVent.GetElapsedTime().AsSeconds();
        if((m_diffVent>0&&m_vent>=m_vitesse)||(m_diffVent<0&&m_vent<=m_vitesse))
        {
            m_transitionVent=false;
            m_vitesse=m_vent;
        }
    }
    sf::Vector2f dep(m_vitesse*m_timerVent.Restart().AsSeconds()*(((1-m_charge)>0.25)?(1-m_charge):0.25f), 0);
    Move(dep);

    m_GererPluie();
}
void Nuage::m_GererPluie()
{
    if(m_charge>0.33)
    {
        int goutesAAjouter = m_coefPluie*m_charge*m_timerPluie.GetElapsedTime().AsMilliseconds();
        for(int i(0);i<goutesAAjouter;++i)
        {
            SceneNodeSpriteItem *item = new SceneNodeSpriteItem;
            item->SetImage("data/goute.png");
            item->SetRelativePosition(Random::Rand(0.f,m_size.x),0);
            m_node->AddItem(item);
            m_goutes.push_back(item);
        }
        m_charge-= m_timerPluie.GetElapsedTime().AsSeconds()*0.005;
        float charge = (1-m_charge)*255;
        SetColor(sf::Color(charge, charge, charge));
    }
    for(auto it = m_goutes.begin();it!=m_goutes.end();)
    {
        if((*it)->GetRelativePosition().y+m_node->GetAbsoluteInformations().position.y<=m_maxY)
        {
            (*it)->SetRelativePosition((*it)->GetRelativePosition()+sf::Vector2f(0, 250*m_timerPluie.GetElapsedTime().AsSeconds()));
            ++it;
        }
        else
        {
            m_node->RemoveItem(*it);
            delete *it;
            it = m_goutes.erase(it);
        }
    }
    m_timerPluie.Restart();
}
void Nuage::SetWind(float w)
{
    m_diffVent = m_vent-w;
    m_vent = w;
    m_transitionVent=true;
}
