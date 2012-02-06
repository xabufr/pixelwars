#ifndef PLAYERPARAMETERS_H
#define PLAYERPARAMETERS_H

#include "../../core/singleton.h"
#include <SFML2/Graphics.hpp>
#include <tinyxml.h>

struct PlayerParametersData
{
    sf::Color couleur;
    std::string nom;
};
class PlayerParameters: public Singleton<PlayerParameters>
{
    friend class Singleton<PlayerParameters>;
    public:
        const PlayerParametersData& GetParam(int) const;
        void SetPlayerColor(const sf::Color&, int = 0);

        void SetPlayerName(const std::string&, int = 0);
        const std::string& GetPlayerName(int = 0) const;

    protected:
        PlayerParameters();
        virtual ~PlayerParameters();

    private:
        PlayerParametersData m_param[2];

        void Load();
        void LoadPlayer(TiXmlNode*);
        void SaveParam(int);
        std::string Sanitanyse(const std::string&);
};

#endif // PLAYERPARAMETERS_H
