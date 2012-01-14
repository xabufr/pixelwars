#ifndef ENGINEEVENT_H
#define ENGINEEVENT_H
#include <SFML2/System.hpp>
#include <unordered_map>
#include <vector>
#include "engine.h"

class EngineEvent
{
    public:
        EngineEvent();

        virtual ~EngineEvent();
        /////////////////////////////////////////////////
        /// \brief Ajoute un moteur récépteur de l'évènement
        /// \param engine Moteur récépteur
        /// \return nothing
        ///
        /////////////////////////////////////////////////
        void To(Engine* engine);
        void AddPosition(sf::Uint8 index, const sf::Vector2f&);
        void AddInt(sf::Uint8 index, sf::Int32);
        void AddFloat(sf::Uint8 index, float);
        void AddString(sf::Uint8 index, std::string);

        const std::unordered_map<sf::Uint8, sf::Vector2f>& GetPositions() const;
        const std::unordered_map<sf::Uint8, sf::Int32>& GetInts() const;
        const std::unordered_map<sf::Uint8, float>& GetFloats() const;
        const std::unordered_map<sf::Uint8, char*>& GetStrings() const;

    protected:
    private:
        sf::Uint8 m_message;
        std::vector<Engine*> m_reciever;
        std::unordered_map<sf::Uint8, sf::Vector2f> m_position;
        std::unordered_map<sf::Uint8, sf::Int32> m_int;
        std::unordered_map<sf::Uint8, float> m_float;
        std::unordered_map<sf::Uint8, char*> m_string;
};

#endif // ENGINEEVENT_H
