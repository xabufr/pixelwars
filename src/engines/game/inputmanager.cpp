#include "inputmanager.h"

InputManager::InputManager()
{
    m_touches[0]["droite"] = sf::Keyboard::Key::Right;
    m_touches[0]["gauche"] = sf::Keyboard::Key::Left;
    m_touches[0]["haut"] = sf::Keyboard::Key::Up;
    m_touches[0]["bas"] = sf::Keyboard::Key::Down;
    m_touches[0]["tirer"] = sf::Keyboard::Key::Space;
    m_touches[0]["changer_unite"] = sf::Keyboard::Key::RControl;
    m_touches[0]["zoomer"] = sf::Keyboard::Key::Add;
    m_touches[0]["dezoomer"] = sf::Keyboard::Key::Subtract;

    m_touches[1]["droite"] = sf::Keyboard::Key::D;
    m_touches[1]["gauche"] = sf::Keyboard::Key::Q;
    m_touches[1]["haut"] = sf::Keyboard::Key::Z;
    m_touches[1]["bas"] = sf::Keyboard::Key::S;
    m_touches[1]["tirer"] = sf::Keyboard::Key::LControl;
    m_touches[1]["changer_unite"] = sf::Keyboard::Key::E;
    m_touches[1]["zoomer"] = sf::Keyboard::Key::W;
    m_touches[1]["dezoomer"] = sf::Keyboard::Key::X;
}

InputManager::~InputManager()
{
    //dtor
}
void InputManager::HandleEvent(const sf::Event& event)
{
    if(event.Type == sf::Event::KeyReleased)
    {
        for(int i=0;i<2;++i)
        {
            if(event.Key.Code == m_touches[i]["changer_unite"])
                m_input[i].changer=true;
            else if(event.Key.Code == m_touches[i]["zoomer"])
                m_input[i].zoomer=true;
            else if(event.Key.Code == m_touches[i]["dezoomer"])
                m_input[i].dezoomer=true;
        }
    }
}
bool InputManager::GetSate(const std::string &clef, int joueur)
{
    auto it = m_touches[joueur].find(clef);
    if(it==m_touches[joueur].end())
        return false;
    return m_keyboard.IsKeyPressed(it->second);
}

UnitInput InputManager::GetAll(int joueur)
{
    m_input[joueur].gauche = GetSate("gauche", joueur);
    m_input[joueur].droite = GetSate("droite", joueur);
    m_input[joueur].bas = GetSate("bas", joueur);
    m_input[joueur].haut = GetSate("haut", joueur);
    m_input[joueur].tirer = GetSate("tirer",joueur);

    UnitInput tmp = m_input[joueur];
    m_input[joueur].changer=false;
    m_input[joueur].zoomer=false;
    m_input[joueur].dezoomer=false;

    return tmp;
}
