#include "projectile.h"
#include "bodytype.h"
Projectile::Projectile(b2World* world, b2Vec2 pos, b2Vec2 direction, float taille, float poids, float puissance) :
    m_puissance(puissance), m_poids(poids)
{
    b2BodyDef bd;
    bd.type=b2_dynamicBody;
    bd.position=pos;
    m_body=world->CreateBody(&bd);
    b2CircleShape shape;
    shape.m_radius=taille;
    b2FixtureDef fd;
    fd.density=poids;
    fd.shape=&shape;
    fd.friction=1;
    m_body->CreateFixture(&fd);
    m_body->ApplyLinearImpulse(direction, pos);
    m_body->SetUserData((void*) new BodyType(BodyTypeEnum::ProjectileE, this));

    m_node = GraphicalEngine::GetInstance()->GetSceneManager()->GetRootNode()->AddSceneNode();
    m_projectileItem = (SceneNodeCircleShapeItem*) m_node->AddItem(new SceneNodeCircleShapeItem);
    m_projectileItem->SetRadius(taille*10);
    m_projectileItem->SetRelativePosition(-taille*10, -taille*10);
    m_projectileItem->SetColor(sf::Color(255,255,255));
    m_node->SetAbsolutePosition(pos.x*10, -pos.y*10);

}

Projectile::~Projectile()
{
    m_body->GetWorld()->DestroyBody(m_body);
    GraphicalEngine::GetInstance()->GetSceneManager()->RemoveNode(m_node);
}
void Projectile::Update()
{
    m_node->SetAbsolutePosition(m_body->GetPosition().x*10, -m_body->GetPosition().y*10);
}
float Projectile::GetPuissance() const
{
    return m_puissance;
}
