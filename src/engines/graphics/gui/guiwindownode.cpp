#include "guiwindownode.h"
#include "guiitem.h"
#include "guimanager.h"

GuiWindowNode::GuiWindowNode(SceneManager* mng, SceneNode* parent): GuiNode(mng,parent)
{
    m_windowShape = new SceneNodeShapeItem;
    m_windowTitle = new GuiTextItem;

    m_windowTitle->SetRelativePosition(0,0);
    m_windowTitle->SetCharacterSize(10);

    AddItem(m_windowShape);
    AddItem(m_windowTitle);

    m_windowShape->SetSize(100,100);
    m_windowShape->SetColor(sf::Color(207,204,201));
    m_windowShape->SetOutlineColor(sf::Color(100,100,100));
    m_windowShape->SetOutlineThickness(1.f);
    m_windowShape->SetRelativePosition(0,0);


    m_moving=false;
    m_closable=false;
    m_btnClose=0;
}
GuiWindowNode::~GuiWindowNode()
{
}
void GuiWindowNode::HandleEvent(const sf::Event& event)
{
    if(event.Type==sf::Event::MouseButtonPressed)
    {
        if(m_windowShape->GetGlobalBounds().Contains(GuiManager::GetMousePosition()))
        {
            m_moving=true;
            m_posClickMoving = GuiManager::GetMousePosition() - m_absolute.position;
            ((GuiManager*)m_manager)->LockEvent(this);
        }
    }
    if(event.Type==sf::Event::MouseButtonReleased)
    {
        if(m_moving)
            ((GuiManager*)m_manager)->UnlockEvent();
        m_moving=false;
    }
    if(event.Type==sf::Event::MouseMoved&&m_moving)
    {
        SetAbsolutePosition(GuiManager::GetMousePosition()-m_posClickMoving);
    }
    for(SceneNodeItem* i : m_childItems)
    {
        if(i!=m_windowShape)
            ((GuiItem*)(i))->HandleEvent(event);
    }
    for(SceneNode* i : m_childNodes)
    {
        ((GuiNode*)(i))->HandleEvent(event);
    }
}
void GuiWindowNode::SetClosable(bool clos)
{
    m_closable=clos;
    ClosableChanged();
}

bool GuiWindowNode::IsClosable() const
{
    return m_closable;
}

void GuiWindowNode::ClosableChanged()
{
    if(m_closable&&!m_btnClose)
    {
        m_btnClose = new GuiButtonItem;
        m_btnClose->SetText("x");
        m_btnClose->SetData("window", (void*)this);
        m_btnClose->SetCallBack("clicked", GuiWindowNode::CloseWindowCallBack);
        m_btnClose->SetNormalColor(sf::Color(0,0,0),sf::Color(0,0,0,0));
        m_btnClose->SetMouseOverColor(sf::Color(255,0,0),sf::Color(0,0,0,0));

        this->AddItem(m_btnClose);
    }
    else if(!m_closable&&m_btnClose)
    {
        this->RemoveItem(m_btnClose);
        delete m_btnClose;
        m_btnClose=0;
    }
}
void GuiWindowNode::CloseWindowCallBack(GuiItem* item)
{
    ((GuiWindowNode*)item->GetData("window"))->m_RemoveMeNextDraw();
}
void GuiWindowNode::SetWindowTitle(const sf::String& title)
{
    m_windowTitle->SetText(title);
    sf::Vector2f t(m_windowTitle->GetWidth(), m_windowTitle->GetHeigth());
    if(m_closable)
    {
        t.x+=m_btnClose->GetWidth();
        t.y=(t.y<m_btnClose->GetHeight())?m_btnClose->GetHeight():t.y;
    }
    m_windowShape->SetSize(t);
    if(m_closable)
        m_btnClose->SetRelativePosition(m_windowShape->GetGlobalBounds().Width-m_btnClose->GetWidth()-3, 0);
}
