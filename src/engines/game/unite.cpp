#include "unite.h"
#include "bodytype.h"

Unite::Unite(b2World *world)
{
    m_node = GraphicalEngine::GetInstance()->GetSceneManager()->GetRootNode()->AddSceneNode();
    b2BodyDef bd;
    bd.type = b2_dynamicBody;
    bd.userData = (void*) new BodyType(BodyTypeEnum::Unite, (void*)this);
    m_body =world->CreateBody(&bd);
}

Unite::~Unite()
{
    //dtor
}
