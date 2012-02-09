#include "menuoptions.h"
#include "engines/graphics/graphicalengine.h"
#include "../playerparameters.h"
#include "../../../core/logger.h"
MenuOptions::MenuOptions()
{
    m_contener = GraphicalEngine::GetInstance()->GetGuiManager()->GetRootNode()->AddContener();
    contJ1 = new GuiContener(GraphicalEngine::GetInstance()->GetGuiManager(), m_contener);
    contJ2 = new GuiContener(GraphicalEngine::GetInstance()->GetGuiManager(), m_contener);


    m_couleurJ1 = new GuiColorSelector(GraphicalEngine::GetInstance()->GetGuiManager(), contJ1);
    m_couleurJ1->SetColor(PlayerParameters::GetInstance()->GetParam(0).couleur);
    m_nameJ1 = new GuiTextInputItem;
    m_nameJ1->SetText(PlayerParameters::GetInstance()->GetParam(0).nom);
    m_nameJ1->SetBgColor(sf::Color(128,128,128));
    m_nameJ1->SetColor(sf::Color(0,0,0));
    m_couleurJ2 = new GuiColorSelector(GraphicalEngine::GetInstance()->GetGuiManager(), contJ2);
    m_couleurJ2->SetColor(PlayerParameters::GetInstance()->GetParam(1).couleur);
    m_nameJ2 = new GuiTextInputItem;
    m_nameJ2->SetText(PlayerParameters::GetInstance()->GetParam(1).nom);
    m_nameJ2->SetBgColor(sf::Color(128,128,128));
    m_nameJ2->SetColor(sf::Color(0,0,0));
    m_labelJ1 = new GuiTextItem;
    m_labelJ2 = new GuiTextItem;
    m_labelJ1->SetText("Joueur 1");
    m_labelJ2->SetText("Joueur 2");


    m_contener->AddSceneNode(contJ1);
    m_contener->AddSceneNode(contJ2);


    contJ1->AjouterItem(m_labelJ1, 1, 1);
    contJ1->AjouterItem(m_nameJ1, 1, 2);
    contJ1->AjouterItem(m_couleurJ1, 1, 3);

    contJ2->AjouterItem(m_labelJ2, 1, 1);
    contJ2->AjouterItem(m_nameJ2, 1, 2);
    contJ2->AjouterItem(m_couleurJ2, 1, 3);


    m_contener->AjouterItem(contJ1, 1, 0);
    m_contener->AjouterItem(contJ2, 2, 0);

    Afficher();
}

MenuOptions::~MenuOptions()
{
    PlayerParameters::GetInstance()->SetPlayerColor(m_couleurJ1->GetColor(), 0);
    PlayerParameters::GetInstance()->SetPlayerName(m_nameJ1->GetText(), 0);
    m_contener->Remove();
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
