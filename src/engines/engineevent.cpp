#include <cstring>
#include "../game.h"
#include "engineevent.h"

Game* EngineEvent::m_game = 0;

EngineEvent::EngineEvent()
{
    //ctor
}

EngineEvent::~EngineEvent()
{

}
void EngineEvent::To(Engine* engine)
{
    m_reciever.push_back(engine);
    CreateId();
}

void EngineEvent::To(EngineType type)
{
    m_recieverId.push_back(type);
    CreateEngines();
}

void EngineEvent::AddPosition(sf::Uint8 index, const sf::Vector2f& pos)
{
    m_position[index]=pos;
}

void EngineEvent::AddInt(sf::Uint8 index, sf::Int32 i)
{
    m_int[index]=i;
}
void EngineEvent::AddFloat(sf::Uint8 index, float f)
{
    m_float[index]=f;
}

void EngineEvent::AddString(sf::Uint8 index, const std::string &s)
{
    m_string[index]=s;
}


const std::unordered_map<sf::Uint8, sf::Vector2f>& EngineEvent::GetPositions() const
{
    return m_position;
}
const std::unordered_map<sf::Uint8, sf::Int32>& EngineEvent::GetInts() const
{
    return m_int;
}
const std::unordered_map<sf::Uint8, float>& EngineEvent::GetFloats() const
{
    return m_float;
}
const std::unordered_map<sf::Uint8, std::string>& EngineEvent::GetStrings() const
{
    return m_string;
}
const std::vector<Engine*>& EngineEvent::GetEngines() const
{
    return m_reciever;
}
void EngineEvent::SetGame(Game* g)
{
    m_game=g;
}
void EngineEvent::SetMessage(sf::Uint8 message)
{
    m_message=message;
}
sf::Uint8 EngineEvent::GetMessage() const
{
    return m_message;
}
void EngineEvent::CreateId()
{
    m_recieverId.clear();
    for(Engine *e : m_reciever)
    {
        m_recieverId.push_back(e->GetEngineId());
    }
}
void EngineEvent::CreateEngines()
{
    m_reciever.clear();
    for(EngineType id : m_recieverId)
    {
        m_reciever.push_back(m_game->GetEngineFromId(id));
    }
}
sf::Packet EngineEvent::ToPacket() const
{
    sf::Packet pack;
    sf::Uint32 taille = m_recieverId.size();
    pack << m_message << taille;
    for(sf::Uint8 id : m_recieverId)
        pack << id;
    taille = m_position.size();
    pack<<taille;
    for(std::pair<sf::Uint8, sf::Vector2f> pos: m_position)
    {
        pack<<pos.first<<pos.second.x<<pos.second.y;
    }
    taille = m_int.size();
    pack<<taille;
    for(std::pair<sf::Uint8, sf::Int32> i: m_int)
    {
        pack<<i.first<<i.second;
    }
    taille = m_float.size();
    pack<<taille;
    for(std::pair<sf::Uint8, float> f: m_float)
    {
        pack<<f.first<<f.second;
    }
    taille = m_string.size();
    pack<<taille;
    for(std::pair<sf::Uint8, std::string> s: m_string)
    {
        pack<<s.first<<s.second;
    }
    return pack;
}
void EngineEvent::FromPacket(sf::Packet &packet)
{
    sf::Uint32 taille,i;
    sf::Int32 int32_tmp;
    sf::Uint8 int8_tmp;
    std::string str;
    float f1,f2;

    m_reciever.clear();
    m_recieverId.clear();
    m_float.clear();
    m_int.clear();
    m_position.clear();
    m_string.clear();

    packet >> m_message >> taille;

    for(i=0;i<taille;++i)
    {
        packet>>int8_tmp;
        m_recieverId.push_back((EngineType)int8_tmp);
    }
    packet>>taille;
    for(i=0;i<taille;++i)
    {
        packet>>int8_tmp>>f1>>f2;
        m_position[int8_tmp]=sf::Vector2f(f1,f2);
    }
    packet>>taille;
    for(i=0;i<taille;++i)
    {
        packet>>int8_tmp>>int32_tmp;
        m_int[int8_tmp]=int32_tmp;
    }
    packet>>taille;
    for(i=0;i<taille;++i)
    {
        packet>>int8_tmp>>f1;
        m_float[int8_tmp]=f1;
    }
    packet>>taille;
    for(i=0;i<taille;++i)
    {
        packet>>int8_tmp>>str;
        m_string[int8_tmp]=str;
    }
    CreateEngines();
}
