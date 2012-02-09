#include "guislidernumberitem.h"
#include "../graphicalengine.h"
#include <iostream>
#include "../../../core/logger.h"

GuiSliderNumberItem::GuiSliderNumberItem()
{
    m_barre.SetSize(sf::Vector2f(100,2));
    m_slider.SetSize(sf::Vector2f(10,20));
    m_min = 0;
    m_max = 100;
    m_value=0;
    ValueChanged();
}

GuiSliderNumberItem::~GuiSliderNumberItem()
{
    //dtor
}
sf::Vector2f GuiSliderNumberItem::GetSize() const
{
    return sf::Vector2f(m_barre.GetSize().x, m_slider.GetSize().y);
}
void GuiSliderNumberItem::SetSize(const sf::Vector2f& size)
{
    m_barre.SetSize(sf::Vector2f(size.x,2));
    m_slider.SetSize(sf::Vector2f(m_slider.GetSize().x, size.y));
}
void GuiSliderNumberItem::SetRange(float min, float max)
{
    m_min = min;
    m_max = max;
    ValueChanged();
}
float GuiSliderNumberItem::GetValue() const
{
    return m_value;
}
void GuiSliderNumberItem::SetValue(float val)
{
    m_value=val;
    ValueChanged();
}
void GuiSliderNumberItem::SetColor(const sf::Color& col)
{
    m_couleur=col;
}
void GuiSliderNumberItem::SetFocusColor(const sf::Color& col)
{
    m_couleurFocus=col;
}
void GuiSliderNumberItem::SetBarColor(const sf::Color& col)
{
    m_barre.SetFillColor(col);
}
void GuiSliderNumberItem::HandleEvent(const sf::Event& event)
{
    if(!m_visible)
        return;
    if(event.Type == sf::Event::MouseButtonPressed)
    {
        if(m_slider.GetGlobalBounds().Contains(GraphicalEngine::GetInstance()->GetGuiManager()->GetMousePosition()))
        {
            m_focus=true;
            m_posClick = GraphicalEngine::GetInstance()->GetGuiManager()->GetMousePosition() - m_slider.GetPosition()+m_slider.GetOrigin();
        }
    }
    if(event.Type == sf::Event::MouseButtonReleased)
        m_focus=false;
    if(m_focus&&event.Type == sf::Event::MouseMoved)
    {
        m_slider.SetPosition(GraphicalEngine::GetInstance()->GetGuiManager()->GetMousePosition().x-m_posClick.x+m_slider.GetOrigin().x, m_slider.GetPosition().y);
        CalculerValue();
    }

}
void GuiSliderNumberItem::Draw(sf::RenderWindow* app)
{
    if(!m_visible)
        return;
    app->Draw(m_barre);
    if(m_focus)
        m_slider.SetFillColor(m_couleurFocus);
    else
        m_slider.SetFillColor(m_couleur);
    app->Draw(m_slider);
}
void GuiSliderNumberItem::PositionChanged()
{
    m_barre.SetOrigin(-m_relative.position.x, -m_relative.position.y);
    m_slider.SetOrigin(-m_relative.position.x, -m_relative.position.y);
    m_barre.SetPosition(m_parent.position.x, m_parent.position.y-1+m_slider.GetSize().y/2);
    ValueChanged();
}
void GuiSliderNumberItem::RotationChanged()
{
    m_barre.SetRotation(m_parent.rotation+m_relative.rotation);
    m_slider.SetRotation(m_parent.rotation+m_relative.rotation);
}
void GuiSliderNumberItem::ScaleChanged()
{
    m_barre.SetScale(m_parent.scale.x*m_relative.scale.x, m_parent.scale.y*m_relative.scale.y);
    m_slider.SetScale(m_parent.scale.x*m_relative.scale.x, m_parent.scale.y*m_relative.scale.y);
}
void GuiSliderNumberItem::ValueChanged()
{
    if(m_value>m_max)
        m_value=m_max;
    else if(m_value<m_min)
        m_value=m_min;
    float ratio = (m_value-m_min)/(m_max-m_min);
    m_slider.SetPosition(m_barre.GetPosition().x+m_barre.GetSize().x*ratio-m_slider.GetSize().x/2, m_barre.GetPosition().y-m_slider.GetSize().y/2);
    CallCallBack("value_changed");
}
void GuiSliderNumberItem::CalculerValue()
{
    float ratio = (m_slider.GetPosition().x-m_slider.GetOrigin().x+m_slider.GetSize().x*0.5-(m_barre.GetPosition().x-m_barre.GetOrigin().x))/m_barre.GetSize().x;
    m_value = ratio*(m_max-m_min)+m_min;
    ValueChanged();
}
