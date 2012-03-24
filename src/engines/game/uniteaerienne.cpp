#include "uniteaerienne.h"
#include "../../core/trigo.h"
#include "projectile.h"

UniteAerienne::UniteAerienne(b2World* world, b2Vec2 pos): Unite(world, pos, BodyTypeEnum::UniteAirE)
{
    b2PolygonShape sh;
    sh.SetAsBox(1, 0.25);
    b2FixtureDef fd;
    fd.density=2;
    fd.friction=1;
    fd.restitution=0;
    fd.shape = &sh;
    m_body->CreateFixture(&fd);
    m_corpsAvion = new SceneNodeShapeItem;
    m_corpsAvion->SetSize(20, 5);
    m_node->AddItem(m_corpsAvion);
    m_corpsAvion->SetRelativePosition(-10, -2.5);
    m_vie=1;
    m_acceleration = 80;
    m_rotationVelocity = 0.5;
    m_minVelocity = 20;
    m_body->SetAngularDamping(10);
    m_body->SetLinearDamping(0.5);
    m_tempRechargement = 33;
}

UniteAerienne::~UniteAerienne()
{
    ParticleParameters params;
    params.colorFromList=true;
    params.colorList.push_back(sf::Color(128,128,128));
    params.colorList.push_back(sf::Color(255,149,0));
    params.colorList.push_back(m_corpsAvion->GetColor());
    params.level=-1;
    params.minAngle=0;
    params.maxAngle=360;
    params.minPower=250;
    params.maxPower=1250;
    params.timeToLive=2000;
    params.number=1500;
    params.minSize=0.5;
    params.maxSize=2;
    params.position=m_node->GetAbsoluteInformations().position;
    GraphicalEngine::GetInstance()->GetSceneManager()->GetParticleManager()->AddParticleSystem(params);
}


void UniteAerienne::Deplacer(const UnitInput& in)
{
    m_forces.descend=in.bas;
    m_forces.monte=in.haut;
    m_forces.avant=in.droite;
    m_forces.arriere=in.gauche;
    m_fire=in.tirer;
}


void UniteAerienne::Stop()
{
    m_forces.descend=false;
    m_forces.monte=false;
    m_forces.avant=false;
    m_forces.arriere=false;
    m_fire=false;
}

Projectile* UniteAerienne::Tirer()
{
    m_timerFire.Restart();
    sf::Transform t;
    t.Rotate(Trigo::ToDeg(m_body->GetAngle()));
    sf::Vector2f d = t.TransformPoint(1.3, 0);
    sf::Vector2f pos = sf::Vector2f(m_body->GetPosition().x, m_body->GetPosition().y)+d;
    return new Projectile(m_body->GetWorld(), b2Vec2(pos.x, pos.y), b2Vec2(d.x*10, d.y*10), 0.2, 0.1, 5);
}

void UniteAerienne::SetColor(const sf::Color& col)
{
    m_corpsAvion->SetColor(col);
}

float UniteAerienne::GetShootAngle() const
{
    return Trigo::ToDeg(m_body->GetAngle());
}

float UniteAerienne::GetPuissanceExpulsion() const
{
    return 2;
}
BodyTypeEnum UniteAerienne::GetType() const
{
    return BodyTypeEnum::UniteAirE;
}
void UniteAerienne::Update()
{
    if(m_body->GetPosition().y>150)
        m_body->SetGravityScale(5);
    else if(b2Distance(m_body->GetLinearVelocity(), b2Vec2(0,0))<m_minVelocity)
        m_body->SetGravityScale(1);
    else
        m_body->SetGravityScale(0);
    b2Vec2 vec(0,0);
    if(m_forces.avant)
    {
        sf::Transform tr;
        tr.Rotate(Trigo::ToDeg(m_body->GetAngle()));
        sf::Vector2f v=tr.TransformPoint(m_acceleration, 0);
        vec.Set(v.x, v.y);
        m_body->ApplyForceToCenter(vec);
    }
    if(m_forces.arriere)
    {
        sf::Transform tr;
        tr.Rotate(Trigo::ToDeg(m_body->GetAngle()));
        sf::Vector2f v=tr.TransformPoint(-m_acceleration, 0);
        vec.Set(v.x, v.y);
        m_body->ApplyForceToCenter(vec);
    }
    if(m_forces.monte)
        m_body->ApplyAngularImpulse(m_rotationVelocity);
    else if(m_forces.descend)
        m_body->ApplyAngularImpulse(-m_rotationVelocity);
    Unite::Update();
}
bool UniteAerienne::SubirDegatsTerrain()
{
    if(b2Distance(m_body->GetLinearVelocity(), b2Vec2(0,0))>20)
        SubirDegats(100000);
    return EstVivant();
}

float UniteAerienne::GetExplosionRadius()
{
    return 10.f;
}
