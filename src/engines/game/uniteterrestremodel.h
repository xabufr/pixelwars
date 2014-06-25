#ifndef UNITETERRESTREMODEL_H
#define UNITETERRESTREMODEL_H
#include <unordered_map>
#include <Box2D/Box2D.h>
#include "../../core/singleton.h"
#include <tinyxml.h>

struct UniteTerrestreParameters
{
    float tailleCanon;
    float puissanceProjectiles;
    float poidsCorp;

    std::string cheminRoues, nom, uid;
};
class UniteTerrestreModel: public Singleton<UniteTerrestreModel>
{
    friend class Singleton<UniteTerrestreModel>;
    public:
        UniteTerrestreParameters* const Get(const std::string&);
        bool Exist(const std::string&);
        const b2Vec2& GetTailleUnit() const;
        float GetDensiteRoue() const;
        float GetFrictionRoue() const;
        float GetPuissanceRoue() const;
        float GetTailleRoue() const;
        float GetDensiteProjectile() const;

        unsigned int GetReloadTimeMin() const;
        unsigned int GetReloadTimeMax() const;
        float GetPuissanceMax() const;
        float GetPuissanceMin() const;
        float GetCanonMax() const;
        float GetCanonMin() const;

        float GetTailleXTotale() const;


    protected:
        UniteTerrestreModel();
        virtual ~UniteTerrestreModel();
    private:
        void m_SetDefaultValues();
        void LoadAll();
        void LoadModel(TiXmlNode*);
        bool m_IsValid(UniteTerrestreParameters*) const;

        std::unordered_map<std::string, UniteTerrestreParameters*> m_models;
        b2Vec2 m_tailleUnite;
        float m_densiteProjectile, m_densiteRoue, m_frictionRoue, m_puissanceRoues, m_tailleRoues;
        unsigned int m_tempReloadMin, m_tempReloadMax;

        float m_tailleCanonMax, m_tailleCanonMin, m_puissanceMin, m_puissanceMax, m_poidMin, m_poidMax;
};

#endif // UNITETERRESTREMODEL_H
