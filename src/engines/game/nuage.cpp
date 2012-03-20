#include "nuage.h"
#include "../../core/random.h"
#include <vector>
#include <string>

Nuage::Nuage(const sf::Vector2f& pos, int nb)
{
    m_node = GraphicalEngine::GetInstance()->GetSceneManager()->GetRootNode()->AddSceneNode();
    m_node->SetLevel(-9);
    m_node->SetAbsolutePosition(pos);

    m_nbParties = nb;
    m_partiesNuage = new SceneNodeSpriteItem*[nb];

    std::vector<std::string> listeParticules = std::vector<std::string>({"data/nuage_1.png",
                                                                        "data/nuage_2.png",
                                                                        "data/nuage_3.png"});
    size_t nbPartiePos = listeParticules.size();
    float last_x=0;
    for(int i(0);i<nb;++i)
    {
        m_partiesNuage[i] = new SceneNodeSpriteItem;
        m_node->AddItem(m_partiesNuage[i]);
        m_partiesNuage[i]->SetImage(listeParticules[Random::Rand(0, nbPartiePos-1)]);
        m_partiesNuage[i]->SetRelativePosition(last_x, -m_partiesNuage[i]->GetSize().y*0.5);
        last_x+= m_partiesNuage[i]->GetSize().x*0.5;
    }
    m_transitionTime = 2500;
    m_timer.Restart();
    Update();
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
void Nuage::Update()
{
    int currTime = m_timer.GetElapsedTime().AsMilliseconds();
    if(currTime<=m_transitionTime)
    {
        SetColor(sf::Color(255,255,255,(1.f-float(m_transitionTime-currTime)/float(m_transitionTime))*255));
    }
    else
    {
        SetColor(sf::Color(255,255,255,255));
    }
}
