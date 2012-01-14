#ifndef UNITE_H
#define UNITE_H
#include <Box2D/Box2D.h>
#include "engines/graphics/graphicalengine.h"

class Unite
{
    public:
        Unite(b2World*);
        virtual ~Unite();
        void SetPosition();
    protected:
    private:
        b2Body *m_body;
        SceneNode *m_node;
};

#endif // UNITE_H
