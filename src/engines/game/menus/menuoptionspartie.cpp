#include "menuoptionspartie.h"
#include "engines/graphics/graphicalengine.h"
#include "core/conversion.h"
#include "../gameengine.h"
#include "game.h"
MenuOptionsPartie::MenuOptionsPartie(TypePartie type)
{
    m_tailleCarte=2000;
    m_start=false;
    if(type==TypePartie::ECRAN_SCINDE)
        StartMenuEcranScinde();
}

MenuOptionsPartie::~MenuOptionsPartie()
{
    //dtor
}
void MenuOptionsPartie::StartMenuEcranScinde()
{
    GraphicalEngine* engine = GraphicalEngine::GetInstance();
    sf::RenderWindow *app = engine->GetRenderWindow();
    GuiNode *node = (GuiNode*)engine->GetGuiManager()->GetRootNode()->AddGuiNode();
    GuiSliderNumberItem *tailleCarteItem = (GuiSliderNumberItem*)node->AddItem(new GuiSliderNumberItem);
    GuiTextItem* affTailleCarte = (GuiTextItem*)node->AddItem(new GuiTextItem);
    GuiButtonItem* btn = (GuiButtonItem*)node->AddItem(new GuiButtonItem);
    btn->SetText("Jouer");
    btn->SetData("this", this);
    btn->SetCallBack("clicked", CommencerPartie);
    tailleCarteItem->SetSize(sf::Vector2f(200,20));
    tailleCarteItem->SetRange(2000, 8000);
    tailleCarteItem->SetValue(2000);
    tailleCarteItem->SetColor(sf::Color(255,0,0));
    tailleCarteItem->SetBarColor(sf::Color(255,255,255));
    tailleCarteItem->SetFocusColor(sf::Color(0,255,0));
    tailleCarteItem->SetData("this", this);
    tailleCarteItem->SetData("indicator", affTailleCarte);
    tailleCarteItem->SetCallBack("value_changed", ChangerTailleCarte);
    node->SetAbsolutePosition(100,100);
    tailleCarteItem->SetRelativePosition(0,100);
    btn->SetRelativePosition(0,200);

    bool continuer=true;
    sf::Event event;
    while(continuer)
    {
        while(app->PollEvent(event))
        {
            engine->GetGuiManager()->HandleEvent(event);
            if(event.Type==sf::Event::Closed)
                continuer=false;
        }
        engine->DrawScene();
        if(m_start)
            continuer=false;
    }
    int taille = tailleCarteItem->GetValue();
    engine->GetGuiManager()->RemoveNode(node);
    if(m_start)
    {
        Game game;
        game.SetTailleCarte(taille);
        game.Start();
    }
}
void MenuOptionsPartie::ChangerTailleCarte(GuiItem* item)
{
    int val = ((GuiSliderNumberItem*)item)->GetValue();
    ((MenuOptionsPartie*)item->GetData("this"))->m_tailleCarte = val;
    ((GuiTextItem*)item->GetData("indicator"))->SetText("Largeur de la carte: "+int2string(val*0.1)+ " M");
}
void MenuOptionsPartie::CommencerPartie(GuiItem* item)
{
    ((MenuOptionsPartie*)item->GetData("this"))->m_start=true;
}
