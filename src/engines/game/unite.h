#ifndef UNITE_H
#define UNITE_H
#include <Box2D/Box2D.h>
#include "engines/graphics/graphicalengine.h"
#include "inputmanager.h"

class Unite
{
    public:
        Unite(b2World*, b2Vec2 pos);
        virtual ~Unite();
        void SetPosition();
        virtual void Update();
        virtual void Deplacer(const UnitInput&) = 0;
        virtual void Stop()=0;
    protected:
        b2Body *m_body;
        SceneNode *m_node;
};

#endif // UNITE_H
