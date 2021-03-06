#ifndef JOUEURHUMAIN_H
#define JOUEURHUMAIN_H

#include "joueur.h"
#include <SFML/Graphics.hpp>
class Carte;
class JoueurHumain: public Joueur
{
    public:
        JoueurHumain(Carte &, int numero,const sf::FloatRect & portion = sf::FloatRect(0,0,1,1));
        virtual ~JoueurHumain();
        virtual void SetInput(const UnitInput&);

        virtual void Update();
        sf::View* GetCam() const;
    private:

        void Zoomer();
        void Dezoomer();

        float m_zoom, m_lastValidZoom, m_specialZoom;
        bool m_useSpecialZoom;
        sf::View *m_camera;
        sf::RenderWindow *m_app;
};

#endif // JOUEURHUMAIN_H
