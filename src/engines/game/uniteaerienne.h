#ifndef UNITEAERIENNE_H
#define UNITEAERIENNE_H

#include "unite.h"

class UniteAerienne: public Unite
{
    public:
        UniteAerienne(b2World*, b2Vec2 pos, int);
        virtual ~UniteAerienne();

        virtual void Deplacer(const UnitInput&);
        virtual void Stop();
        virtual Projectile* Tirer();
        virtual void SetColor(const sf::Color& col);
        virtual float GetShootAngle() const;
        virtual float GetPuissanceExpulsion() const;
        virtual bool PeutTirer();

        virtual void Update();
        virtual BodyTypeEnum GetType() const;

        bool SubirDegatsTerrain();
        float GetExplosionRadius();
        virtual const std::string GetShootSound() const;
        virtual const std::string GetDeathSound() const;

    protected:
    private:
        void m_AddTrainee();
        SceneNodeSpriteItem *m_corpsAvion;
        struct {
            bool avant, arriere, monte, descend;
        } m_forces;
        float m_minVelocity, m_acceleration, m_rotationVelocity;
        int m_sens;
        float m_maxShootTime;
        bool m_surchauffe;
        sf::Clock m_timerSurfauffe;
        sf::Color m_color;
};

#endif // UNITEAERIENNE_H
