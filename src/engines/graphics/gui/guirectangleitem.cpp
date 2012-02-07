#include "guirectangleitem.h"

GuiRectangleItem::GuiRectangleItem()
{
    //ctor
}

GuiRectangleItem::~GuiRectangleItem()
{
    //dtor
}
void GuiRectangleItem::SetSize(const sf::Vector2f& size)
{
    m_shape.SetSize(size);
}
void GuiRectangleItem::SetSize(float x, float y)
{
    SetSize(sf::Vector2f(x,y));
}
void GuiRectangleItem::Draw(sf::RenderWindow *app)
{
    if(!m_visible)
        return;
    sf::Vector2f PosCam(app->GetView().GetCenter()-(app->GetView().GetSize()/2.f));
    sf::FloatRect rect(PosCam.x, PosCam.y, app->GetView().GetSize().x, app->GetView().GetSize().y);

    if(rect.Intersects(m_shape.GetGlobalBounds()))
        app->Draw(m_shape);
}
void GuiRectangleItem::PositionChanged()
{
    m_shape.SetOrigin(-m_relative.position.x,-m_relative.position.y);
    m_shape.SetPosition(m_parent.position);
}
void GuiRectangleItem::RotationChanged()
{
    m_shape.SetRotation(m_parent.rotation+m_relative.rotation);
}
void GuiRectangleItem::ScaleChanged()
{
    m_shape.SetScale(m_parent.scale.x*m_relative.scale.x, m_parent.scale.y*m_relative.scale.y);
}
void GuiRectangleItem::SetColor(const sf::Color& col)
{
    m_shape.SetFillColor(col);
}
void GuiRectangleItem::SetOutlineColor(const sf::Color& col)
{
    m_shape.SetOutlineColor(col);
}
void GuiRectangleItem::SetOutlineThickness(float width)
{
    m_shape.SetOutlineThickness(width);
}
sf::FloatRect GuiRectangleItem::GetGlobalBounds() const
{
    return m_shape.GetGlobalBounds();
}
sf::Vector2f GuiRectangleItem::GetSize() const
{
    return sf::Vector2f(m_shape.GetGlobalBounds().Width, m_shape.GetGlobalBounds().Height);
}
const sf::Color& GuiRectangleItem::GetColor() const
{
    return m_shape.GetFillColor();
}

void GuiRectangleItem::HandleEvent(const sf::Event&)
{

}
