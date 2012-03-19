#include "menuoptionspartie.h"
#include "engines/graphics/graphicalengine.h"
#include "core/conversion.h"
#include "../gameengine.h"
#include "game.h"
#include "infomessage.h"

MenuOptionsPartie::MenuOptionsPartie(TypePartie type)
{
    m_tailleCarte=2000;
    m_start = false;
    m_seed = time(NULL);
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
    GuiTextItem *texteSeed = (GuiTextItem*)node->AddItem(new GuiTextItem);
    GuiButtonItem* btn = (GuiButtonItem*)node->AddItem(new GuiButtonItem);
    GuiSliderNumberItem *seedSelector = (GuiSliderNumberItem*)node->AddItem(new GuiSliderNumberItem);
    GuiTextItem *seedIndicator = (GuiTextItem*)node->AddItem(new GuiTextItem);

    seedSelector->SetRange(-1, 50000);
    seedSelector->SetValue(-1);
    seedSelector->SetSize(sf::Vector2f(350,20));
    seedSelector->SetColor(sf::Color(255,0,0));
    seedSelector->SetBarColor(sf::Color(255,255,255));
    seedSelector->SetFocusColor(sf::Color(0,255,0));
    seedSelector->SetData("this", this);
    seedSelector->SetData("indicator", seedIndicator);
    seedSelector->SetCallBack("value_changed", ChangerSeed);
    seedIndicator->SetText("-1");

    seedIndicator->SetRelativePosition(400,300);
    seedSelector->SetRelativePosition(0, 300);
    texteSeed->SetRelativePosition(0, 250);


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
    texteSeed->SetText("Seed:");
    node->SetAbsolutePosition(25,0);
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
            if(event.Type==sf::Event::EventType::KeyPressed && event.Key.Code==sf::Keyboard::Key::Escape)
                continuer = false;
        }
        engine->DrawScene();
        if(m_start)
            continuer=false;
    }
    int taille = tailleCarteItem->GetValue();
    engine->GetGuiManager()->RemoveNode(node);
    std::string tmpMes;
    if(m_start)
    {
        {
            Game game;
            game.SetTailleCarte(taille);
            game.SetSeed(m_seed);
            game.Start();
            tmpMes = "Le joueur " + game.GetWinner() + " gagne !";
        } //Appel du destructeur de game
        InfoMessage info(tmpMes);
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
void MenuOptionsPartie::ChangerSeed(GuiItem* item)
{
    int seed = (int)((GuiSliderNumberItem*)item)->GetValue();
    if(seed==-1)
        seed = time(NULL);
    ((MenuOptionsPartie*)item->GetData("this"))->m_seed = seed;
    ((GuiTextItem*)item->GetData("indicator"))->SetText(int2string(seed));
}
