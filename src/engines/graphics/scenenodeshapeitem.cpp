#include "scenenodeshapeitem.h"
#include <SFML2/Graphics/ConvexShape.hpp>

SceneNodeShapeItem::SceneNodeShapeItem()
{

}

SceneNodeShapeItem::~SceneNodeShapeItem()
{
    //dtor
}

void SceneNodeShapeItem::SetSize(const sf::Vector2f& size)
{
    m_shape.SetSize(size);
}
void SceneNodeShapeItem::SetSize(float x, float y)
{
    SetSize(sf::Vector2f(x,y));
}
void SceneNodeShapeItem::Draw(sf::RenderWindow *app)
{
    if(!m_visible)
        return;
    sf::Vector2f PosCam(app->GetView().GetCenter()-(app->GetView().GetSize()/2.f));
    sf::FloatRect rect(PosCam.x, PosCam.y, app->GetView().GetSize().x, app->GetView().GetSize().y);

    if(rect.Intersects(m_shape.GetGlobalBounds()))
        app->Draw(m_shape);
}
void SceneNodeShapeItem::PositionChanged()
{
    m_shape.SetOrigin(-m_relative.position.x,-m_relative.position.y);
    m_shape.SetPosition(m_parent.position);
}
void SceneNodeShapeItem::RotationChanged()
{
    m_shape.SetRotation(m_parent.rotation+m_relative.rotation);
}
void SceneNodeShapeItem::ScaleChanged()
{
    m_shape.SetScale(m_parent.scale.x*m_relative.scale.x, m_parent.scale.y*m_relative.scale.y);
}
void SceneNodeShapeItem::SetColor(const sf::Color& col)
{
    m_shape.SetFillColor(col);
}
void SceneNodeShapeItem::SetOutlineColor(const sf::Color& col)
{
    m_shape.SetOutlineColor(col);
}
void SceneNodeShapeItem::SetOutlineThickness(float width)
{
    m_shape.SetOutlineThickness(width);
}
sf::FloatRect SceneNodeShapeItem::GetGlobalBounds() const
{
    return m_shape.GetGlobalBounds();
}
sf::Vector2f SceneNodeShapeItem::GetSize() const
{
    return sf::Vector2f(m_shape.GetGlobalBounds().Width, m_shape.GetGlobalBounds().Height);
}
