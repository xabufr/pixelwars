#ifndef PARTICLEMANAGER_H
#define PARTICLEMANAGER_H

#include <unordered_map>
#include <vector>
#include <SFML2/Graphics.hpp>
#include <SFML2/System.hpp>
#include "../../core/random.h"
#include <Box2D/Box2D.h>

class SceneManager;
class SceneNode;
class SceneNodeItem;
struct ParticleParameters
{
    int number;
    bool colorFromList;
    std::vector<sf::Color> colorList;
    sf::Color colorMin, colorMax;
    int level;
    int timeToLive;
    sf::Clock clock;
    float minSize, maxSize;
    sf::Vector2f position;
    float minAngle, maxAngle;
    float minPower, maxPower;
};

struct Particle
{
    SceneNodeItem *item;
    int timeToLive;
    float puissance;
    sf::Clock timer, timerDep;
    sf::Vector2f dir;
    sf::Color color;
};

class ParticleManager
{
    public:
        ParticleManager(SceneManager* parent);
        virtual ~ParticleManager();

        void Update();
        SceneNode* AddParticleSystem(const ParticleParameters&);
        void Remove(SceneNode*);
        void SetWorld(b2World *world);

    protected:
    private:
        static sf::Color m_RandomColor(const ParticleParameters&);

        std::unordered_map<SceneNode*, ParticleParameters> m_nodes;
        std::unordered_map<SceneNodeItem*,SceneNode*> m_nodesItems;
        std::vector<Particle> m_particles;
        SceneManager *m_parent;
        b2World *m_world;

        SceneNode* m_CreateNodeFromParameters(const ParticleParameters&);
};

#endif // PARTICLEMANAGER_H
