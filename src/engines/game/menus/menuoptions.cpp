#include "menuoptions.h"
#include "engines/graphics/graphicalengine.h"
#include "../playerparameters.h"
#include "../../../core/logger.h"
#include "../../../core/conversion.h"

MenuOptions::MenuOptions()
{
    m_contener = GraphicalEngine::GetInstance()->GetGuiManager()->GetRootNode()->AddContener();
    contJ1 = new GuiContener(GraphicalEngine::GetInstance()->GetGuiManager(), m_contener);
    contJ2 = new GuiContener(GraphicalEngine::GetInstance()->GetGuiManager(), m_contener);


    m_couleurJ1 = new GuiColorSelector(GraphicalEngine::GetInstance()->GetGuiManager(), contJ1);
    m_couleurJ2 = new GuiColorSelector(GraphicalEngine::GetInstance()->GetGuiManager(), contJ2);
    m_nameJ1 = new GuiTextInputItem;
    m_nameJ2 = new GuiTextInputItem;
    m_labelJ1 = new GuiTextItem;
    m_labelJ2 = new GuiTextItem;
    m_btn_bas[0] = new GuiButtonItem;
    m_btn_bas[1] = new GuiButtonItem;
    m_btn_haut[0] = new GuiButtonItem;
    m_btn_haut[1] = new GuiButtonItem;
    m_btn_droite[0] = new GuiButtonItem;
    m_btn_droite[1] = new GuiButtonItem;
    m_btn_gauche[0] = new GuiButtonItem;
    m_btn_gauche[1] = new GuiButtonItem;
    m_labelBas[0] = new GuiTextItem;
    m_labelBas[1] = new GuiTextItem;
    m_labelHaut[0] = new GuiTextItem;
    m_labelHaut[1] = new GuiTextItem;
    m_labelGauche[0] = new GuiTextItem;
    m_labelGauche[1] = new GuiTextItem;
    m_labelDroite[0] = new GuiTextItem;
    m_labelDroite[1] = new GuiTextItem;

    m_labelBas[0]->SetText("Bas");
    m_labelBas[1]->SetText("Bas");
    m_labelHaut[0]->SetText("Haut");
    m_labelHaut[1]->SetText("Haut");
    m_labelGauche[0]->SetText("Gauche");
    m_labelGauche[1]->SetText("Gauche");
    m_labelDroite[0]->SetText("Droite");
    m_labelDroite[1]->SetText("Droite");


    m_couleurJ1->SetColor(PlayerParameters::GetInstance()->GetParam(0).couleur);
    m_couleurJ2->SetColor(PlayerParameters::GetInstance()->GetParam(1).couleur);

    m_nameJ1->SetText(PlayerParameters::GetInstance()->GetParam(0).nom);
    m_nameJ1->SetBgColor(sf::Color(128,128,128));
    m_nameJ1->SetColor(sf::Color(0,0,0));

    m_nameJ2->SetText(PlayerParameters::GetInstance()->GetParam(1).nom);
    m_nameJ2->SetBgColor(sf::Color(128,128,128));
    m_nameJ2->SetColor(sf::Color(0,0,0));

    m_labelJ1->SetText("Joueur 1");
    m_labelJ2->SetText("Joueur 2");

    m_contener->SetEspacement(sf::Vector2f(20, 5));
    m_contener->AddSceneNode(contJ1);
    m_contener->AddSceneNode(contJ2);


    contJ1->AjouterItem(m_labelJ1, 1, 1);
    contJ1->AjouterItem(m_nameJ1, 1, 2);
    contJ1->AjouterItem(m_couleurJ1, 1, 3);
    contJ1->AjouterItem(m_labelHaut[0], 1, 4);
    contJ1->AjouterItem(m_btn_haut[0], 2, 4);
    contJ1->AjouterItem(m_labelBas[0], 1, 5);
    contJ1->AjouterItem(m_btn_bas[0], 2, 5);
    contJ1->AjouterItem(m_labelGauche[0], 1, 6);
    contJ1->AjouterItem(m_btn_gauche[0], 2, 6);
    contJ1->AjouterItem(m_labelDroite[0], 1, 7);
    contJ1->AjouterItem(m_btn_droite[0], 2, 7);

    contJ2->AjouterItem(m_labelJ2, 1, 1);
    contJ2->AjouterItem(m_nameJ2, 1, 2);
    contJ2->AjouterItem(m_couleurJ2, 1, 3);
    contJ2->AjouterItem(m_labelHaut[1], 1, 4);
    contJ2->AjouterItem(m_btn_haut[1], 2, 4);
    contJ2->AjouterItem(m_labelBas[1], 1, 5);
    contJ2->AjouterItem(m_btn_bas[1], 2, 5);
    contJ2->AjouterItem(m_labelGauche[1], 1, 6);
    contJ2->AjouterItem(m_btn_gauche[1], 2, 6);
    contJ2->AjouterItem(m_labelDroite[1], 1, 7);
    contJ2->AjouterItem(m_btn_droite[1], 2, 7);



    m_contener->AjouterItem(contJ1, 1, 0);
    m_contener->AjouterItem(contJ2, 2, 0);

    Logger::Log()<<key2string(sf::Keyboard::Key::F5) << " " << key2string(sf::Keyboard::Key::F12) <<  " " << key2string(sf::Keyboard::Key::Numpad2) << Logger::endl;

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
