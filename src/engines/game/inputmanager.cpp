#include "inputmanager.h"

InputManager::InputManager()
{
    m_touchesJ1["droite"] = sf::Keyboard::Key::Right;
    m_touchesJ1["gauche"] = sf::Keyboard::Key::Left;
    m_touchesJ1["haut"] = sf::Keyboard::Key::Up;
    m_touchesJ1["bas"] = sf::Keyboard::Key::Down;
    m_touchesJ1["tirer"] = sf::Keyboard::Key::Space;
}

InputManager::~InputManager()
{
    //dtor
}
bool InputManager::GetSate(const std::string &clef)
{
    auto it = m_touchesJ1.find(clef);
    if(it==m_touchesJ1.end())
        return false;
    return m_keyboard.IsKeyPressed(it->second);
}

UnitInput InputManager::GetAll()
{
    UnitInput i;
    i.gauche = GetSate("gauche");
    i.droite = GetSate("droite");
    i.bas = GetSate("bas");
    i.haut = GetSate("haut");
    i.tirer = GetSate("tirer");
    return i;
}
