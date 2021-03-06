#ifndef CIEL_H
#define CIEL_H
#include "../graphics/graphicalengine.h"
#include <vector>

class Nuage;
class Ciel
{
    public:
        Ciel(const sf::Vector2f&, float dayDuration=5.f);
        virtual ~Ciel();
        void Work();
    private:
        void m_SetVisibleStars(bool=true);
        std::vector<Nuage*> m_nuages;
        std::vector<SceneNodeSpriteItem*> m_etoiles;
        sf::Clock m_time, m_timerDep, m_timerVent;
        float m_vent;
        int m_timeChangeWind;
        size_t m_nbNuages;

        int m_dayDuration;

        SceneNodeSpriteItem *m_itemSoleil, *m_itemLune;
        SceneNodeCircleShapeItem *m_fondCiel;
        SceneNode *m_nodeCiel;
        SceneNode *m_nodeEtoiles;

        sf::Vector2f m_taille;

        void m_GererNuages();
        void m_ChangeWind();
};

#endif // CIEL_H
