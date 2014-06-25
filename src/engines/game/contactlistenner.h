#ifndef CONTACTLISTENNER_H
#define CONTACTLISTENNER_H
#include <Box2D/Box2D.h>
#include <SFML/System.hpp>
#include <deque>

class Unite;
class Projectile;
struct ExplosionPosition
{
    sf::Vector2f position;
    float radius, angle;
};
struct ExplosionImpusle
{
    b2Vec2 impulse, pts;
    b2Body *object;
};
class ContactListenner : public b2ContactListener
{
    public:
        ContactListenner(const std::unordered_map<sf::Uint32, Unite*>&);
        virtual ~ContactListenner();
        void PreSolve(b2Contact *contact, const b2Manifold* oldManif);
        void Clear();
        const std::vector<ExplosionPosition>& GetExplosions() const;
        const std::vector<Projectile*>& GetProjectileToDestroy() const;
        const std::vector<ExplosionImpusle>& GetImpulsions() const;
    protected:
    private:
        std::vector<ExplosionPosition> m_explosions;
        std::vector<Projectile*> m_toDestroy;
        std::vector<ExplosionImpusle> m_impulsions;
        const std::unordered_map<sf::Uint32, Unite*> & m_unites;
        std::vector <Projectile*> m_traites;

        void m_CalculerImpulsions(Projectile*);
        bool aEteTraite(Projectile*);
};

#endif // CONTACTLISTENNER_H
