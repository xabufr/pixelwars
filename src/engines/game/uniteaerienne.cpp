#include "uniteaerienne.h"
#include "../../core/trigo.h"
#include "projectile.h"

UniteAerienne::UniteAerienne(b2World* world, b2Vec2 pos, int joueur): Unite(world, pos, BodyTypeEnum::UniteAirE)
{
    b2PolygonShape sh;
    sh.SetAsBox(2, 0.5);
    b2FixtureDef fd;
    fd.density=0.5;
    fd.friction=1;
    fd.restitution=0;
    fd.shape = &sh;
    m_body->CreateFixture(&fd);
    m_corpsAvion = new SceneNodeSpriteItem;
    m_corpsAvion->SetImage("data/avion.png");
    m_node->AddItem(m_corpsAvion);
    m_corpsAvion->SetRelativePosition(-20, -5);
    if(joueur==1)
        m_corpsAvion->SetRelativeRotation(180);
    m_vie=1;
    m_acceleration = 60;
    m_rotationVelocity = 1.5;
    m_minVelocity = 20;
    m_body->SetAngularDamping(10);
    m_body->SetLinearDamping(0.25);
    m_tempRechargement = 33;
    m_sens = joueur==0?1:-1;
    m_maxShootTime=0.5f;
    m_surchauffe = false;
}

UniteAerienne::~UniteAerienne()
{
    ParticleParameters params;
    params.colorFromList=true;
    params.colorList.push_back(sf::Color(128,128,128));
    params.colorList.push_back(sf::Color(255,149,0));
    params.colorList.push_back(m_color);
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

bool UniteAerienne::PeutTirer()
{
    return Unite::PeutTirer()&&(!m_surchauffe);
}
void UniteAerienne::Deplacer(const UnitInput& in)
{
    m_forces.descend=in.bas;
    m_forces.monte=in.haut;
    m_forces.avant=in.droite;
    m_forces.arriere=in.gauche;
    if((in.tirer&&!m_fire&&!m_surchauffe)||(!in.tirer&&m_fire&&!m_surchauffe))
        m_timerSurfauffe.Restart();
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
    sf::Transform t;
    t.Rotate(Trigo::ToDeg(m_body->GetAngle()));
    sf::Vector2f d = t.TransformPoint(2.3*m_sens, 0);
    sf::Vector2f pos = sf::Vector2f(m_body->GetPosition().x, m_body->GetPosition().y)+d;
    m_body->ApplyLinearImpulse(b2Vec2(-d.x, -d.y), m_body->GetPosition());
    return new Projectile(m_body->GetWorld(), b2Vec2(pos.x, pos.y), b2Vec2(d.x*10, d.y*10), 0.2, 0.1, 5);
}

void UniteAerienne::SetColor(const sf::Color& col)
{
    m_corpsAvion->SetColor(col);
    m_color = col;
}

float UniteAerienne::GetShootAngle() const
{
    return Trigo::ToDeg(m_body->GetAngle())+(m_sens==-1?180:0);
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
    if(m_timerSurfauffe.GetElapsedTime().AsSeconds()>=m_maxShootTime&&!m_surchauffe&&m_fire)
    {
        m_surchauffe=true;
        m_timerSurfauffe.Restart();
    }
    else if(m_surchauffe && m_timerSurfauffe.GetElapsedTime().AsSeconds()>=m_maxShootTime*3)
    {
        m_surchauffe=false;
        m_timerSurfauffe.Restart();
    }
    if(m_body->GetPosition().y>150)
        m_body->SetGravityScale(5);
    else if(b2Distance(m_body->GetLinearVelocity(), b2Vec2(0,0))<m_minVelocity||(!m_forces.avant&&!m_forces.arriere))
        m_body->SetGravityScale(1);
    else
        m_body->SetGravityScale(0);
    b2Vec2 vec(0,0);
    if(m_forces.avant)
    {
        sf::Transform tr;
        tr.Rotate(Trigo::ToDeg(m_body->GetAngle()));
        sf::Vector2f v;
        if(m_sens==1)
        {
            v=tr.TransformPoint(m_acceleration, 0);
            m_AddTrainee();
        }
        else
            v=tr.TransformPoint(m_acceleration*0.05, 0);

        vec.Set(v.x, v.y);
        m_body->ApplyForceToCenter(vec);
    }
    if(m_forces.arriere)
    {
        sf::Transform tr;
        tr.Rotate(Trigo::ToDeg(m_body->GetAngle()));
        sf::Vector2f v;
        if(m_sens==-1)
        {
            v=tr.TransformPoint(-m_acceleration, 0);
            m_AddTrainee();
        }
        else
            v=tr.TransformPoint(-m_acceleration*0.05, 0);
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
    return 30.f;
}
void UniteAerienne::m_AddTrainee()
{
    ParticleParameters p;
    p.useImage = true;
    p.image="data/trainee.png";
    sf::Transform t;
    t.Rotate(Trigo::ToDeg(-m_body->GetAngle()));
    p.position=sf::Vector2f(10*m_body->GetPosition().x, -10*m_body->GetPosition().y)+t.TransformPoint(-1.f*m_sens*m_corpsAvion->GetSize().x/2.f, 0);
    p.timeToLive=2000;
    p.gravity=false;
    GraphicalEngine::GetInstance()->GetSceneManager()->GetParticleManager()->AddParticle(p);
}
const std::string UniteAerienne::GetShootSound() const
{
    return "data/avionExplose.wav";
}
const std::string UniteAerienne::GetDeathSound() const
{
    return "salut.wav";
}
