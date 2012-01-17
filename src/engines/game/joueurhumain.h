#ifndef JOUEURHUMAIN_H
#define JOUEURHUMAIN_H

#include "joueur.h"
#include <SFML2/Graphics.hpp>

class JoueurHumain: public Joueur
{
    public:
        JoueurHumain(const sf::FloatRect & portion = sf::FloatRect(0,0,1,1));
        virtual ~JoueurHumain();

        virtual void Update();
    private:
        sf::View *m_camera;
        sf::RenderWindow *m_app;
};

#endif // JOUEURHUMAIN_H
