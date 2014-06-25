#ifndef NUAGE_H
#define NUAGE_H
#include "../graphics/graphicalengine.h"

class Nuage
{
    public:
        Nuage(const sf::Vector2f&, float maxY, int nb=5, float charge=0.f);
        virtual ~Nuage();
        void Move(const sf::Vector2f&);
        void SetColor(const sf::Color&);
        const sf::Vector2f& GetPosition() const;
        void Update();
        const sf::Vector2f& GetSize();
        void SetPosition(const sf::Vector2f&);
        void SetX(float x);
        void Work();
        void SetWind(float);

    private:
        sf::Clock m_timerVent, m_timerPluie;
        sf::Vector2f m_size;
        int m_nbParties;
        SceneNode *m_node;
        SceneNodeSpriteItem **m_partiesNuage;
        float m_vitesse, m_diffVent, m_vent, m_charge, m_maxY;
        bool m_transitionVent;
        std::vector<SceneNodeSpriteItem*> m_goutes;
        float m_coefPluie;
        void m_GererPluie();
};

#endif // NUAGE_H
