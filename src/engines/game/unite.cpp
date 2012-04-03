#include "unite.h"
#include "bodytype.h"

Unite::Unite(b2World *world, b2Vec2 pos, SoundEngine *sEngine,BodyTypeEnum t): m_soundEngine(sEngine)
{
    m_node = GraphicalEngine::GetInstance()->GetSceneManager()->GetRootNode()->AddSceneNode();
    b2BodyDef bd;
    bd.type = b2_dynamicBody;
    bd.position=pos;
    bd.userData = (void*) new BodyType(t, (void*)this);
    m_body =world->CreateBody(&bd);
    m_fire = false;
    m_node->SetAbsolutePosition(pos.x*10, -pos.y*10);
    m_playingForwadSound = false;
}

Unite::~Unite()
{
    delete (BodyType*)m_body->GetUserData();
    m_body->GetWorld()->DestroyBody(m_body);
    GraphicalEngine::GetInstance()->GetSceneManager()->RemoveNode(m_node);
}
void Unite::Update()
{
    m_node->SetAbsolutePosition(m_body->GetPosition().x*10, -m_body->GetPosition().y*10);
    m_node->SetAbsoluteRotation(-m_body->GetAngle()*180/3.14);
}
void Unite::SubirDegats(float degat)
{
    m_vie-=degat;
}
bool Unite::EstVivant() const
{
    return m_vie>0;
}
b2Body* Unite::GetBody() const
{
    return m_body;
}
bool Unite::PeutTirer()
{
    if(m_timerFire.GetElapsedTime().AsMilliseconds()<m_tempRechargement)
        return false;
    return m_fire;
}
BodyTypeEnum Unite::GetType() const
{
    return BodyTypeEnum::UniteE;
}
