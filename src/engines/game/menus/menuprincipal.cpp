#include "menuprincipal.h"
#include "../gameengine.h"
#include "core/logger.h"
MenuPrincipal::MenuPrincipal()
{
    GraphicalEngine* engine = GraphicalEngine::GetInstance();
    engine->CreateRenderWindow(sf::VideoMode(600,400), "Test");
    m_contenerMenu = engine->GetGuiManager()->GetRootNode()->AddContener();

    m_btn_quitter = new GuiButtonItem;
    m_btn_jouer_scinde = new GuiButtonItem;

    m_btn_quitter->SetText("Quitter");
    m_btn_jouer_scinde->SetText(L"Jouer en écran scindé");

    m_btn_jouer_scinde->SetData("menu", this);

    m_btn_quitter->SetCallBack("clicked", CallbackQuitter);
    m_btn_jouer_scinde->SetCallBack("clicked", JouerScinde);

    m_btn_quitter->SetMouseOverColor(sf::Color(255,0,0), sf::Color(0,0,0,0));
    m_btn_jouer_scinde->SetMouseOverColor(sf::Color(255,0,0), sf::Color(0,0,0,0));

    m_btn_quitter->SetNormalColor(sf::Color(255,255,255), sf::Color(0,0,0,0));
    m_btn_jouer_scinde->SetNormalColor(sf::Color(255,255,255), sf::Color(0,0,0,0));

    m_contenerMenu->AjouterItem(m_btn_jouer_scinde, 0, 0);
    m_contenerMenu->AjouterItem(m_btn_quitter, 0, 1);

    m_gameToStart = false;
}

MenuPrincipal::~MenuPrincipal()
{
    GraphicalEngine::Kill();
}
void MenuPrincipal::Show()
{
    GraphicalEngine* engine = GraphicalEngine::GetInstance();

    sf::RenderWindow *app = engine->GetRenderWindow();
    sf::Event event;

    while(app->IsOpened())
    {
        while(app->PollEvent(event))
        {
            engine->GetGuiManager()->HandleEvent(event);
        }
        engine->DrawScene();
        if(m_gameToStart)
        {
            m_gameToStart=false;
            this->m_contenerMenu->Hide();
            GameEngine game;
            game.Start();
            this->m_contenerMenu->Show();
        }
    }
}

void MenuPrincipal::CallbackQuitter(GuiItem* item)
{
    GraphicalEngine::GetInstance()->GetRenderWindow()->Close();
}
void MenuPrincipal::JouerScinde(GuiItem* item)
{
    MenuPrincipal *menu = (MenuPrincipal*)item->GetData("menu");
    menu->m_gameToStart=true;
}
