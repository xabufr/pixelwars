#ifndef JOUEUR_H
#define JOUEUR_H

#include <SFML2/Graphics.hpp>
#include <unordered_map>
struct UnitInput;
class Unite;
class Joueur
{
    public:
        Joueur(const sf::FloatRect &portionEcran=sf::FloatRect(0.f,0.f,1.f,1.f));
        virtual ~Joueur();

        void SetInput(const UnitInput &);

        void AjouterUnite(sf::Uint32, Unite*);

        void EnleverUnite(sf::Uint32 id);
        void EnleverUnite(Unite*);

        void SelectedFollowingUnit();
        void SelectFirstUnit();
        void SelectedUnit(sf::Uint32 id);

        void PositionnerCamera();

        void Update();

    private:
        sf::View *m_camera;
        std::unordered_map<sf::Uint32, Unite*> m_unites;
        Unite *m_selectedUnit;
        sf::Uint32 m_idSelected;
};

#endif // JOUEUR_H
