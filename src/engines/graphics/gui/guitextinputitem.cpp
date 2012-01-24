#include "guitextinputitem.h"
#include "../graphicalengine.h"
#include "core/logger.h"

GuiTextInputItem::GuiTextInputItem(): GuiItem(), m_maxSize(100,30)
{
    m_fond.SetSize(m_maxSize);
    m_pos.SetSize(sf::Vector2f(2,30));
    m_maxCharacters=255;
}

GuiTextInputItem::~GuiTextInputItem()
{
    //dtor
}
void GuiTextInputItem::HandleEvent(const sf::Event& event)
{
    if(!m_visible)
        return;
    if(m_focus)
    {
        if(event.Type == sf::Event::TextEntered)
        {
            if(event.Text.Unicode!=13&&event.Text.Unicode!=8&&m_maxCharacters>m_string.GetSize())
            {
                m_string+=event.Text.Unicode;
                CallCallBack("text_changed");
            }
            else if(event.Text.Unicode==8&&m_string.GetSize()!=0)
                m_string.Erase(m_string.GetSize()-1);
            else if(event.Text.Unicode==13)
            {
                m_focus=false;
                CallCallBack("focus_out");
            }

        }
    }
    if(event.Type == sf::Event::MouseButtonPressed)
    {
        if(m_fond.GetGlobalBounds().Contains(GraphicalEngine::GetInstance()->GetGuiManager()->GetMousePosition()))
        {
            if(!m_focus)
            {
                CallCallBack("focus_in");
                m_timer.Reset();
            }
            m_focus=true;
        }
        else
        {
            if(m_focus)
                CallCallBack("focus_out");
            m_focus=false;
        }
    }
}
sf::String GuiTextInputItem::GetText() const
{
    return m_string;
}
void GuiTextInputItem::SetText(const sf::String& txt)
{
    m_string=txt;
}
void GuiTextInputItem::SetMaxSize(const sf::Vector2f& size)
{
    m_maxSize=size;
    m_fond.SetSize(size);
    m_pos.SetSize(sf::Vector2f(2,size.y));
}
void GuiTextInputItem::SetCharacterSize(unsigned int taille)
{
    m_text.SetCharacterSize(taille);
}
void GuiTextInputItem::Draw(sf::RenderWindow* app)
{
    if(!m_visible)
        return;
    m_CalculerTexte();
    app->Draw(m_fond);
    app->Draw(m_text);
    if(m_focus&&m_timer.GetElapsedTime()>500)
    {
        app->Draw(m_pos);
        if(m_timer.GetElapsedTime()>1000)
            m_timer.Reset();
    }
}
void GuiTextInputItem::PositionChanged()
{
    m_text.SetOrigin(-m_relative.position.x, -m_relative.position.y);
    m_text.SetPosition(m_parent.position);
    //Correction d'un de SFML2 qui décalle le texte
    sf::Vector2f rel = -(m_relative.position*2.f) + sf::Vector2f(m_text.GetGlobalBounds().Left, m_text.GetGlobalBounds().Top)-m_parent.position;
    m_text.SetOrigin(rel);

    m_fond.SetOrigin(-m_relative.position.x, -m_relative.position.y);
    m_pos.SetOrigin(-m_relative.position.x, -m_relative.position.y);
    m_fond.SetPosition(m_parent.position);
    m_pos.SetPosition(m_parent.position);
}
void GuiTextInputItem::RotationChanged()
{
    m_text.SetRotation(m_parent.rotation+m_relative.rotation);
    m_fond.SetRotation(m_parent.rotation+m_relative.rotation);
}
void GuiTextInputItem::ScaleChanged()
{
    m_text.SetScale(m_parent.scale.x*m_relative.scale.x,m_parent.scale.y*m_relative.scale.y);
    m_fond.SetScale(m_parent.scale.x*m_relative.scale.x,m_parent.scale.y*m_relative.scale.y);
}
void GuiTextInputItem::m_CalculerTexte()
{
    m_text.SetString(m_string);
    if(m_text.GetGlobalBounds().Width > m_maxSize.x)
    {
        float width;
        sf::String tmpStr;
        unsigned int currIndex;
        for(currIndex=m_string.GetSize()-1, width=0;currIndex>=0&&width<m_maxSize.x;--currIndex)
        {
            tmpStr.Insert(0, m_string[currIndex]);
            width = m_CalculerLargeurTexte(tmpStr);
        }
        if(width>m_maxSize.x)
            tmpStr.Erase(0);
        m_text.SetString(tmpStr);
    }
    m_pos.SetPosition(m_text.GetPosition().x+m_text.GetGlobalBounds().Width, m_text.GetPosition().y);
}
sf::Vector2f GuiTextInputItem::GetSize() const
{
    return m_maxSize;
}
void GuiTextInputItem::SetBgColor(const sf::Color& col)
{
    m_fond.SetFillColor(col);
}
void GuiTextInputItem::SetColor(const sf::Color& col)
{
    m_text.SetColor(col);
}
unsigned int GuiTextInputItem::m_CalculerLargeurTexte(const sf::String & str)
{
    int width = 0;
    const sf::Font &font = sf::Font::GetDefaultFont();
    for(size_t i=0, taille = str.GetSize();i<taille;++i)
    {
        width+= font.GetGlyph(str[i], m_text.GetCharacterSize(), m_text.GetStyle()&sf::Text::Bold).Advance;
    }
    return width;
}
