#include "playerparameters.h"
#include "../../core/exception.h"
#include "../../core/logger.h"
#include "../../core/conversion.h"

template<>
PlayerParameters *Singleton<PlayerParameters>::m_singleton = 0;

PlayerParameters::PlayerParameters()
{
    Load();
}

PlayerParameters::~PlayerParameters()
{
    //dtor
}

const PlayerParametersData& PlayerParameters::GetParam(int player) const
{
    return m_param[player];
}

void PlayerParameters::SetPlayerColor(const sf::Color& color, int player)
{
    m_param[player].couleur = color;
    SaveParam(player);
}

void PlayerParameters::Load()
{
    TiXmlDocument fichier("data/player/param.xml");
    if(!fichier.LoadFile(TIXML_ENCODING_LEGACY))
        throw Exception("Impossible de charger le fichier: data/player/param.xml");

    TiXmlElement *rootNode = fichier.RootElement();
    TiXmlNode *player = 0;
    while((player=rootNode->IterateChildren("player",player)))
    {
        LoadPlayer(player);
    }
}
void PlayerParameters::LoadPlayer(TiXmlNode* node)
{
    int index;
    node->ToElement()->Attribute("numero", &index);
    int r,g,b;
    TiXmlElement *n;
    n = node->FirstChild("rouge")->ToElement();
    n->Attribute("value", &r);
    n = node->FirstChild("vert")->ToElement();
    n->Attribute("value", &g);
    n = node->FirstChild("bleu")->ToElement();
    n->Attribute("value", &b);
    m_param[index].couleur = sf::Color(r,g,b);
    m_param[index].nom=node->ToElement()->Attribute("name");

    TiXmlNode* nodeTouches, *nodeTouche = 0;
    nodeTouches=node->FirstChild("touches");
    std::string keyTouche;
    int keyCode;
    while((nodeTouche=nodeTouches->IterateChildren("touche", nodeTouche)))
    {
        keyTouche = nodeTouche->ToElement()->Attribute("name");
        nodeTouche->ToElement()->Attribute("code", &keyCode);
        m_param[index].touches[keyTouche] = sf::Keyboard::Key(keyCode);
    }
}
void PlayerParameters::SaveParam(int index)
{
    TiXmlDocument fichier("data/player/param.xml");
    if(!fichier.LoadFile(TIXML_ENCODING_LEGACY))
        throw Exception("Impossible de charger le fichier: data/player/param.xml");

    TiXmlElement *rootNode = fichier.RootElement();
    TiXmlNode *player = 0;
    while((player=rootNode->IterateChildren("player",player)))
    {
        int current;
        player->ToElement()->Attribute("numero", &current);
        if(current==index)
        {
            player->FirstChild("rouge")->ToElement()->SetAttribute("value", m_param[index].couleur.r);
            player->FirstChild("vert")->ToElement()->SetAttribute("value", m_param[index].couleur.g);
            player->FirstChild("bleu")->ToElement()->SetAttribute("value", m_param[index].couleur.b);
            player->FirstChild("touches")->Clear();
            player->ToElement()->SetAttribute("name", Sanitanyse(m_param[index].nom));
            for(auto it: m_param[index].touches)
            {
                TiXmlNode *toucheNode = new TiXmlElement("touche");
                toucheNode->ToElement()->SetAttribute("name", it.first);
                toucheNode->ToElement()->SetAttribute("code", it.second);
                player->FirstChild("touches")->ToElement()->LinkEndChild(toucheNode);
            }

            fichier.SaveFile("data/player/param.xml");
            return;
        }
    }
}

const std::string& PlayerParameters::GetPlayerName(int index) const
{
    return m_param[index].nom;
}
void PlayerParameters::SetPlayerName(const std::string& name, int index)
{
    m_param[index].nom=name;
    SaveParam(index);
}
std::string PlayerParameters::Sanitanyse(const std::string& str)
{
    std::string ret;
    ret.resize(str.size());
    for(size_t i=0,t = str.size();i<t;++i)
    {
        char c = str[i];
        if((c>='a'&&c<='z')||
           (c>='A'&&c<='Z')||
           (c>='0'&&c<='9')||
           c=='|'||
           c=='"'||
           c=='\\'||
           c=='&'||
           c=='('||
           c==')'||
           c=='['||
           c==']'||
           c=='-'||
           c=='#'||
           c=='?'||
           c==':'||
           c=='!'||
           c==','||
           c=='.'||
           c=='='||
           c=='<'||
           c=='>'||
           c=='{'||
           c=='}'||
           c=='/'||
           c=='_'||
           c=='@')
            ret[i]=c;
        else
            ret[i]='_';
    }
    return ret;
}
void PlayerParameters::SetTouche(const std::string& clef, sf::Keyboard::Key touche, int index)
{
    m_param[index].touches[clef]=touche;
    SaveParam(index);
}
sf::Keyboard::Key PlayerParameters::GetTouche(const std::string& clef, int index) const
{
    auto it = m_param[index].touches.find(clef);
    if(it!=m_param[index].touches.end())
    {
        return it->second;
    }
    return sf::Keyboard::Key::KeyCount;
}
std::string PlayerParameters::GetStringTouche(const std::string& touche, int index) const
{
    return key2string(GetTouche(touche, index));
}
bool PlayerParameters::IsToucheValide(sf::Keyboard::Key key)
{
    return key2string(key)!="";
}
