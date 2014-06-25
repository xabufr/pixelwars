#ifndef JOUEUR_H
#define JOUEUR_H

#include <unordered_map>
#include <SFML/System.hpp>
#include <Box2D/Box2D.h>
#include <SFML/Graphics.hpp>

struct UnitInput;
class Unite;
class Carte;
class Joueur
{
    public:
        Joueur(Carte &carte, int numero);
        virtual ~Joueur();

        virtual void SetInput(const UnitInput &);
        void SetCamp(float xMin, float xMax);

        void AjouterUnite(sf::Uint32, Unite*);

        void EnleverUnite(sf::Uint32 id);
        void EnleverUnite(Unite*);

        void SelectedFollowingUnit();
        void SelectFirstUnit();
        void SelectedUnit(sf::Uint32 id);

        Unite* GetSelectedUnit() const;

        void SetColor(const sf::Color& col);

        b2Vec2 GetPositionNouvelleUnite() const;

        virtual void Update() = 0;

    protected:
        std::unordered_map<sf::Uint32, Unite*> m_unites;
        sf::Color m_couleur;
        Unite *m_selectedUnit;
        sf::Uint32 m_idSelected;
        float m_xMinCamp, m_xMaxCamp, m_tailleCamp;
        Carte &m_carte;

        int m_numero;
};

#endif // JOUEUR_H
