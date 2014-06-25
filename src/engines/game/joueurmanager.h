#ifndef JOUEURMANAGER_H
#define JOUEURMANAGER_H

#include <SFML/System.hpp>
#include <unordered_map>

class Joueur;
class Carte;
class Unite;
struct UnitInput;
enum TypeJoueur{
    JOUEUR_LOCAL, JOUEUR_IA, JOUEUR_DISTANT
};
class JoueurManager
{
    public:
        JoueurManager(Carte&);
        virtual ~JoueurManager();
        void SetJoueur(int, TypeJoueur);
        void AjouterUnite(int, sf::Uint32, Unite*);
        void SupprimerUnite(Unite*);
        void Update();
        Joueur* GetJoueur(int);
        Joueur* GetJoueur(Unite*) const;
        int GetId(Joueur*);
        void Input(int, const UnitInput&);

    private:
        Joueur *m_joueurs[2];
        Carte &m_carte;
        std::unordered_map<Unite*, Joueur*> m_unitesJoueur;
        int m_nbLocal;
};

#endif // JOUEURMANAGER_H
