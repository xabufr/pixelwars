#include "playerparameters.h"
#include "../../core/exception.h"
#include "../../core/logger.h"

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
            player->ToElement()->SetAttribute("name", Sanitanyse(m_param[index].nom));
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
