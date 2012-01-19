#include "inputmanager.h"

InputManager::InputManager()
{
    m_touchesJ1["droite"] = sf::Keyboard::Key::Right;
    m_touchesJ1["gauche"] = sf::Keyboard::Key::Left;
    m_touchesJ1["haut"] = sf::Keyboard::Key::Up;
    m_touchesJ1["bas"] = sf::Keyboard::Key::Down;
    m_touchesJ1["tirer"] = sf::Keyboard::Key::Space;
    m_touchesJ1["changer_unite"] = sf::Keyboard::Key::RControl;

    m_touchesJ2["droite"] = sf::Keyboard::Key::D;
    m_touchesJ2["gauche"] = sf::Keyboard::Key::Q;
    m_touchesJ2["haut"] = sf::Keyboard::Key::Z;
    m_touchesJ2["bas"] = sf::Keyboard::Key::S;
    m_touchesJ2["tirer"] = sf::Keyboard::Key::LControl;
    m_touchesJ2["changer_unite"] = sf::Keyboard::Key::E;
}

InputManager::~InputManager()
{
    //dtor
}
void InputManager::HandleEvent(const sf::Event& event)
{
    if(event.Type == sf::Event::KeyReleased)
    {
        if(event.Key.Code == m_touchesJ1["changer_unite"])
            m_input[0].changer=true;
        else if(event.Key.Code == m_touchesJ2["changer_unite"])
            m_input[1].changer=true;
    }
}
bool InputManager::GetSate(const std::string &clef, int joueur)
{
    if(joueur==0)
    {
        auto it = m_touchesJ1.find(clef);
        if(it==m_touchesJ1.end())
            return false;
        return m_keyboard.IsKeyPressed(it->second);
    }
    else
    {
        auto it = m_touchesJ2.find(clef);
        if(it==m_touchesJ2.end())
            return false;
        return m_keyboard.IsKeyPressed(it->second);
    }

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

    return tmp;
}
