#include "uniteaerienne.h"
#include "../../core/trigo.h"

UniteAerienne::UniteAerienne(b2World* world, b2Vec2 pos): Unite(world, pos, BodyTypeEnum::UniteAirE)
{
    b2PolygonShape sh;
    sh.SetAsBox(1, 0.25);
    b2FixtureDef fd;
    fd.density=1;
    fd.friction=10;
    fd.restitution=0;
    fd.shape = &sh;
    m_body->CreateFixture(&fd);
    m_corpsAvion = new SceneNodeShapeItem;
    m_corpsAvion->SetSize(20, 5);
    m_node->AddItem(m_corpsAvion);
    m_corpsAvion->SetRelativePosition(-10, -2.5);
    m_vie=10000;
    m_acceleration = 40;
    m_rotationVelocity = 0.25;
    m_minVelocity = 20;
    m_body->SetAngularDamping(10);
    m_body->SetLinearDamping(0.5);
    m_tempRechargement = 10;
}

UniteAerienne::~UniteAerienne()
{
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

bool UniteAerienne::PeutTirer()
{
    if(m_timerFire.GetElapsedTime().AsMicroseconds()<m_tempRechargement)
        return false;
    return m_fire;
}

Projectile* UniteAerienne::Tirer()
{
}

void UniteAerienne::SetColor(const sf::Color& col)
{
    m_corpsAvion->SetColor(col);
}

float UniteAerienne::GetShootAngle() const
{
}

float UniteAerienne::GetPuissanceExpulsion() const
{
}
void UniteAerienne::Update()
{
    if(b2Distance(m_body->GetLinearVelocity(), b2Vec2(0,0))<m_minVelocity)
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
