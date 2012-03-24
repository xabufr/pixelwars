#ifndef UNITE_H
#define UNITE_H
#include <Box2D/Box2D.h>
#include "engines/graphics/graphicalengine.h"
#include "inputmanager.h"
class Projectile;
class Unite
{
    public:
        Unite(b2World*, b2Vec2 pos, BodyTypeEnum t = BodyTypeEnum::UniteE);
        virtual ~Unite();
        void SetPosition();
        virtual void Update();
        virtual void Deplacer(const UnitInput&) = 0;
        virtual void Stop()=0;
        virtual bool PeutTirer();
        virtual Projectile* Tirer()=0;
        virtual bool EstVivant()const;
        virtual void SetColor(const sf::Color& col) = 0;
        virtual float GetShootAngle() const = 0;
        virtual float GetPuissanceExpulsion() const = 0;

        virtual void SubirDegats(float degat);

        b2Body* GetBody()const;

    protected:
        b2Body *m_body;
        SceneNode *m_node;
        sf::Clock m_timerFire;
        float m_tempRechargement;

        float m_vie;
        bool m_fire;
};

#endif // UNITE_H
