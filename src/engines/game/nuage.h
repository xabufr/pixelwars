#ifndef NUAGE_H
#define NUAGE_H
#include "../graphics/graphicalengine.h"

class Nuage
{
    public:
        Nuage(const sf::Vector2f&, int nb=5);
        virtual ~Nuage();
        void Move(const sf::Vector2f&);
        void SetColor(const sf::Color&);
        const sf::Vector2f& GetPosition() const;
        void Update();
        const sf::Vector2f& GetSize();
        void SetPosition(const sf::Vector2f&);
        void SetX(float x);
    protected:
    private:
        sf::Vector2f m_size;
        int m_nbParties;
        SceneNode *m_node;
        SceneNodeSpriteItem **m_partiesNuage;
};

#endif // NUAGE_H
