#include "joueurhumain.h"
#include "engines/graphics/graphicalengine.h"
#include "unite.h"

JoueurHumain::JoueurHumain(const sf::FloatRect& portion)
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
    sf::FloatRect viewport = m_camera->GetViewport();
    sf::Vector2f newSize(0,0);
    sf::Vector2f windowSize(m_app->GetWidth(), m_app->GetHeight());
    newSize.x = windowSize.x*viewport.Width;
    newSize.y = windowSize.y*viewport.Height;
    m_camera->SetSize(newSize);
    if(m_selectedUnit!=0)
    {
        m_camera->SetCenter(m_selectedUnit->GetBody()->GetPosition().x*10,
                            -m_selectedUnit->GetBody()->GetPosition().y*10);
    }
    else
    {
        m_camera->SetCenter(newSize/2.f);
    }
}
