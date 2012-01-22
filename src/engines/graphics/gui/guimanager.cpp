#include "guimanager.h"
#include "core/logger.h"
#include "../graphicalengine.h"

GuiManager::GuiManager(): SceneManager()
{
    delete m_rootSceneNode;
    Logger::Log()<<"Création du GuiManager"<<Logger::endl;
    m_rootSceneNode = new GuiNode(this);
    m_eventLockedBy=0;
}

GuiManager::~GuiManager()
{
    Logger::Log()<<"Destruction du GuiManager"<<Logger::endl;
}

GuiItem* GuiManager::AddItem(GuiItem* item)
{
    m_rootSceneNode->AddItem((SceneNodeItem*)item);
    return item;
}

void GuiManager::Draw()
{
    CalculerCamera();
    GraphicalEngine::GetInstance()->GetRenderWindow()->SetView(m_view);
    for(GuiNode *i : m_toRemove)
    {
        this->RemoveNode((SceneNode*)i);
    }
    m_toRemove.clear();
    SceneManager::Draw();
}

void GuiManager::HandleEvent(const sf::Event& event)
{
    if(event.Type == sf::Event::Resized)
    {
        CalculerCamera();
    }
    if(isEventLocked())
        m_eventLockedBy->HandleEventRecurse(event);
    else
    {
        for(auto it = m_nodesLevel.rbegin();it!=m_nodesLevel.rend();it++)
        {
            for(auto itTab = it->second.rbegin();itTab != it->second.rend();itTab++)
            {
                ((GuiNode*)(*itTab))->HandleEvent(event);
                if(isEventLocked())
                    return;
            }
        }
    }
}
GuiNode* GuiManager::GetRootNode()
{
    return (GuiNode*)m_rootSceneNode;
}
sf::Vector2f GuiManager::GetMousePosition()
{
    sf::RenderWindow *app = GraphicalEngine::GetInstance()->GetRenderWindow();
    return app->ConvertCoords(sf::Mouse::GetPosition(*app).x, sf::Mouse::GetPosition(*app).y, m_view);
}
void GuiManager::RemoveNode(SceneNode* node)
{
    //if(node==m_eventLockedBy)
        m_eventLockedBy=0;
    SceneManager::RemoveNode(node);
}
void GuiManager::LockEvent(GuiNode* node)
{
    m_eventLockedBy=node;
}
void GuiManager::UnlockEvent()
{
    m_eventLockedBy=0;
}
void GuiManager::AddToRemoveNode(GuiNode* node)
{
    m_toRemove.push_back(node);
}
void GuiManager::CalculerCamera()
{
    sf::RenderWindow *app = GraphicalEngine::GetInstance()->GetRenderWindow();
    m_view.SetSize(app->GetWidth(), app->GetHeight());
    m_view.SetCenter(app->GetWidth()/2, app->GetHeight()/2);
}
bool GuiManager::isEventLocked() const
{
    return m_eventLockedBy!=0;
}
GuiNode* GuiManager::GetEventLockerNode() const
{
    return m_eventLockedBy;
}
void GuiManager::ResetLevels()
{
    for(auto it = m_nodesLevel.begin();it!=m_nodesLevel.end();)
    {
        if(it->first==0)
        {
            it++;
            continue;
        }
        for(SceneNode* n: it->second)
        {
            m_nodesLevel[0].push_back(n);
            n->SetRealLevelNotUpdate(0);
        }
        it = m_nodesLevel.erase(it);
    }
}
