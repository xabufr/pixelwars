#include "unite.h"
#include "bodytype.h"

Unite::Unite(b2World *world, b2Vec2 pos)
{
    m_node = GraphicalEngine::GetInstance()->GetSceneManager()->GetRootNode()->AddSceneNode();
    b2BodyDef bd;
    bd.type = b2_dynamicBody;
    bd.position=pos;
    bd.userData = (void*) new BodyType(BodyTypeEnum::UniteE, (void*)this);
    m_body =world->CreateBody(&bd);
}

Unite::~Unite()
{
    m_body->GetWorld()->DestroyBody(m_body);
    GraphicalEngine::GetInstance()->GetSceneManager()->RemoveNode(m_node);
}
void Unite::Update()
{
    m_node->SetAbsolutePosition(m_body->GetPosition().x*10, -m_body->GetPosition().y*10);
    m_node->SetAbsoluteRotation(-m_body->GetAngle()*180/3.14);
}
