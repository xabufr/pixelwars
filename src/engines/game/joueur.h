#ifndef JOUEUR_H
#define JOUEUR_H

#include <unordered_map>
#include <SFML2/System.hpp>

struct UnitInput;
class Unite;
class Joueur
{
    public:
        Joueur();
        virtual ~Joueur();

        virtual void SetInput(const UnitInput &);

        void AjouterUnite(sf::Uint32, Unite*);

        void EnleverUnite(sf::Uint32 id);
        void EnleverUnite(Unite*);

        void SelectedFollowingUnit();
        void SelectFirstUnit();
        void SelectedUnit(sf::Uint32 id);

        virtual void Update() = 0;

    protected:
        std::unordered_map<sf::Uint32, Unite*> m_unites;
        Unite *m_selectedUnit;
        sf::Uint32 m_idSelected;
};

#endif // JOUEUR_H
