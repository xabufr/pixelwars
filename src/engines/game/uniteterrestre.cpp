#include "uniteterrestre.h"
#include "bodytype.h"
#include "projectile.h"

UniteTerrestre::UniteTerrestre(b2World* world): Unite(world, b2Vec2(15,-20))
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
    fd.friction = 30000.f;
    b2BodyDef bd;
    bd.type = b2_dynamicBody;
    bd.userData= (void*) new BodyType(BodyTypeEnum::UniteE, (void*)this);
    bd.position=b2Vec2(-2, -1)+m_body->GetPosition();
    m_roue1 = m_body->GetWorld()->CreateBody(&bd);
    m_roue1->CreateFixture(&fd);
    b2WheelJointDef jd;
    b2Vec2 axis(0,1);
    jd.Initialize(m_body, m_roue1, m_roue1->GetPosition(), axis);
    jd.maxMotorTorque=2000.f;
    jd.motorSpeed=1.f;
    jd.frequencyHz=4.f;
    jd.enableMotor=true;

    jointure1=(b2WheelJoint*)m_body->GetWorld()->CreateJoint(&jd);

    bd.position.x=2.f+m_body->GetPosition().x;
    m_roue2=m_body->GetWorld()->CreateBody(&bd);
    m_roue2->CreateFixture(&fd);

    jd.Initialize(m_body, m_roue2, m_roue2->GetPosition(), axis);
    jd.maxMotorTorque=2000.f;
    jd.motorSpeed=0;
    jd.frequencyHz=4.f;
    jd.enableMotor=true;
    jointure2=(b2WheelJoint*)m_body->GetWorld()->CreateJoint(&jd);

    bd.position=b2Vec2(0, 1)+m_body->GetPosition();
    m_tourelle = m_body->GetWorld()->CreateBody(&bd);
    polyShape.SetAsBox(3, 0.2, b2Vec2(3, 0),0);
    fd.shape = &polyShape;
    fd.friction=0.5;
    fd.density=1.f;
    m_tourelle->CreateFixture(&fd);

    b2RevoluteJointDef revoluteJd;
    revoluteJd.Initialize(m_body, m_tourelle, m_tourelle->GetPosition());
    revoluteJd.maxMotorTorque=1000;
    revoluteJd.motorSpeed=0;
    revoluteJd.enableMotor=true;
    revoluteJd.enableLimit=true;
    revoluteJd.lowerAngle=0;
    revoluteJd.upperAngle=3.14;

    m_jointureTourelle = (b2RevoluteJoint*)m_body->GetWorld()->CreateJoint(&revoluteJd);


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

    m_nodeTourelle=m_node->AddSceneNode();
    m_nodeTourelle->SetRelativePosition(0,-10);
    m_itemTourelle = (SceneNodeShapeItem*) m_nodeTourelle->AddItem(new SceneNodeShapeItem);
    m_itemTourelle->SetSize(60,4);
    m_itemTourelle->SetRelativePosition(0,-2);

    m_node->SetAbsolutePosition(0,0);

    m_fire = false;
    m_timerFire.Reset();
    m_tempRechargement = 1000;
    m_vie=100;
}

UniteTerrestre::~UniteTerrestre()
{
    m_roue1->GetWorld()->DestroyBody(m_roue1);
    m_roue2->GetWorld()->DestroyBody(m_roue2);
    m_tourelle->GetWorld()->DestroyBody(m_tourelle);
}
void UniteTerrestre::Update()
{
    Unite::Update();
    m_nodeRoue1->SetAbsoluteRotation(-m_roue1->GetAngle()*180/3.14);
    m_nodeRoue2->SetAbsoluteRotation(-m_roue2->GetAngle()*180/3.14);
    m_nodeTourelle->SetAbsolutePosition(m_tourelle->GetPosition().x*10, -m_tourelle->GetPosition().y*10);
    m_nodeTourelle->SetAbsoluteRotation(-m_tourelle->GetAngle()*180/3.14);
}
void UniteTerrestre::Deplacer(const UnitInput& input)
{
    if(input.droite)
    {
        jointure1->SetMotorSpeed(-5);
        jointure2->SetMotorSpeed(-5);
    }
    else if(input.gauche)
    {
        jointure1->SetMotorSpeed(5);
        jointure2->SetMotorSpeed(5);
    }
    else
    {
        Stop();
    }
    if(input.haut)
    {
        m_jointureTourelle->SetMotorSpeed(1);
    }
    else if(input.bas)
    {
        m_jointureTourelle->SetMotorSpeed(-1);
    }
    m_fire = input.tirer;
}
void UniteTerrestre::Stop()
{
    jointure1->SetMotorSpeed(0);
    jointure2->SetMotorSpeed(0);
    m_jointureTourelle->SetMotorSpeed(0);
}
bool UniteTerrestre::PeutTirer()
{
    if(m_tempRechargement>m_timerFire.GetElapsedTime())
        return false;
    return m_fire;
}
Projectile* UniteTerrestre::Tirer()
{
    b2Vec2 pos, direction;
    float taille, poids, puissance;

    m_timerFire.Reset();

    float angle = -m_tourelle->GetAngle()*180/3.14;
    sf::Transform trans;
    trans.Rotate(angle);
    sf::Vector2f posRel = trans.TransformPoint(63,0);
    posRel.x*=0.1;
    posRel.y*=-0.1;

    pos.x=m_tourelle->GetPosition().x+posRel.x;
    pos.y=m_tourelle->GetPosition().y+posRel.y;
    direction.x = posRel.x*20;
    direction.y = posRel.y*20;
    taille=0.2;
    poids=50.f;
    puissance=10.f;
    m_tourelle->ApplyLinearImpulse(-direction, pos);

    return new Projectile(m_body->GetWorld(), pos, direction, taille, poids, puissance);
}
