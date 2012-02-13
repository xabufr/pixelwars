#include "menuoptions.h"
#include "engines/graphics/graphicalengine.h"
#include "../playerparameters.h"
#include "../../../core/logger.h"
#include "../../../core/conversion.h"
#include "menutouche.h"

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
    m_btn_tirer[0] = new GuiButtonItem;
    m_btn_tirer[1] = new GuiButtonItem;
    m_btn_zoom[0] = new GuiButtonItem;
    m_btn_zoom[1] = new GuiButtonItem;
    m_btn_dezoom[0] = new GuiButtonItem;
    m_btn_dezoom[1] = new GuiButtonItem;
    m_labelBas[0] = new GuiTextItem;
    m_labelBas[1] = new GuiTextItem;
    m_labelHaut[0] = new GuiTextItem;
    m_labelHaut[1] = new GuiTextItem;
    m_labelGauche[0] = new GuiTextItem;
    m_labelGauche[1] = new GuiTextItem;
    m_labelDroite[0] = new GuiTextItem;
    m_labelDroite[1] = new GuiTextItem;
    m_labelTirer[0] = new GuiTextItem;
    m_labelTirer[1] = new GuiTextItem;
    m_labelZoom[0] = new GuiTextItem;
    m_labelZoom[1] = new GuiTextItem;
    m_labelDezoom[0] = new GuiTextItem;
    m_labelDezoom[1] = new GuiTextItem;

    m_labelBas[0]->SetText("Bas");
    m_labelBas[1]->SetText("Bas");
    m_labelHaut[0]->SetText("Haut");
    m_labelHaut[1]->SetText("Haut");
    m_labelGauche[0]->SetText("Gauche");
    m_labelGauche[1]->SetText("Gauche");
    m_labelDroite[0]->SetText("Droite");
    m_labelDroite[1]->SetText("Droite");
    m_labelTirer[0]->SetText("Tirer");
    m_labelTirer[1]->SetText("Tirer");
    m_labelZoom[0]->SetText("Zoomer");
    m_labelZoom[1]->SetText("Zoomer");
    m_labelDezoom[0]->SetText("Dezoomer");
    m_labelDezoom[1]->SetText("Dezoomer");

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



    for(int i=0; i<2; ++i)
    {
        m_btn_bas[i]->SetData("this", this);
        m_btn_bas[i]->SetCallBack("clicked", CallbackChangerTouche);
        m_btn_haut[i]->SetData("this", this);
        m_btn_haut[i]->SetCallBack("clicked", CallbackChangerTouche);
        m_btn_gauche[i]->SetData("this", this);
        m_btn_gauche[i]->SetCallBack("clicked", CallbackChangerTouche);
        m_btn_droite[i]->SetData("this", this);
        m_btn_droite[i]->SetCallBack("clicked", CallbackChangerTouche);
        m_btn_tirer[i]->SetData("this", this);
        m_btn_tirer[i]->SetCallBack("clicked", CallbackChangerTouche);
        m_btn_zoom[i]->SetData("this", this);
        m_btn_zoom[i]->SetCallBack("clicked", CallbackChangerTouche);
        m_btn_dezoom[i]->SetData("this", this);
        m_btn_dezoom[i]->SetCallBack("clicked", CallbackChangerTouche);
        m_btn_bas[i]->SetText(PlayerParameters::GetInstance()->GetStringTouche("bas", i));
        m_btn_haut[i]->SetText(PlayerParameters::GetInstance()->GetStringTouche("haut", i));
        m_btn_gauche[i]->SetText(PlayerParameters::GetInstance()->GetStringTouche("gauche", i));
        m_btn_droite[i]->SetText(PlayerParameters::GetInstance()->GetStringTouche("droite", i));
        m_btn_tirer[i]->SetText(PlayerParameters::GetInstance()->GetStringTouche("tirer", i));
        m_btn_zoom[i]->SetText(PlayerParameters::GetInstance()->GetStringTouche("zoom", i));
        m_btn_dezoom[i]->SetText(PlayerParameters::GetInstance()->GetStringTouche("dezoom", i));

        btnToucheData data;
        data.index=i;

        data.cible="bas";
        data.item=m_btn_bas[i];
        m_dataBtnTouches[m_btn_bas[i]] = data;

        data.cible="haut";
        data.item=m_btn_haut[i];
        m_dataBtnTouches[m_btn_haut[i]] = data;

        data.cible="gauche";
        data.item=m_btn_gauche[i];
        m_dataBtnTouches[m_btn_gauche[i]] = data;

        data.cible="droite";
        data.item=m_btn_droite[i];
        m_dataBtnTouches[m_btn_droite[i]] = data;

        data.cible="tirer";
        data.item=m_btn_tirer[i];
        m_dataBtnTouches[m_btn_tirer[i]] = data;

        data.cible="zoom";
        data.item=m_btn_zoom[i];
        m_dataBtnTouches[m_btn_zoom[i]] = data;

        data.cible="dezoom";
        data.item=m_btn_dezoom[i];
        m_dataBtnTouches[m_btn_dezoom[i]] = data;
    }

    m_contener->SetEspacement(sf::Vector2f(20, 5));

    contJ1->AjouterItem(m_labelJ1, 1, 0);
    contJ1->AjouterItem(m_nameJ1, 1, 1);
    contJ1->AjouterItem(m_couleurJ1, 1, 2);
    contJ1->AjouterItem(m_labelHaut[0], 1, 3);
    contJ1->AjouterItem(m_btn_haut[0], 2, 3);
    contJ1->AjouterItem(m_labelBas[0], 1, 4);
    contJ1->AjouterItem(m_btn_bas[0], 2, 4);
    contJ1->AjouterItem(m_labelGauche[0], 1, 5);
    contJ1->AjouterItem(m_btn_gauche[0], 2, 5);
    contJ1->AjouterItem(m_labelDroite[0], 1, 6);
    contJ1->AjouterItem(m_btn_droite[0], 2, 6);
    contJ1->AjouterItem(m_labelTirer[0], 1, 7);
    contJ1->AjouterItem(m_btn_tirer[0], 2, 7);
    contJ1->AjouterItem(m_labelZoom[0], 1, 8);
    contJ1->AjouterItem(m_btn_zoom[0], 2, 8);
    contJ1->AjouterItem(m_labelDezoom[0], 1, 9);
    contJ1->AjouterItem(m_btn_dezoom[0], 2, 9);

    contJ2->AjouterItem(m_labelJ2, 1, 0);
    contJ2->AjouterItem(m_nameJ2, 1, 1);
    contJ2->AjouterItem(m_couleurJ2, 1, 2);
    contJ2->AjouterItem(m_labelHaut[1], 1, 3);
    contJ2->AjouterItem(m_btn_haut[1], 2, 3);
    contJ2->AjouterItem(m_labelBas[1], 1, 4);
    contJ2->AjouterItem(m_btn_bas[1], 2, 4);
    contJ2->AjouterItem(m_labelGauche[1], 1, 5);
    contJ2->AjouterItem(m_btn_gauche[1], 2, 5);
    contJ2->AjouterItem(m_labelDroite[1], 1, 6);
    contJ2->AjouterItem(m_btn_droite[1], 2, 6);
    contJ2->AjouterItem(m_labelTirer[1], 1, 7);
    contJ2->AjouterItem(m_btn_tirer[1], 1, 7);
    contJ2->AjouterItem(m_labelZoom[1], 1, 8);
    contJ2->AjouterItem(m_btn_zoom[1], 1, 8);
    contJ2->AjouterItem(m_labelDezoom[1], 1, 9);
    contJ2->AjouterItem(m_btn_dezoom[1], 1, 9);

    m_contener->AjouterItem(contJ1, 1, 0);
    m_contener->AjouterItem(contJ2, 2, 0);


    m_changerTouche = false;

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
        if(m_changerTouche)
        {
            m_changerTouche=false;
            m_contener->Hide();
            MenuTouche menuTouche;
            sf::Keyboard::Key keycode = menuTouche.GetTouche();
            m_contener->Show();
            PlayerParameters::GetInstance()->SetTouche(m_dataMenuTouches.cible, keycode, m_dataMenuTouches.index);
            m_dataMenuTouches.item->SetText(PlayerParameters::GetInstance()->GetStringTouche(m_dataMenuTouches.cible, m_dataMenuTouches.index));
        }
    }
}
void MenuOptions::LoadTouches()
{
    for(int i=0; i<2; ++i)
    {
        m_btn_bas[i]->SetText(PlayerParameters::GetInstance()->GetStringTouche("bas", i));
        m_btn_haut[i]->SetText(PlayerParameters::GetInstance()->GetStringTouche("haut", i));
        m_btn_gauche[i]->SetText(PlayerParameters::GetInstance()->GetStringTouche("gauche", i));
        m_btn_droite[i]->SetText(PlayerParameters::GetInstance()->GetStringTouche("droite", i));
    }
}
void MenuOptions::CallbackChangerTouche(GuiItem* item)
{
    MenuOptions *menu = (MenuOptions*) item->GetData("this");
    menu->CallMenuTouches(menu->m_dataBtnTouches[item]);
}
void MenuOptions::CallMenuTouches(btnToucheData param)
{
    m_dataMenuTouches = param;
    m_changerTouche=true;
}
