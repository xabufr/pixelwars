#ifndef SOUNDPROPRITIES_H
#define SOUNDPROPRITIES_H

#include <vector>
#include <string>
#include "../../core/singleton.h"

struct Intervalle
{
    float min, max;
    std::string chemin;
};

class TiXmlNode;
class Projectile;
class UniteTerrestre;
class SoundProprities: public Singleton<SoundProprities>
{
    friend class Singleton<SoundProprities>;
    public:
        std::string GetProjectileSound(Projectile*) const;
        std::string GetUniteTerrestreSound(UniteTerrestre const * const)const;
        std::string GetUniteTerrestreDestructionSound(const UniteTerrestre *) const;
    protected:
        SoundProprities();
        virtual ~SoundProprities();
    private:
        std::vector<Intervalle> m_projectilesSounds, m_unitesTerrestreSounds, m_unitesTerrestreDeathSounds;

        void AddIntervalle(TiXmlNode*, std::vector<Intervalle>&);
};

#endif // PROJECTILESOUNDPROPRITIES_H
