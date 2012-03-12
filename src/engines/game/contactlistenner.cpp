#include "projectile.h"

#include "contactlistenner.h"
#include "bodytype.h"
#include "unite.h"
#include "core/logger.h"
ContactListenner::ContactListenner(const std::unordered_map<sf::Uint32, Unite*> & unites): m_unites(unites)
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
    if(b1==0||b2==0) return;
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
        /*if(aEteTraite(proj))
            return;
        m_traites.push_back(proj);*/

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
    m_traites.clear();
}

void ContactListenner::m_CalculerImpulsions(Projectile* proj)
{
    static b2DistanceInput input;
    static b2DistanceOutput out;
    static b2SimplexCache cache;
    static b2Shape *shapeProj;

    shapeProj = proj->GetBody()->GetFixtureList()->GetShape();

    input.proxyA.Set(shapeProj, 0);
    input.useRadii=true;
    input.transformA=proj->GetBody()->GetTransform();
    b2Body* body;
    const float coefAjustement = 0.5;
    float coefDistance;
    for(auto &it : m_unites)
    {
        body=it.second->GetBody();
        input.proxyB.Set(body->GetFixtureList()->GetShape(), 0);
        input.transformB=body->GetTransform();
        cache.count=0;
        b2Distance(&out, &cache, &input);
        if(out.distance<proj->GetPuissance()*coefAjustement)
        {
            coefDistance = 1-(out.distance/(proj->GetPuissance()*coefAjustement));
            ExplosionImpusle tmp;
            tmp.object=body;
            tmp.impulse=out.pointB-out.pointA;
            if(out.distance!=0) // Pas de division par 0 !!!
            {
                tmp.impulse.x = tmp.impulse.x/out.distance;
                tmp.impulse.y = tmp.impulse.y/out.distance;
            }
            else
            {
                tmp.impulse.x = 0;
                tmp.impulse.y = 0;
            }
            tmp.impulse *= coefDistance;
            tmp.impulse*=20*proj->GetPuissance()*coefAjustement;
            tmp.pts=out.pointB;
            m_impulsions.push_back(tmp);
            if(out.distance<proj->GetPuissance()*0.1)
            {
                coefDistance = 1-(out.distance/(proj->GetPuissance()*0.1));
                it.second->SubirDegats(proj->GetPuissance()*coefDistance);
            }
        }
    }
}
const std::vector<ExplosionImpusle>& ContactListenner::GetImpulsions() const
{
    return m_impulsions;
}
bool ContactListenner::aEteTraite(Projectile* proj)
{
    for(Projectile* p: m_traites)
    {
        if(p==proj)
            return true;
    }
    return false;
}
