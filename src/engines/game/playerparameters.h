#ifndef PLAYERPARAMETERS_H
#define PLAYERPARAMETERS_H

#include "../../core/singleton.h"
#include <SFML2/Graphics.hpp>
#include <tinyxml.h>
#include <unordered_map>

struct PlayerParametersData
{
    sf::Color couleur;
    std::string nom;
    std::unordered_map<std::string, sf::Keyboard::Key> touches;
};
class PlayerParameters: public Singleton<PlayerParameters>
{
    friend class Singleton<PlayerParameters>;
    public:
        const PlayerParametersData& GetParam(int) const;
        void SetPlayerColor(const sf::Color&, int = 0);

        void SetPlayerName(const std::string&, int = 0);
        const std::string& GetPlayerName(int = 0) const;

        void SetTouche(const std::string&, sf::Keyboard::Key, int=0);
        sf::Keyboard::Key GetTouche(const std::string&, int=0) const;
        std::string GetStringTouche(const std::string&, int=0) const;
        static bool IsToucheValide(sf::Keyboard::Key);

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
