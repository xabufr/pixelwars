#ifndef UNITEMANAGER_H
#define UNITEMANAGER_H

#include <unordered_map>
#include <SFML2/System.hpp>
#include <deque>

class Unite;
class Joueur;
class JoueurManager;
class Projectile;
class UniteManager
{
    public:
        UniteManager(JoueurManager *j_manager=0);
        void SetJoueurManager(JoueurManager *j_manager);
        virtual ~UniteManager();

        void EnleverUnite(sf::Uint32);
        void EnleverUnite(Unite*);

        void DetruireUnite(sf::Uint32);
        void DetruireUnite(Unite*);

        void AjouterUnite(int, sf::Uint32, Unite*);
        void AjouterUnite(sf::Uint32, Unite*);

        Unite* GetUnite(sf::Uint32);

        void Update();

        const std::unordered_map<sf::Uint32, Unite*>& GetListe() const;

        void DeleteProjectile(Projectile*);

    private:
        std::unordered_map<sf::Uint32, Unite*> m_unites;
        std::deque<Projectile*> m_projectiles;
        JoueurManager *m_joueurManager;
};

#endif // UNITEMANAGER_H
