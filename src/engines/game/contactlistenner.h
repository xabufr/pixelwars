#ifndef CONTACTLISTENNER_H
#define CONTACTLISTENNER_H
#include <Box2D/Box2D.h>
#include <SFML2/System.hpp>

class Projectile;
struct ExplosionPosition
{
    sf::Vector2f position;
    float radius;
};
class ContactListenner : public b2ContactListener
{
    public:
        ContactListenner();
        virtual ~ContactListenner();
        void BeginContact (b2Contact *contact);
        void Clear();
        const std::vector<ExplosionPosition>& GetExplosions() const;
        const std::vector<Projectile*>& GetProjectileToDestroy() const;
    protected:
    private:
        std::vector<ExplosionPosition> m_explosions;
        std::vector<Projectile*> m_toDestroy;
};

#endif // CONTACTLISTENNER_H
