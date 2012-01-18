#include "joueurhumain.h"
#include "engines/graphics/graphicalengine.h"
#include "unite.h"
#include "carte.h"

JoueurHumain::JoueurHumain(Carte &carte, const sf::FloatRect& portion): Joueur(carte)
{
    m_camera = GraphicalEngine::GetInstance()->GetCameraManager()->AddCamera();
    m_camera->SetViewport(portion);
    m_app = GraphicalEngine::GetInstance()->GetRenderWindow();
}

JoueurHumain::~JoueurHumain()
{
    GraphicalEngine::GetInstance()->GetCameraManager()->RemoveCamera(m_camera);
}
//Repositionner la caméra
void JoueurHumain::Update()
{
    const float tailleZone = 50.f;
    sf::FloatRect viewport = m_camera->GetViewport();
    sf::Vector2f newSize(0,0);
    sf::Vector2f windowSize(m_app->GetWidth(), m_app->GetHeight());
    newSize.x = windowSize.x*viewport.Width;
    newSize.y = windowSize.y*viewport.Height;
    m_camera->SetSize(newSize);
    if(m_selectedUnit!=0)
    {
        sf::Vector2f ancienCentre(m_camera->GetCenter());
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
        m_camera->Move(move);

    }
    else
    {
        m_camera->SetCenter(newSize/2.f);
    }
}
