#include "scenenodecircleshapeitem.h"

SceneNodeCircleShapeItem::SceneNodeCircleShapeItem()
{
    //ctor
}

SceneNodeCircleShapeItem::~SceneNodeCircleShapeItem()
{
    //dtor
}
/** @brief SetRadius
  *
  * @todo: document this function
  */
void SceneNodeCircleShapeItem::SetRadius(float rad)
{
    m_shape.SetRadius(rad);
}

/** @brief SetColor
  *
  * @todo: document this function
  */
void SceneNodeCircleShapeItem::SetColor(const sf::Color& col)
{
    m_shape.SetFillColor(col);
}

/** @brief SetOutlineColor
  *
  * @todo: document this function
  */
void SceneNodeCircleShapeItem::SetOutlineColor(const sf::Color& col)
{
    m_shape.SetOutlineColor(col);
}

/** @brief SetOutlineThickness
  *
  * @todo: document this function
  */
void SceneNodeCircleShapeItem::SetOutlineThickness(float tick)
{
    m_shape.SetOutlineThickness(tick);
}

/** @brief GetGlobalBounds
  *
  * @todo: document this function
  */
sf::FloatRect SceneNodeCircleShapeItem::GetGlobalBounds() const
{
    return m_shape.GetGlobalBounds();
}

/** @brief Draw
  *
  * @todo: document this function
  */
void SceneNodeCircleShapeItem::Draw(sf::RenderWindow* app)
{
    app->Draw(m_shape);
}

/** @brief PositionChanged
  *
  * @todo: document this function
  */
void SceneNodeCircleShapeItem::PositionChanged()
{
    m_shape.SetOrigin(-m_relative.position);
    m_shape.SetPosition(m_parent.position);
}

/** @brief RotationChanged
  *
  * @todo: document this function
  */
void SceneNodeCircleShapeItem::RotationChanged()
{
    m_shape.SetRotation(m_parent.rotation+m_relative.rotation);
}

/** @brief ScaleChanged
  *
  * @todo: document this function
  */
void SceneNodeCircleShapeItem::ScaleChanged()
{
    m_shape.SetScale(m_parent.scale.x*m_relative.scale.x, m_parent.scale.y*m_relative.scale.y);
}
