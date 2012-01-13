#include "guitextitem.h"
#include <iostream>
GuiTextItem::GuiTextItem()
{
    //ctor
}
GuiTextItem::~GuiTextItem()
{
}
/** @brief ScaleChanged
  *
  * @todo: document this function
  */
void GuiTextItem::ScaleChanged()
{
}

/** @brief RotationChanged
  *
  * @todo: document this function
  */
void GuiTextItem::RotationChanged()
{
}

/** @brief PositionChanged
  *
  * @todo: document this function
  */
void GuiTextItem::PositionChanged()
{
    m_texte.SetOrigin(-m_relative.position.x, -m_relative.position.y);
    m_texte.SetPosition(m_parent.position);
    sf::Vector2f rel = -(m_relative.position*2.f) + sf::Vector2f(m_texte.GetGlobalBounds().Left, m_texte.GetGlobalBounds().Top)-m_parent.position;
    m_texte.SetOrigin(rel);
}

/** @brief Draw
  *
  * @todo: document this function
  */
void GuiTextItem::Draw(sf::RenderWindow* app)
{
    app->Draw(m_texte);
}

/** @brief HandleEvent
  *
  * @todo: document this function
  */
void GuiTextItem::HandleEvent(const sf::Event&)
{
}
void GuiTextItem::SetText(const sf::String& texte)
{
    m_texte.SetString(texte);
    PositionChanged();
}
void GuiTextItem::SetCharacterSize(unsigned int taille)
{
    m_texte.SetCharacterSize(taille);
    SetText(m_texte.GetString());
}
unsigned int GuiTextItem::GetCharacterSize() const
{
    return m_texte.GetCharacterSize();
}
float GuiTextItem::GetWidth() const
{
    return m_texte.GetLocalBounds().Width;
}
float GuiTextItem::GetHeigth() const
{
    return m_texte.GetLocalBounds().Height;
}
