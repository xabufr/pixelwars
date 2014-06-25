#include "uniteterrestre.h"
#include "bodytype.h"
#include "projectile.h"
#include "../../core/exception.h"
#include "../../core/trigo.h"
#include "soundproprities.h"

UniteTerrestre::UniteTerrestre(b2World* world, b2Vec2 pos, SoundEngine *sEngine, const std::string& uid): Unite(world, pos, sEngine),
    m_param(UniteTerrestreModel::GetInstance()->Get(uid))
{
    UniteTerrestreModel *factory = UniteTerrestreModel::GetInstance();
    Assert(m_param);
    m_tailleCanon.Set(m_param->tailleCanon, 0.2);

    b2PolygonShape polyShape;
    polyShape.SetAsBox(factory->GetTailleUnit().x, factory->GetTailleUnit().y);
    b2FixtureDef fd;
    fd.shape = &polyShape;
    fd.density = m_param->poidsCorp;
    fd.friction = 0.5;
    m_body->CreateFixture(&fd);

    b2CircleShape circleShape;
    circleShape.m_radius = factory->GetTailleRoue();
    fd.shape = &circleShape;
    fd.density = 5.f;
    fd.friction = factory->GetFrictionRoue();
    b2BodyDef bd;
    bd.type = b2_dynamicBody;
    bd.userData= (void*) new BodyType(BodyTypeEnum::UniteE, (void*)this);
    bd.position=m_body->GetPosition()-factory->GetTailleUnit();
    m_roue1 = m_body->GetWorld()->CreateBody(&bd);
    m_roue1->CreateFixture(&fd);
    b2WheelJointDef jd;
    b2Vec2 axis(0,1);
    jd.Initialize(m_body, m_roue1, m_roue1->GetPosition(), axis);
    jd.maxMotorTorque= factory->GetPuissanceRoue();
    jd.motorSpeed=1.f;
    jd.frequencyHz=4.f;
    jd.enableMotor=true;

    jointure1=(b2WheelJoint*)m_body->GetWorld()->CreateJoint(&jd);

    bd.position.x=2.f+m_body->GetPosition().x;
    m_roue2=m_body->GetWorld()->CreateBody(&bd);
    m_roue2->CreateFixture(&fd);

    jd.Initialize(m_body, m_roue2, m_roue2->GetPosition(), axis);
    jd.maxMotorTorque=factory->GetPuissanceRoue();
    jd.motorSpeed=0;
    jd.frequencyHz=4.f;
    jd.enableMotor=true;
    jointure2=(b2WheelJoint*)m_body->GetWorld()->CreateJoint(&jd);

    bd.position=b2Vec2(0, factory->GetTailleUnit().y)+m_body->GetPosition();
    m_tourelle = m_body->GetWorld()->CreateBody(&bd);
    polyShape.SetAsBox(m_tailleCanon.x, m_tailleCanon.y, b2Vec2(m_tailleCanon.x, 0),0);
    fd.shape = &polyShape;
    fd.friction=0.5;
    fd.density=1.f;
    m_tourelle->CreateFixture(&fd);

    b2RevoluteJointDef revoluteJd;
    revoluteJd.Initialize(m_body, m_tourelle, m_tourelle->GetPosition());
    revoluteJd.maxMotorTorque=100000;
    revoluteJd.motorSpeed=0;
    revoluteJd.enableMotor=true;
    revoluteJd.enableLimit=true;
    revoluteJd.lowerAngle=0.1;
    revoluteJd.upperAngle=3.13;

    m_jointureTourelle = (b2RevoluteJoint*)m_body->GetWorld()->CreateJoint(&revoluteJd);


    m_itemBody = (SceneNodeShapeItem*) m_node->AddItem(new SceneNodeShapeItem);
    m_itemBody->SetSize(factory->GetTailleUnit().x*20, factory->GetTailleUnit().y*20);
    m_itemBody->SetRelativePosition(-factory->GetTailleUnit().x*10, -factory->GetTailleUnit().y*10);
    m_itemBody->SetColor(sf::Color(255,0,0));

    m_nodeRoue1 = m_node->AddSceneNode();
    m_nodeRoue2 = m_node->AddSceneNode();
    m_itemRoue1 = (SceneNodeSpriteItem*) m_nodeRoue1->AddItem(new SceneNodeSpriteItem);
    m_itemRoue2 = (SceneNodeSpriteItem*) m_nodeRoue2->AddItem(new SceneNodeSpriteItem);
    m_itemRoue1->SetImage("data/roue.png");
    m_itemRoue2->SetImage("data/roue.png");


    m_nodeRoue1->SetRelativePosition(-factory->GetTailleUnit().x*10,factory->GetTailleUnit().y*10);
    m_nodeRoue2->SetRelativePosition(factory->GetTailleUnit().x*10,factory->GetTailleUnit().y*10);
    m_itemRoue1->SetRelativePosition(-factory->GetTailleRoue()*10, -factory->GetTailleRoue()*10);
    m_itemRoue2->SetRelativePosition(-factory->GetTailleRoue()*10, -factory->GetTailleRoue()*10);

    m_nodeTourelle=m_node->AddSceneNode();
    m_nodeTourelle->SetRelativePosition(0,-factory->GetTailleUnit().y*10);
    m_itemTourelle = (SceneNodeShapeItem*) m_nodeTourelle->AddItem(new SceneNodeShapeItem);
    m_itemTourelle->SetSize(m_tailleCanon.x*20, m_tailleCanon.y*20);
    m_itemTourelle->SetRelativePosition(0,-m_tailleCanon.y*10);

    m_barreVie = (GuiProgressBarItem*) m_node->AddItem(new GuiProgressBarItem);
    m_barreVie->SetRange(0, m_param->poidsCorp*10);
    m_barreVie->SetValue(m_param->poidsCorp*10);
    m_barreVie->SetValueColor(sf::Color(0,0,255));
    m_barreVie->SetFondColor(sf::Color(255,0,0));
    m_barreVie->SetSize(sf::Vector2f(50,5));
    m_barreVie->SetRelativePosition(-25, factory->GetTailleUnit().y*20+20);

    m_timerFire.restart();
    float coef = float(factory->GetReloadTimeMax()-factory->GetReloadTimeMin())/
                          (factory->GetCanonMax()-factory->GetCanonMin());
    m_tempRechargement= coef*m_param->tailleCanon;
    m_tempRechargement+= factory->GetReloadTimeMax()-coef*factory->GetCanonMax();
    m_vie=m_param->poidsCorp*10;

    m_sonAvance = m_soundEngine->PlaySound("data/sons/terrestre.wav");
    m_soundEngine->RemoveWhenFinished(m_sonAvance, false);
    m_soundEngine->GetSound(m_sonAvance)->stop();
    m_soundEngine->GetSound(m_sonAvance)->setLoop(true);

    m_sonTourelle = m_soundEngine->PlaySound("data/sons/tourelle.wav");
    m_soundEngine->RemoveWhenFinished(m_sonTourelle, false);
    m_soundEngine->GetSound(m_sonTourelle)->stop();
    m_soundEngine->GetSound(m_sonTourelle)->setLoop(true);

    m_tourelleBouge = false;
}

