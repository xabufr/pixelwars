#include "scenenodespriteitem.h"
#include "core/logger.h"

SceneNodeSpriteItem::SceneNodeSpriteItem()
{
    m_spr.SetTexture(TextureManager::GetInstance()->Get("test.png"));
}

SceneNodeSpriteItem::~SceneNodeSpriteItem()
{
    //dtor
}
void SceneNodeSpriteItem::PositionChanged()
{
    m_spr.SetOrigin(-m_relative.position.x, -m_relative.position.y);
    m_spr.SetPosition(m_parent.position);
}
void SceneNodeSpriteItem::RotationChanged()
{
    m_spr.SetRotation(m_relative.rotation+m_parent.rotation);
}
void SceneNodeSpriteItem::ScaleChanged()
{
    m_spr.SetScale(m_parent.scale.x*m_relative.scale.x, m_parent.scale.y*m_relative.scale.y);
}
void SceneNodeSpriteItem::Draw(sf::RenderWindow* app)
{
    sf::Vector2f PosCam(app->GetView().GetCenter()-(app->GetView().GetSize()/2.f));
    sf::FloatRect rect(PosCam.x, PosCam.y, app->GetView().GetSize().x, app->GetView().GetSize().y);

    if(rect.Intersects(m_spr.GetGlobalBounds()))
    {
        app->Draw(m_spr);
    }

}
