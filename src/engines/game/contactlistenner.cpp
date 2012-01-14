#include "contactlistenner.h"

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
void ContactListenner::BeginContact(b2Contact *contact)
{
    b2WorldManifold manif;
    contact->GetWorldManifold(&manif);
    ExplosionPosition exp;
    exp.position.x = manif.points[0].x*10;
    exp.position.y = -manif.points[0].y*10;
    exp.radius = 10.f;
    m_explosions.push_back(exp);
}
void ContactListenner::Clear()
{
    m_explosions.clear();
}
