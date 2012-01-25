#ifndef ENGINEEVENT_H
#define ENGINEEVENT_H
#include <SFML2/System.hpp>
#include <unordered_map>
#include <vector>
#include <SFML2/Network.hpp>

namespace TypeMessage
{
    sf::Uint8 const Quitter = 0;
}

class Game;
class Engine;
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
        void AddString(sf::Uint8 index, const std::string&);

        const std::unordered_map<sf::Uint8, sf::Vector2f>& GetPositions() const;
        const std::unordered_map<sf::Uint8, sf::Int32>& GetInts() const;
        const std::unordered_map<sf::Uint8, float>& GetFloats() const;
        const std::unordered_map<sf::Uint8, std::string>& GetStrings() const;

        const std::vector<Engine*>& GetEngines() const;
        const std::vector<sf::Uint8> GetEnginesById() const;

        sf::Uint8 GetMessage() const;
        void SetMessage(sf::Uint8);

        static void SetGame(Game*);

        sf::Packet ToPacket() const;
        void FromPacket(sf::Packet &pack);

    protected:

    private:
        sf::Uint8 m_message;
        std::vector<Engine*> m_reciever;
        std::vector<EngineType> m_recieverId;
        std::unordered_map<sf::Uint8, sf::Vector2f> m_position;
        std::unordered_map<sf::Uint8, sf::Int32> m_int;
        std::unordered_map<sf::Uint8, float> m_float;
        std::unordered_map<sf::Uint8, std::string> m_string;

        EngineEvent(const EngineEvent&){};

        static Game* m_game;

        void CreateId();
        void CreateEngines();
};

#endif // ENGINEEVENT_H
