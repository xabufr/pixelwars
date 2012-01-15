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
            proj=(Projectile*)b1->proprietaire;
        else
            proj=(Projectile*)b2->proprietaire;
        if(b1->type==BodyTypeEnum::UniteE||b2->type==BodyTypeEnum::UniteE)
        {
            Unite* unite;
            if(b1->type==BodyTypeEnum::UniteE)
                unite = (Unite*)b1->proprietaire;
            else
                unite = (Unite*)b2->proprietaire;
            unite->SubirDegats(1000);
        }

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
}