UniteTerrestre::~UniteTerrestre()
{
    ParticleParameters params;
    params.colorFromList=true;
    params.colorList.push_back(sf::Color(128,128,128));
    params.colorList.push_back(sf::Color(255,149,0));
    params.colorList.push_back(m_itemBody->GetColor());
    params.level=-1;
    params.minAngle=0;
    params.maxAngle=360;
    params.minPower=250;
    params.maxPower=1000;
    params.timeToLive=1500;
    params.number=1000;
    params.minSize=1;
    params.maxSize=2;
    params.position=m_node->GetAbsoluteInformations().position;
    GraphicalEngine::GetInstance()->GetSceneManager()->GetParticleManager()->AddParticleSystem(params);
    m_roue1->GetWorld()->DestroyBody(m_roue1);
    m_roue2->GetWorld()->DestroyBody(m_roue2);
    m_tourelle->GetWorld()->DestroyBody(m_tourelle);
    m_soundEngine->RemoveWhenFinished(m_sonAvance, true);
    m_soundEngine->GetSound(m_sonAvance)->stop();
    m_soundEngine->RemoveWhenFinished(m_sonTourelle, true);
    m_soundEngine->GetSound(m_sonTourelle)->stop();
}
void UniteTerrestre::Update()
{
    Unite::Update();
    m_nodeRoue1->SetAbsoluteRotation(-m_roue1->GetAngle()*180/3.14);
    m_nodeRoue2->SetAbsoluteRotation(-m_roue2->GetAngle()*180/3.14);
    m_nodeTourelle->SetAbsolutePosition(m_tourelle->GetPosition().x*10, -m_tourelle->GetPosition().y*10);
    m_nodeTourelle->SetAbsoluteRotation(-m_tourelle->GetAngle()*180/3.14);
    m_barreVie->SetValue(m_vie);
}
#include "../../core/logger.h"
void UniteTerrestre::Deplacer(const UnitInput& input)
{
    if((input.droite||input.gauche)&&!m_playingForwadSound)
    {
        m_playingForwadSound=true;
        m_soundEngine->FadeIn(m_sonAvance, 0.25);
    }
    else if(m_playingForwadSound&&!input.droite&&!input.gauche)
    {
        m_playingForwadSound=false;
        m_soundEngine->FadeOut(m_sonAvance, 0.25);
    }
    if(input.droite)
    {
        jointure1->SetMotorSpeed(-6.28);
        jointure2->SetMotorSpeed(-6.28);
    }
    else if(input.gauche)
    {
        jointure1->SetMotorSpeed(6.28);
        jointure2->SetMotorSpeed(6.28);
    }
    else
    {
        Stop();
    }
    if(!m_tourelleBouge&&(input.haut||input.bas))
    {
        m_tourelleBouge=true;
        m_soundEngine->FadeIn(m_sonTourelle, 0.25);
    }
    else if(m_tourelleBouge&&!input.haut&&!input.bas)
    {
        m_tourelleBouge=false;
        m_soundEngine->FadeOut(m_sonTourelle, 0.15);
    }
    if(input.haut)
    {
        m_jointureTourelle->SetMotorSpeed(1);
    }
    else if(input.bas)
    {
        m_jointureTourelle->SetMotorSpeed(-1);
    }
    else
        m_jointureTourelle->SetMotorSpeed(0);
    m_fire = input.tirer;
}
void UniteTerrestre::Stop()
{
    jointure1->SetMotorSpeed(0);
    jointure2->SetMotorSpeed(0);
    m_jointureTourelle->SetMotorSpeed(0);
    m_playingForwadSound=false;
    m_soundEngine->FadeOut(m_sonAvance, 0.2);
}
Projectile* UniteTerrestre::Tirer()
{
    b2Vec2 pos, direction;
    float taille, poids, puissance;

    m_timerFire.restart();
    taille=0.2;
    float angle = -m_tourelle->GetAngle()*180/3.14;
    sf::Transform trans;
    trans.rotate(angle);
    sf::Vector2f posRel = trans.transformPoint(m_tailleCanon.x*20+taille*10+1,0);
    posRel.x*=0.1;
    posRel.y*=-0.1;

    pos.x=m_tourelle->GetPosition().x+posRel.x;
    pos.y=m_tourelle->GetPosition().y+posRel.y;
    direction.x = posRel.x*10;
    direction.y = posRel.y*10;


    puissance=m_param->puissanceProjectiles;
    poids=UniteTerrestreModel::GetInstance()->GetDensiteProjectile()*puissance*0.5;
    m_tourelle->ApplyLinearImpulse(-poids*0.2*direction, pos);

    direction.x *= 9;
    direction.y *= 9;

    return new Projectile(m_body->GetWorld(), pos, direction, taille, poids, puissance);
}
void UniteTerrestre::SetColor(const sf::Color& col)
{
    m_itemBody->SetColor(col);
}
float UniteTerrestre::GetTailleCanon() const
{
    return m_param->tailleCanon;
}
float UniteTerrestre::GetShootAngle() const
{
    return Trigo::ToDeg(m_tourelle->GetAngle());
}
float UniteTerrestre::GetPuissanceExpulsion() const
{
    return m_param->tailleCanon;
}
const std::string UniteTerrestre::GetShootSound() const
{
    return SoundProprities::GetInstance()->GetUniteTerrestreSound(this);
}
const std::string UniteTerrestre::GetDeathSound() const
{
    return SoundProprities::GetInstance()->GetUniteTerrestreDestructionSound(this);
}
const UniteTerrestreParameters* UniteTerrestre::GetParams() const
{
    return m_param;
}
