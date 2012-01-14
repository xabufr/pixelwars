#ifndef PROJECTILE_H
#define PROJECTILE_H

#include <Box2D/Box2D.h>
#include "engines/graphics/graphicalengine.h"

class Projectile
{
    public:
        Projectile(b2World*, b2Vec2 pos, b2Vec2 direction, float taille, float poids, float puissance);
        virtual ~Projectile();
        void Update();
        float GetPuissance() const;
    protected:
    private:
        SceneNode *m_node;
        SceneNodeCircleShapeItem *m_projectileItem;
        b2Body *m_body;

        float m_puissance, m_poids;
};

#endif // PROJECTILE_H
