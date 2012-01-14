#include "uniteterrestre.h"
#include "bodytype.h"

UniteTerrestre::UniteTerrestre(b2World* world): Unite(world, b2Vec2(15,0))
{
    b2PolygonShape polyShape;
    polyShape.SetAsBox(2.5, 1);
    b2FixtureDef fd;
    fd.shape = &polyShape;
    fd.density = 1.f;
    fd.friction = 0.5;
    m_body->CreateFixture(&fd);

    b2CircleShape circleShape;
    circleShape.m_radius = 1;
    fd.shape = &circleShape;
    fd.density = 5.f;
    fd.friction = 3000.f;
    b2BodyDef bd;
    bd.type = b2_dynamicBody;
    bd.userData= (void*) new BodyType(BodyTypeEnum::UniteE, (void*)this);
    bd.position=b2Vec2(-2, -1)+m_body->GetPosition();
    m_roue1 = m_body->GetWorld()->CreateBody(&bd);
    m_roue1->CreateFixture(&fd);
    b2WheelJointDef jd;
    b2Vec2 axis(0,1);
    jd.Initialize(m_body, m_roue1, m_roue1->GetPosition(), axis);
    jd.maxMotorTorque=200.f;
    jd.motorSpeed=0.f;
    jd.frequencyHz=4.f;
    jd.enableMotor=true;

    jointure1=(b2WheelJoint*)m_body->GetWorld()->CreateJoint(&jd);

    bd.position.x=2.f+m_body->GetPosition().x;
    m_roue2=m_body->GetWorld()->CreateBody(&bd);
    m_roue2->CreateFixture(&fd);

    jd.Initialize(m_body, m_roue2, m_roue2->GetPosition(), axis);
    jd.maxMotorTorque=200.f;
    jd.motorSpeed=0;
    jd.frequencyHz=4.f;
    jd.enableMotor=true;
    jointure2=(b2WheelJoint*)m_body->GetWorld()->CreateJoint(&jd);


    m_itemBody = (SceneNodeShapeItem*) m_node->AddItem(new SceneNodeShapeItem);
    m_itemBody->SetSize(50, 20);
    m_itemBody->SetRelativePosition(-25, -10);
    m_itemBody->SetColor(sf::Color(255,0,0));

    m_nodeRoue1 = m_node->AddSceneNode();
    m_nodeRoue2 = m_node->AddSceneNode();
    m_itemRoue1 = (SceneNodeSpriteItem*) m_nodeRoue1->AddItem(new SceneNodeSpriteItem);
    m_itemRoue2 = (SceneNodeSpriteItem*) m_nodeRoue2->AddItem(new SceneNodeSpriteItem);
    m_itemRoue1->SetImage("image_transport_369.png");
    m_itemRoue2->SetImage("image_transport_369.png");


    m_nodeRoue1->SetRelativePosition(-20,10);
    m_nodeRoue2->SetRelativePosition(20,10);
    m_itemRoue1->SetRelativePosition(-10, -10);
    m_itemRoue2->SetRelativePosition(-10, -10);

    m_node->SetAbsolutePosition(0,0);
}

UniteTerrestre::~UniteTerrestre()
{
    //dtor
}
void UniteTerrestre::Update()
{
    Unite::Update();
    m_nodeRoue1->SetAbsoluteRotation(-m_roue1->GetAngle()*180/3.14);
    m_nodeRoue2->SetAbsoluteRotation(-m_roue2->GetAngle()*180/3.14);
}
void UniteTerrestre::Deplacer(const UnitInput& input)
{
    if(input.droite)
    {
        jointure1->SetMotorSpeed(-5);
        jointure2->SetMotorSpeed(-10);
    }
    else if(input.gauche)
    {
        jointure1->SetMotorSpeed(10);
        jointure2->SetMotorSpeed(5);
    }
    else
    {
        Stop();
    }
}
void UniteTerrestre::Stop()
{
    jointure1->SetMotorSpeed(0);
    jointure2->SetMotorSpeed(0);
}
