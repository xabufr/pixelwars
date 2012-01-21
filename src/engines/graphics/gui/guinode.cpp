#include "guinode.h"
#include "guiitem.h"
#include "guiwindownode.h"
#include "guimanager.h"

GuiNode::GuiNode(SceneManager* mng, SceneNode* parent): SceneNode(mng,parent)
{
}
GuiNode::~GuiNode()
{
}

void GuiNode::HandleEvent(const sf::Event& event)
{

    for(SceneNodeItem* i : m_childItems)
    {
        ((GuiItem*)(i))->HandleEvent(event);
    }
}
void GuiNode::HandleEventRecurse(const sf::Event& event)
{
    GuiNode::HandleEvent(event);
    for(SceneNode* i : m_childNodes)
    {
        ((GuiNode*)(i))->HandleEvent(event);
    }
}
GuiWindowNode* GuiNode::AddWindow()
{
    GuiWindowNode *window = new GuiWindowNode(m_manager, this);
    AddSceneNode(window);
    return window;
}
void GuiNode::m_RemoveMeNextDraw()
{
    ((GuiManager*)m_manager)->AddToRemoveNode(this);
}
