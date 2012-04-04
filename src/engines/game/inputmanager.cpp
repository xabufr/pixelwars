#include "inputmanager.h"
#include "playerparameters.h"

InputManager::InputManager()
{
    PlayerParameters *playerParam = PlayerParameters::GetInstance();
    for(int i=0;i<2;++i)
    {
        m_touches[i]["droite"] = playerParam->GetTouche("droite", i);
        m_touches[i]["gauche"] = playerParam->GetTouche("gauche", i);
        m_touches[i]["haut"] = playerParam->GetTouche("haut", i);
        m_touches[i]["bas"] = playerParam->GetTouche("bas", i);
        m_touches[i]["tirer"] = playerParam->GetTouche("tirer", i);
        m_touches[i]["changer_unite"] = playerParam->GetTouche("changer_unite", i);
        m_touches[i]["zoomer"] = playerParam->GetTouche("zoomer", i);
        m_touches[i]["dezoomer"] = playerParam->GetTouche("dezoomer", i);
    }
}

InputManager::~InputManager()
{
    //dtor
}
void InputManager::HandleEvent(const sf::Event& event)
{
    if(event.type == sf::Event::KeyReleased)
    {
        for(int i=0;i<2;++i)
        {
            if(event.key.code == m_touches[i]["changer_unite"])
                m_input[i].changer=true;
            else if(event.key.code == m_touches[i]["zoomer"])
                m_input[i].zoomer=true;
            else if(event.key.code == m_touches[i]["dezoomer"])
                m_input[i].dezoomer=true;
        }
    }
}
bool InputManager::GetSate(const std::string &clef, int joueur)
{
    auto it = m_touches[joueur].find(clef);
    if(it==m_touches[joueur].end())
        return false;
    return m_keyboard.isKeyPressed(it->second);
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
