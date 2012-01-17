#include "projectile.h"

#include "contactlistenner.h"
#include "bodytype.h"
#include "unite.h"
#include "core/logger.h"

ContactListenner::ContactListenner()
{
    //ctor
}

ContactListenner::~ContactListenner()
{
    //dtor
}
const std::vector<ExplosionPosition>& ContactListenner::GetExplosions() const
{
    return m_explosions;
}

const std::vector<Projectile*>& ContactListenner::GetProjectileToDestroy() const
{
    return m_toDestroy;
}

void ContactListenner::BeginContact(b2Contact *contact)
{
    BodyType *b1 = (BodyType*) contact->GetFixtureA()->GetBody()->GetUserData();
    BodyType *b2 = (BodyType*) contact->GetFixtureB()->GetBody()->GetUserData();

    if(b1->type==BodyTypeEnum::ProjectileE||b2->type==BodyTypeEnum::ProjectileE)
    {
        Projectile *proj;
        if(b1->type==BodyTypeEnum::ProjectileE)
        {
            proj=(Projectile*)b1->proprietaire;
        }

        else
        {
            proj=(Projectile*)b2->proprietaire;
        }

        if(b1->type==BodyTypeEnum::UniteE||b2->type==BodyTypeEnum::UniteE)
        {
            Unite* unite;

            if(b1->type==BodyTypeEnum::UniteE)
            {
                unite = (Unite*)b1->proprietaire;
            }

            else
            {
                unite = (Unite*)b2->proprietaire;
            }

            unite->SubirDegats(proj->GetPuissance());
        }
        m_CalculerImpulsions(proj);

        b2WorldManifold manif;
        contact->GetWorldManifold(&manif);
        ExplosionPosition exp;
        exp.position.x = manif.points[0].x*10;
        exp.position.y = -manif.points[0].y*10;
        exp.radius = proj->GetPuissance();
        m_explosions.push_back(exp);
        m_toDestroy.push_back(proj);
    }

}

void ContactListenner::Clear()
{
    m_explosions.clear();
    m_toDestroy.clear();
    m_impulsions.clear();
}

void ContactListenner::m_CalculerImpulsions(Projectile* proj)
{
    static b2DistanceInput input;
    static b2DistanceOutput out;
    static b2SimplexCache cache;
    static b2Shape *shapeProj;
    static b2World* world;
    world = proj->GetBody()->GetWorld();
    shapeProj = proj->GetBody()->GetFixtureList()->GetShape();

    input.proxyA.Set(shapeProj, 0);
    input.useRadii=false;
    input.transformA=proj->GetBody()->GetTransform();
    for(b2Body* body = world->GetBodyList(); body!=0; body=body->GetNext())
    {
        if(body==proj->GetBody())
            continue;
        cache.count=0;
        input.proxyB.Set(body->GetFixtureList()->GetShape(), 0);
        input.transformB=body->GetTransform();
        b2Distance(&out, &cache, &input);
        if(out.distance<proj->GetPuissance()*0.5)
        {
            ExplosionImpusle tmp;
            tmp.object=body;
            tmp.impulse=out.pointB-out.pointA;
            tmp.impulse.x = 1/tmp.impulse.x;
            tmp.impulse.y = 1/tmp.impulse.y;
            tmp.impulse*=10;
            tmp.pts=out.pointB;
            m_impulsions.push_back(tmp);
        }
    }
}

const std::vector<ExplosionImpusle>& ContactListenner::GetImpulsions() const
{
    return m_impulsions;
}
