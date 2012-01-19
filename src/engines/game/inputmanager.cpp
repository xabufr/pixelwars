#include "inputmanager.h"

InputManager::InputManager()
{
    m_touchesJ1["droite"] = sf::Keyboard::Key::Right;
    m_touchesJ1["gauche"] = sf::Keyboard::Key::Left;
    m_touchesJ1["haut"] = sf::Keyboard::Key::Up;
    m_touchesJ1["bas"] = sf::Keyboard::Key::Down;
    m_touchesJ1["tirer"] = sf::Keyboard::Key::Space;

    m_touchesJ2["droite"] = sf::Keyboard::Key::D;
    m_touchesJ2["gauche"] = sf::Keyboard::Key::Q;
    m_touchesJ2["haut"] = sf::Keyboard::Key::Z;
    m_touchesJ2["bas"] = sf::Keyboard::Key::S;
    m_touchesJ2["tirer"] = sf::Keyboard::Key::LControl;
}

InputManager::~InputManager()
{
    //dtor
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
    UnitInput i;
    i.gauche = GetSate("gauche", joueur);
    i.droite = GetSate("droite", joueur);
    i.bas = GetSate("bas", joueur);
    i.haut = GetSate("haut", joueur);
    i.tirer = GetSate("tirer",joueur);
    return i;
}
