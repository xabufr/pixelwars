#ifndef UNITETERRESTRE_H
#define UNITETERRESTRE_H
#include "unite.h"

class UniteTerrestre : public Unite
{
    public:
        UniteTerrestre(b2World*);
        virtual ~UniteTerrestre();
        virtual void Update();
        void Deplacer(const UnitInput& input);
        void Stop();
    protected:
    private:
        b2Body *m_roue1, *m_roue2;
        b2WheelJoint *jointure1, *jointure2;
        SceneNodeShapeItem *m_itemBody;
        SceneNodeSpriteItem *m_itemRoue1, *m_itemRoue2;
        SceneNode *m_nodeRoue1, *m_nodeRoue2;
};

#endif // UNITETERRESTRE_H
