#include "scenenode.h"
#include "engines/graphics/scenemanager.h"

SceneNode::SceneNode(SceneManager* mng, SceneNode* parent)
{
    m_parent=parent;

    m_level=0;
    m_levelReel = this->CalculerLevel();
    m_relative.rotation=0;
    m_relative.scale=sf::Vector2f(1,1);

    m_manager=mng;
    m_manager->AddNodeLevel(m_levelReel, this);
    SetRelativePosition(m_relative.position);
    if(m_parent)
    {
        m_absolute = m_parent->m_absolute;
    }

}

SceneNode::~SceneNode()
{
    if(m_parent)
        m_parent->InformedParentBeforeDeletion(this);
    unsigned int taille = m_childItems.size();
    for(unsigned int i=0;i<taille;++i)
    {
        delete m_childItems[i];
    }

}

SceneNode* SceneNode::AddSceneNode()
{
    SceneNode* node = new SceneNode(m_manager, this);
    return AddSceneNode(node);
}
SceneNode* SceneNode::AddSceneNode(SceneNode *newNode)
{
    m_childNodes.push_back(newNode);
    return newNode;
}
SceneNodeItem *SceneNode::AddItem(SceneNodeItem *item)
{
    m_childItems.push_back(item);
    item->SetParentPosition(m_absolute.position);
    item->SetAbsoluteRotation(m_absolute.rotation);
    item->SetAbsoluteScale(m_absolute.scale);
    return item;
}

const std::vector<SceneNode*> & SceneNode::GetChildSceneNodes() const
{
    return m_childNodes;
}
const std::vector<SceneNodeItem*>& SceneNode::GetChildItems() const
{
    return m_childItems;
}
int SceneNode::CalculerLevel() const
{
    int level = m_level;
    SceneNode *suiv = m_parent;
    while(suiv)
    {
        level+=suiv->m_level;
        suiv=suiv->m_parent;
    }
    return level;
}
void SceneNode::SetLevel(int level)
{
    m_level=level;
    UpdateLevel();
}
void SceneNode::MoveLevel(int level)
{
    m_level+=level;
    UpdateLevel();
}
void SceneNode::UpdateLevel()
{
    int anc = m_levelReel;
    m_levelReel=CalculerLevel();
    m_manager->ChangeNodeLevel(anc,m_levelReel,this);
    for(SceneNode* i : m_childNodes)
    {
        i->UpdateLevel();
    }
}
int SceneNode::GetLevel() const
{
    return m_level;
}
int SceneNode::GetRealLevel() const
{
    return m_levelReel;
}
void SceneNode::SetRelativeRotation(float rot)
{
    m_relative.rotation = rot;
    m_absolute.rotation = rot;
    if(m_parent)
    {
        m_absolute.rotation+=m_parent->m_absolute.rotation;
    }
    SetRelativePosition(m_relative.position);
    RotationChanged();
}
void SceneNode::SetRelativePosition(const sf::Vector2f& pos)
{
    m_relative.position=pos;
    m_absolute.position=pos;
    if(m_parent)
    {
        sf::Transform trans;
        trans.Rotate(m_parent->m_absolute.rotation);
        m_absolute.position.x=m_parent->m_absolute.position.x+trans.TransformPoint(m_relative.position).x;
        m_absolute.position.y=m_parent->m_absolute.position.y+trans.TransformPoint(m_relative.position).y;
    }
    PositionChanged();
}
void SceneNode::SetRelativeScale(const sf::Vector2f& scale)
{
    m_absolute.scale=m_relative.scale=scale;
    if(m_parent)
    {
        m_absolute.scale.x*=m_parent->m_absolute.scale.x;
        m_absolute.scale.y*=m_parent->m_absolute.scale.y;
    }
    ScaleChanged();
}
void SceneNode::PositionChanged()
{
    for(auto i : m_childNodes)
    {
        i->SetRelativePosition(i->m_relative.position);
    }
    for(auto i: m_childItems)
    {
        i->SetRelativePosition(i->GetRelativePosition());
        i->SetParentPosition(m_absolute.position);
    }
}
void SceneNode::RotationChanged()
{
    for(auto i : m_childItems)
    {
        i->SetParentRotation(m_absolute.rotation);
    }
    for(auto i: m_childNodes)
    {
        i->SetRelativeRotation(i->m_relative.rotation);
    }
}
void SceneNode::ScaleChanged()
{
    for(auto i : m_childNodes)
    {
        i->SetRelativeScale(i->m_relative.scale);
    }
    for(auto i : m_childItems)
    {
        i->SetParentScale(m_absolute.scale);
    }
}
void SceneNode::SetAbsolutePosition(const sf::Vector2f& pos)
{
    m_relative.position=m_absolute.position=pos;
    if(m_parent)
    {
        m_relative.position.x-=m_parent->m_absolute.position.x;
        m_relative.position.y-=m_parent->m_absolute.position.y;
    }
    PositionChanged();
}
void SceneNode::SetAbsoluteRotation(float rot)
{
    m_relative.rotation=m_absolute.rotation=rot;
    if(m_parent)
    {
        m_relative.rotation-=m_parent->m_absolute.rotation;
    }
    RotationChanged();
}
void SceneNode::SetAbsoluteScale(const sf::Vector2f& scale)
{
    m_relative.scale=m_absolute.scale=scale;
    if(m_parent)
    {
        m_relative.scale.x/=m_parent->m_absolute.scale.x;
        m_relative.scale.y/=m_parent->m_absolute.scale.y;
    }
    ScaleChanged();
}
void SceneNode::InformedParentBeforeDeletion(SceneNode* node)
{
    unsigned int taille = m_childNodes.size();
    for(unsigned int i=0;i<taille;++i)
    {
        if(m_childNodes[i]==node)
        {
            m_childNodes.erase(m_childNodes.begin()+i);
            return;
        }
    }
}
void SceneNode::SetRelativePosition(float x, float y)
{
    SetRelativePosition(sf::Vector2f(x,y));
}
void SceneNode::SetRelativeScale(float x, float y)
{
    SetRelativeScale(sf::Vector2f(x,y));
}
void SceneNode::SetAbsolutePosition(float x, float y)
{
    SetAbsolutePosition(sf::Vector2f(x,y));
}
void SceneNode::SetAbsoluteScale(float x, float y)
{
    SetAbsoluteScale(sf::Vector2f(x,y));
}
void SceneNode::RemoveItem(SceneNodeItem* item)
{
    unsigned int taille = m_childItems.size();
    for(unsigned int i=0;i<taille;++i)
    {
        if(m_childItems[i]==item)
        {
            m_childItems.erase(m_childItems.begin()+i);
            return;
        }
    }
}
