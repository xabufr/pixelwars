#ifndef GAMEENGINE_H
#define GAMEENGINE_H
#include "../engine.h"
#include "carte.h"
#include "contactlistenner.h"
class SceneNodeTextureItem;
class GameEngine : public Engine
{
    public:
        GameEngine();
        virtual ~GameEngine();
        void Start();
        EngineType GetEngineId() const;
    protected:
    private:
        ContactListenner m_listner;
        void GererExplosions();
        SceneNodeTextureItem* worldItem;
        b2World* m_world;
        b2Body* ground;
        Carte *m_carte;
};

#endif // GAMEENGINE_H
