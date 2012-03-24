#ifndef UNITEAERIENNE_H
#define UNITEAERIENNE_H

#include "unite.h"

class UniteAerienne: public Unite
{
    public:
        UniteAerienne(b2World*, b2Vec2 pos);
        virtual ~UniteAerienne();

        virtual void Deplacer(const UnitInput&);
        virtual void Stop();
        virtual Projectile* Tirer();
        virtual void SetColor(const sf::Color& col);
        virtual float GetShootAngle() const;
        virtual float GetPuissanceExpulsion() const;

        virtual void Update();
        virtual BodyTypeEnum GetType() const;

        bool SubirDegatsTerrain();
        float GetExplosionRadius();

    protected:
    private:
        SceneNodeShapeItem *m_corpsAvion;
        struct {
            bool avant, arriere, monte, descend;
        } m_forces;
        float m_minVelocity, m_acceleration, m_rotationVelocity;
};

#endif // UNITEAERIENNE_H
