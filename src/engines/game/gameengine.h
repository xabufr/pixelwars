#ifndef GAMEENGINE_H
#define GAMEENGINE_H
#include "../engine.h"
#include "carte.h"
#include "contactlistenner.h"
#include <deque>
class SceneNodeTextureItem;
class Projectile;
class GameEngine : public Engine
{
    public:
        GameEngine();
        virtual ~GameEngine();
        void Start();
        EngineType GetEngineId() const;

    private:
        ContactListenner m_listner;
        void GererExplosions();
        void EnleverUnit(Unite*);
        void EnleverUnit(sf::Uint32);

        SceneNodeTextureItem* worldItem;
        b2World* m_world;
        b2Body* ground;
        Carte *m_carte;
        std::deque<Projectile*> m_projectiles;
        std::unordered_map<sf::Uint32, Unite*> m_unites;

        void DeleteProjectile(Projectile *toDelete);
        void MoveProjectiles();
};

#endif // GAMEENGINE_H
