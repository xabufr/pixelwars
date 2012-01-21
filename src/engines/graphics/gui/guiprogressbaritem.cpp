#include "guiprogressbaritem.h"

GuiProgressBarItem::GuiProgressBarItem()
{

}

GuiProgressBarItem::~GuiProgressBarItem()
{

}

void GuiProgressBarItem::HandleEvent(const sf::Event&)
{
}

sf::Vector2f GuiProgressBarItem::GetSize() const
{
    return m_size;
}
void GuiProgressBarItem::SetSize(const sf::Vector2f& size)
{
    m_size = size;
    m_fond.SetSize(size);
}
void GuiProgressBarItem::SetMaximum(float max)
{
    SetRange(m_min, max);
}
void GuiProgressBarItem::SetMinimum(float min)
{
    SetRange(min, m_max);
}
void GuiProgressBarItem::SetRange(float min, float max)
{
    m_max=max;
    m_min=min;
    CalculerRatio();
}
void GuiProgressBarItem::SetValue(float val)
{
    m_value=val;
    CalculerRatio();
    m_valueShape.SetSize(sf::Vector2f(m_size.x*m_ratio,m_size.y));
}
void GuiProgressBarItem::Draw(sf::RenderWindow* app)
{
    if(!m_visible)
        return;
    app->Draw(m_fond);
    app->Draw(m_valueShape);
}
void GuiProgressBarItem::PositionChanged()
{
    m_fond.SetOrigin(-m_relative.position.x, -m_relative.position.y);
    m_fond.SetPosition(m_parent.position);
    m_valueShape.SetOrigin(-m_relative.position.x, -m_relative.position.y);
    m_valueShape.SetPosition(m_parent.position);
}
void GuiProgressBarItem::RotationChanged()
{
    m_fond.SetRotation(m_parent.rotation+m_relative.rotation);
    m_valueShape.SetRotation(m_parent.rotation+m_relative.rotation);
}
void GuiProgressBarItem::ScaleChanged()
{
    m_fond.SetScale(m_parent.scale.x*m_relative.scale.x,m_parent.scale.y*m_relative.scale.y);
    m_valueShape.SetScale(m_parent.scale.x*m_relative.scale.x,m_parent.scale.y*m_relative.scale.y);
}
void GuiProgressBarItem::CalculerRatio()
{
    m_ratio=m_value/(m_max-m_min);
}
void GuiProgressBarItem::SetFondColor(const sf::Color& col)
{
    m_fond.SetFillColor(col);
}
void GuiProgressBarItem::SetValueColor(const sf::Color& col)
{
    m_valueShape.SetFillColor(col);
}
