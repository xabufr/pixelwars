#include "menuoptions.h"
#include "engines/graphics/graphicalengine.h"
#include "../playerparameters.h"

MenuOptions::MenuOptions()
{
    m_contener = GraphicalEngine::GetInstance()->GetGuiManager()->GetRootNode()->AddContener();
    m_couleurJ1 = (GuiColorSelector*)m_contener->AddSceneNode((SceneNode*)new GuiColorSelector(GraphicalEngine::GetInstance()->GetGuiManager(), m_contener));
    m_couleurJ1->SetColor(PlayerParameters::GetInstance()->GetParam(0).couleur);
    m_nameJ1 = new GuiTextInputItem;
    m_nameJ1->SetText(PlayerParameters::GetInstance()->GetParam(0).nom);
    m_nameJ1->SetBgColor(sf::Color(128,128,128));
    m_nameJ1->SetColor(sf::Color(0,0,0));
    m_nameJ1->SetAbsolutePosition(200, 400);

    m_contener->AddItem(m_nameJ1);
    Afficher();
}

MenuOptions::~MenuOptions()
{
    PlayerParameters::GetInstance()->SetPlayerColor(m_couleurJ1->GetColor(), 0);
    PlayerParameters::GetInstance()->SetPlayerName(m_nameJ1->GetText(), 0);
    GraphicalEngine::GetInstance()->GetGuiManager()->RemoveNode(m_contener);
}
void MenuOptions::Afficher()
{
    sf::Event event;
    GraphicalEngine *engine = GraphicalEngine::GetInstance();
    sf::RenderWindow *app = engine->GetRenderWindow();
    bool cont = true;
    while(cont)
    {
        while(app->PollEvent(event))
        {
            engine->GetGuiManager()->HandleEvent(event);
            if(event.Type==sf::Event::Closed)
                cont = false;
        }
        GraphicalEngine::GetInstance()->DrawScene();
    }
}
