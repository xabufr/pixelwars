#ifndef UNITETERRESTRE_H
#define UNITETERRESTRE_H
#include "unite.h"
#include "uniteterrestremodel.h"

class GuiProgressBarItem;
class UniteTerrestre : public Unite
{
    public:
        UniteTerrestre(b2World*, b2Vec2 pos=b2Vec2(0,0), const std::string& uid="default");
        virtual ~UniteTerrestre();
        virtual void Update();
        void Deplacer(const UnitInput& input);
        void GetFireParameters(b2Vec2 &pos, b2Vec2 &direction, float &taille, float &poids, float &puissance);
        void Stop();
        Projectile* Tirer();
        virtual void SetColor(const sf::Color& col);
        float GetTailleCanon() const;
        virtual float GetShootAngle() const;
        virtual float GetPuissanceExpulsion() const;
        virtual const std::string GetShootSound() const;
        virtual const std::string GetDeathSound() const;
        const UniteTerrestreParameters* GetParams() const;

    private:
        b2Body *m_roue1, *m_roue2, *m_tourelle;
        b2WheelJoint *jointure1, *jointure2;
        b2RevoluteJoint *m_jointureTourelle;
        SceneNodeShapeItem *m_itemBody, *m_itemTourelle;
        SceneNodeSpriteItem *m_itemRoue1, *m_itemRoue2;
        SceneNode *m_nodeRoue1, *m_nodeRoue2, *m_nodeTourelle;
        GuiProgressBarItem *m_barreVie;
        b2Vec2 m_tailleCanon;
        const UniteTerrestreParameters* const m_param;
};

#endif // UNITETERRESTRE_H
