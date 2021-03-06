#ifndef INPUTMANAGER_H
#define INPUTMANAGER_H

#include <SFML/Window.hpp>
#include <unordered_map>

struct UnitInput{
    bool gauche, droite, haut, bas, tirer, changer, zoomer, dezoomer;
    UnitInput()
    {
        gauche = droite = haut = bas = tirer = changer = zoomer = dezoomer = false;
    }
};
class InputManager
{
    public:
        InputManager();
        virtual ~InputManager();
        void HandleEvent(const sf::Event&);
        bool GetSate(const std::string&, int joueur);
        UnitInput GetAll(int joueur=0);
    private:
        sf::Keyboard m_keyboard;
        std::unordered_map<std::string, sf::Keyboard::Key> m_touchesJ1;
        std::unordered_map<std::string, sf::Keyboard::Key> m_touchesJ2;
        std::unordered_map<std::string, sf::Keyboard::Key> m_touches[2];

        UnitInput m_input[2];
};

#endif // INPUTMANAGER_H
