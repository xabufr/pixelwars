#include "inputmanager.h"

InputManager::InputManager()
{
    m_touchesJ1["droite"] = sf::Keyboard::Key::Right;
    m_touchesJ1["gauche"] = sf::Keyboard::Key::Left;
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
    return i;
}
