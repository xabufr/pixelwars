#ifndef NUAGE_H
#define NUAGE_H
#include "../graphics/graphicalengine.h"

class Nuage
{
    public:
        Nuage(const sf::Vector2f&, int nb=5, float charge=0.f);
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

    protected:
    private:
        sf::Clock m_timerVent;
        sf::Vector2f m_size;
        int m_nbParties;
        SceneNode *m_node;
        SceneNodeSpriteItem **m_partiesNuage;
        float m_vitesse, m_diffVent, m_vent, m_charge;
        bool m_transitionVent;
};

#endif // NUAGE_H
