#include "joueurhumain.h"
#include "../graphics/graphicalengine.h"
#include "unite.h"
#include "carte.h"

JoueurHumain::JoueurHumain(Carte &carte, int numero, const sf::FloatRect& portion): Joueur(carte, numero)
{
    m_camera = GraphicalEngine::GetInstance()->GetCameraManager()->AddCamera();
    m_camera->setViewport(portion);
    m_app = GraphicalEngine::GetInstance()->GetRenderWindow();
    m_zoom = 1.f;
    m_useSpecialZoom = false;
}

JoueurHumain::~JoueurHumain()
{
    GraphicalEngine::GetInstance()->GetCameraManager()->RemoveCamera(m_camera);
}
//Repositionner la caméra
void JoueurHumain::Update()
{
    const float tailleZone = 50.f;
    sf::FloatRect viewport = m_camera->getViewport();
    sf::Vector2f newSize(0,0);
    sf::Vector2f windowSize(m_app->getSize().x, m_app->getSize().y);
    newSize.x = windowSize.x*viewport.width;
    newSize.y = windowSize.y*viewport.height;
    m_camera->setSize(newSize);
    newSize.x*=m_zoom;
    newSize.y*=m_zoom;
    if(m_selectedUnit!=0)
    {
        sf::Vector2f ancienCentre(m_camera->getCenter());
        sf::Vector2f centre(m_selectedUnit->GetBody()->GetPosition().x*10,
                            -m_selectedUnit->GetBody()->GetPosition().y*10);
        sf::Vector2f move(0,0);
        if(abs(centre.x-ancienCentre.x)>tailleZone)
        {
            if(centre.x>ancienCentre.x)
                move.x = centre.x-ancienCentre.x-tailleZone;
            else
                move.x = centre.x-ancienCentre.x+tailleZone;
        }
        if(abs(centre.y-ancienCentre.y)>tailleZone)
        {
            if(centre.y>ancienCentre.y)
                move.y = centre.y-ancienCentre.y-tailleZone;
            else
                move.y = centre.y-ancienCentre.y+tailleZone;
        }
        if(ancienCentre.x+move.x-newSize.x*0.5<0)
        {
            move.x = -ancienCentre.x+newSize.x*0.5;
        }
        else if(ancienCentre.x+move.x+newSize.x*0.5>m_carte.Width())
        {
            move.x= m_carte.Width()-ancienCentre.x-newSize.x*0.5;
        }
        m_camera->move(move);

    }
    else
    {
        if(m_numero==0)
            m_camera->setCenter(m_xMinCamp+newSize.x*0.5,0);
        else
            m_camera->setCenter(m_xMaxCamp-newSize.x*0.5,0);
    }

    m_camera->zoom(m_zoom);
}
sf::View* JoueurHumain::GetCam() const
{
    return m_camera;
}
void JoueurHumain::Zoomer()
{
    if(m_app->getSize().x*m_camera->getViewport().width*m_zoom*2>=m_carte.Width()&&!m_useSpecialZoom)
    {
        m_useSpecialZoom=true;
        m_lastValidZoom=m_zoom;
        m_zoom*= (m_carte.Width())/(m_app->getSize().x*m_camera->getViewport().width*m_zoom);
    }
    else if(!m_useSpecialZoom)
        m_zoom*=2;
}
void JoueurHumain::Dezoomer()
{
    if(!m_useSpecialZoom)
    {
        if(m_zoom*0.5>=0.5)
            m_zoom*=0.5;
    }
    else
    {
        m_zoom = m_lastValidZoom;
        m_useSpecialZoom=false;
    }
}
void JoueurHumain::SetInput(const UnitInput& input)
{
    Joueur::SetInput(input);
    if(input.zoomer)
        Zoomer();
    else if(input.dezoomer)
        Dezoomer();
}
