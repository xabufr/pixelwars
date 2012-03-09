#ifndef PARTICLEMANAGER_H
#define PARTICLEMANAGER_H

#include <unordered_map>
#include <SFML2/System.hpp>

class SceneManager;
class SceneNode;

struct ParticleParameters
{
    int number;
    bool colorFromList;
    std::vector<sf::Color> m_colorList;
    sf::Color colorMin, colorMax;
    int level;
    int timeToLive;
    sf::Clock clock;
    float minSize, maxSize;
};

class ParticleManager
{
    public:
        ParticleManager(SceneManager* parent);
        virtual ~ParticleManager();

        void Update();
        SceneNode* AddParticleSystem(const ParticleParameters&);
        void Remove(SceneNode*);

    protected:
    private:
        std::unordered_map<SceneNode*, ParticleParameters> m_nodes;
        SceneManager *m_parent;

        SceneNode* m_CreateNodeFromParameters(const ParticleParameters&);
};

#endif // PARTICLEMANAGER_H
