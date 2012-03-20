#include "ciel.h"
#include "nuage.h"

Ciel::Ciel(const sf::Vector2f& taille, float dayDuration)
{
    m_taille=taille;
    m_nodeCiel = GraphicalEngine::GetInstance()->GetSceneManager()->GetRootNode()->AddSceneNode();
    m_nodeCiel->SetLevel(-10);
    m_nodeCiel->SetAbsolutePosition(taille.x/2, taille.y/2);

    m_fondCiel = new SceneNodeCircleShapeItem;
    m_fondCielTransition = new SceneNodeCircleShapeItem;
    m_fondCiel->SetRadius(taille.x*2);
    m_fondCielTransition->SetRadius(taille.x*3);
    m_nodeCiel->AddItem(m_fondCiel);
    m_nodeCiel->AddItem(m_fondCielTransition);
    m_fondCiel->SetRelativePosition(-taille.x*2,-taille.x*2);
    m_fondCielTransition->SetRelativePosition(-taille.x*3,-taille.x*3);

    m_itemSoleil = new SceneNodeSpriteItem;
    m_itemLune = new SceneNodeSpriteItem;
    m_itemSoleil->SetImage("data/sun.png");
    m_itemLune->SetImage("data/lune.png");

    m_nodeCiel->AddItem(m_itemSoleil);
    m_nodeCiel->AddItem(m_itemLune);
    m_itemSoleil->SetRelativePosition((-m_itemSoleil->GetSize().x/2)+(taille.x/2),-m_itemSoleil->GetSize().y/2);
    m_itemLune->SetRelativePosition((m_itemLune->GetSize().x/2)-(taille.x*3/4),-m_itemLune->GetSize().y/2);

    m_dayDuration = dayDuration*60*1000;
    m_vent = 40;
    m_nbNuages = int(taille.x)/250;
}

Ciel::~Ciel()
{
    //dtor
}
void Ciel::Work()
{
    float ratio;
    int demiDay = m_dayDuration/2;
    int currTime = m_time.GetElapsedTime().AsMilliseconds()%m_dayDuration;
    m_itemSoleil->SetColor(sf::Color(255,225,0));
    if(currTime>demiDay)
    {
        m_fondCiel->SetColor(sf::Color(0,0,0));//Nuit
    }
    else //Calcul du dégradé jour
    {
        if(currTime<demiDay/4)
        {
            if(currTime<demiDay/8)
            {
                ratio = float(currTime)/float(demiDay/8);
                m_fondCiel->SetColor(sf::Color(255*ratio,121*ratio,31*ratio, 255*ratio));
                m_fondCielTransition->SetColor(sf::Color(255*ratio,121*ratio,31*ratio, 128*ratio));
                ratio*=0.5;
                m_itemSoleil->SetColor(sf::Color(255,76+149*ratio,0));
            }
            else
            {
                ratio = float(currTime-(demiDay/8))/float(demiDay/8);
                m_fondCiel->SetColor(sf::Color(255*(1-ratio),70*(ratio)+121,224*ratio+31));
                m_fondCielTransition->SetColor(sf::Color(255*(1-ratio),70*(ratio)+121,224*ratio+31, 128));
                ratio=ratio*0.5+0.5;
                m_itemSoleil->SetColor(sf::Color(255,76+149*ratio,0));
            }
        }
        else if(currTime>demiDay*3/4)
        {
            if(currTime>demiDay*7/8)
            {
                ratio = float(currTime-(demiDay*7/8))/float(demiDay/8);
                m_fondCiel->SetColor(sf::Color(255*(1-ratio),121*(1-ratio),31*(1-ratio)));
                m_fondCielTransition->SetColor(sf::Color(255*(1-ratio),121*(1-ratio),31*(1-ratio), 128*(1-ratio)));
                ratio=ratio*0.5+0.5;
                m_itemSoleil->SetColor(sf::Color(255,76+149*(1-ratio),0));
            }
            else
            {
                ratio = float(currTime-(demiDay*3/4))/float(demiDay/8);
                m_fondCiel->SetColor(sf::Color(255*(ratio),70*(1-ratio)+121,224*(1-ratio)+31));
                m_fondCielTransition->SetColor(sf::Color(255*(ratio),70*(1-ratio)+121,224*(1-ratio)+31, 128));
                ratio*=0.5;
                m_itemSoleil->SetColor(sf::Color(255,76+149*(1-ratio),0));
            }

        }
        else
        {
            m_fondCiel->SetColor(sf::Color(0,191,255));
            m_fondCielTransition->SetColor(sf::Color(0,191,255, 128));
            m_itemSoleil->SetColor(sf::Color(255,225,0));
        }
    }
    ratio = float(m_time.GetElapsedTime().AsMilliseconds()%m_dayDuration)/float(m_dayDuration);
    m_nodeCiel->SetAbsoluteRotation(180+360*ratio);
    m_GererNuages();
}
void Ciel::m_GererNuages()
{
    for(auto i=m_nuages.begin(); i!=m_nuages.end();)
    {
        (*i)->Update();
        if((*i)->GetPosition().x>m_taille.x)
        {
            delete (*i);
            i=m_nuages.erase(i);
        }
        else
        {
            ++i;
        }
    }
    sf::Vector2f dep(m_vent*m_timerDep.Restart().AsSeconds(), 0);
    for(Nuage *n : m_nuages)
    {
        n->Move(dep);
    }

    while(m_nbNuages>m_nuages.size())
    {
        m_nuages.push_back(new Nuage(sf::Vector2f(Random::Rand(0.f, m_taille.x), Random::Rand(int(-m_taille.y), int(-m_taille.y*4))), Random::Rand(1, 7)));
    }

}
