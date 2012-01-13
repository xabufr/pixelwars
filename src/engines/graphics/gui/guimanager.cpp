#include "guimanager.h"
#include "core/logger.h"
#include "../graphicalengine.h"

GuiManager::GuiManager()
{
    Logger::Log()<<"Création du GuiManager"<<Logger::endl;
    m_guiRootNode = new GuiNode(this);
    m_eventLockedBy=0;
}

GuiManager::~GuiManager()
{
    Logger::Log()<<"Destruction du GuiManager"<<Logger::endl;
}

GuiItem* GuiManager::AddItem(GuiItem* item)
{
    m_guiRootNode->AddItem((SceneNodeItem*)item);
    return item;
}

void GuiManager::Draw()
{
    for(GuiNode *i : m_toRemove)
    {
        this->RemoveNode((SceneNode*)i);
    }
    m_toRemove.clear();
    SceneManager::Draw();
}

void GuiManager::HandleEvent(const sf::Event& event)
{
    if(m_eventLockedBy) m_eventLockedBy->HandleEvent(event);
    else m_guiRootNode->HandleEvent(event);
}
GuiNode* GuiManager::GetRootNode()
{
    return m_guiRootNode;
}
sf::Vector2f GuiManager::GetMousePosition()
{
    sf::RenderWindow *app = GraphicalEngine::GetInstance()->GetRenderWindow();
    return app->ConvertCoords(sf::Mouse::GetPosition(*app).x, sf::Mouse::GetPosition(*app).y, app->GetDefaultView());
}
void GuiManager::RemoveNode(SceneNode* node)
{
    if(node==m_eventLockedBy)
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
