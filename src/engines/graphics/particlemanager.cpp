#include "particlemanager.h"

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

    node = new SceneNode(m_parent, m_parent->GetRootNode());
    for(int i(0);i<param.number;++i)
    {
        item = new SceneNodeCircleShapeItem;
    }
}
