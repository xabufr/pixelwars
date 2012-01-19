#ifndef GAMEENGINE_H
#define GAMEENGINE_H
#include "../engine.h"
#include "carte.h"
#include "contactlistenner.h"
#include <deque>
#include "joueurmanager.h"
#include "unitemanager.h"

class JoueurHumain;
class Joueur;
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
        ContactListenner *m_listner;
        void GererExplosions();
        void EnleverUnit(Unite*);
        void EnleverUnit(sf::Uint32);

        SceneNodeTextureItem* worldItem;
        b2World* m_world;
        b2Body* ground;
        Carte *m_carte;

        JoueurManager *joueurManager;
        UniteManager *uniteManager;

        void DeleteProjectile(Projectile *toDelete);

        sf::Uint32 m_lastId;
};

#endif // GAMEENGINE_H
