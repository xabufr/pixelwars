#include "particlemanager.h"
#include "scenenode.h"
#include "scenenodecircleshapeitem.h"
#include "scenemanager.h"
#include "../../core/trigo.h"

ParticleManager::ParticleManager(SceneManager *parent)
{
    m_parent=parent;
}

ParticleManager::~ParticleManager()
{
    //dtor
}

void ParticleManager::Remove(SceneNode* node)
{
    auto it = m_nodes.find(node);
    if(it!=m_nodes.end())
        m_nodes.erase(it);
}

void ParticleManager::Update()
{
    std::vector<SceneNodeItem*> toDelete;
    for(Particle &p : m_particles)
    {
        float time = p.timerDep.Restart().AsSeconds();
        p.dir.y+=p.puissance*time;
        p.item->SetRelativePosition(p.item->GetRelativePosition().x+p.dir.x*time, p.item->GetRelativePosition().y+p.dir.y*time);
        if(p.timer.GetElapsedTime().AsMilliseconds()>p.timeToLive)
        {
            toDelete.push_back(p.item);
        }
        else if(p.timer.GetElapsedTime().AsMilliseconds()>p.timeToLive*0.5)
        {
            float ratio = (p.timeToLive*0.5-p.timer.GetElapsedTime().AsMilliseconds())/(p.timeToLive*0.5);
            p.color.a = 255*ratio;
            ((SceneNodeCircleShapeItem*)p.item)->SetColor(p.color);
        }
    }
    for(SceneNodeItem* item : toDelete)
    {
        for(size_t i(0), taille(m_particles.size());i<taille;++i)
        {
            if(m_particles[i].item==item)
            {
                m_particles.erase(m_particles.begin()+i);
                m_nodesItems[item]->RemoveItem(item);
                delete item;
                break;
            }
        }
    }
    std::unordered_map<SceneNode*, ParticleParameters> temp = m_nodes;
    for(auto &it : temp)
    {
        if(it.first->GetChildItems().size()==0)
        {
            Remove(it.first);
            m_parent->RemoveNode(it.first);
        }
    }
}

SceneNode* ParticleManager::AddParticleSystem(const ParticleParameters& param)
{
    SceneNode* node;
    node = m_CreateNodeFromParameters(param);
    m_nodes[node] = param;
    return node;
}

SceneNode* ParticleManager::m_CreateNodeFromParameters(const ParticleParameters& param)
{
    SceneNode* node;
    SceneNodeCircleShapeItem *item;

    node = m_parent->GetRootNode()->AddSceneNode();
    node->SetLevel(param.level);
    node->SetAbsolutePosition(param.position);
    Particle particle;

    for(int i(0);i<param.number;++i)
    {
        item = new SceneNodeCircleShapeItem;
        float radius = Random::Rand(param.minSize,param.maxSize);
        item->SetRadius(radius);

        particle.color=m_RandomColor(param);
        item->SetColor(particle.color);
        node->AddItem(item);
        particle.timeToLive = param.timeToLive;
        particle.item = item;



        float angle = Random::Rand(param.minAngle, param.maxAngle);
        particle.puissance = Random::Rand(param.minPower, param.maxPower);

        particle.dir=Trigo::TranslateDeg(angle, particle.puissance);


        particle.timer.Restart();
        particle.timerDep.Restart();
        m_particles.push_back(particle);
        m_nodesItems[item] = node;
    }
    return node;
}

sf::Color ParticleManager::m_RandomColor(const ParticleParameters& param)
{
    if(param.colorFromList)
    {
        int choix = Random::Rand(0, param.colorList.size()-1);
        return param.colorList[choix];
    }
    else
    {
        return sf::Color(Random::Rand(param.colorMin.r,param.colorMax.r),
                         Random::Rand(param.colorMin.g,param.colorMax.g),
                         Random::Rand(param.colorMin.b,param.colorMax.b));
    }
    return sf::Color();
}

void ParticleManager::SetWorld(b2World *world)
{
    m_world=world;
}
