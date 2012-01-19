#include "core/exception.h"
#include "uniteterrestremodel.h"


template<>
UniteTerrestreModel *Singleton<UniteTerrestreModel>::m_singleton = 0;

UniteTerrestreModel::UniteTerrestreModel()
{
    m_SetDefaultValues();
    LoadAll();
}
void UniteTerrestreModel::m_SetDefaultValues()
{
    /*Valeurs par défaut, pour toutes les unités*/
    m_densiteProjectile = 10.f;
    m_densiteRoue = 10.f;
    m_frictionRoue = 3000.f;
    m_puissanceRoues = 700.f;
    m_tailleRoues = 1.f;


    m_tailleCanonMax = 4.f;
    m_tailleCanonMin = 1.f;
    m_puissanceMin = 5.f;
    m_puissanceMax = 25.f;
    m_poidMin = 2.5f;
    m_poidMax = 20.f;

    m_tempReloadMax=1500;
    m_tempReloadMin=50;

    m_tailleUnite.Set(2.5, 1);
}
UniteTerrestreModel::~UniteTerrestreModel()
{
    for(auto &i : m_models)
    {
        delete i.second;
    }
}

UniteTerrestreParameters* const UniteTerrestreModel::Get(const std::string& nom)
{
    auto it = m_models.find(nom);
    if(it==m_models.end())
        return 0;
    return it->second;
}

bool UniteTerrestreModel::Exist(const std::string& uid)
{
    return m_models.find(uid)!=m_models.end();
}

void UniteTerrestreModel::LoadAll()
{
    TiXmlDocument fichier("data/units/land.xml");
    if(!fichier.LoadFile())
        throw Exception("Impossible de charger le fichier: data/units/land.xml");

    TiXmlElement *rootNode = fichier.RootElement();
    TiXmlNode *model = 0;
    while((model=rootNode->IterateChildren("model",model)))
    {
        LoadModel(model);
    }
}
void UniteTerrestreModel::LoadModel(TiXmlNode *model)
{

    bool ok=true;
    double f;
    std::string uid = model->ToElement()->Attribute("uid");
    if(uid==""||m_models.find(uid)!=m_models.end())
        return;

    UniteTerrestreParameters *param = new UniteTerrestreParameters;
    param->nom = model->ToElement()->Attribute("name");
    param->uid = uid;
    TiXmlNode *n;
    n=model->FirstChild("blindage");
    if(!n)
        ok=false;
    else
    {
        n->ToElement()->Attribute("value", &f);
        param->poidsCorp = f;
    }

    n=model->FirstChild("canon");
    if(!n)
        ok=false;
    else
    {
        n->ToElement()->Attribute("value", &f);
        param->tailleCanon = f;
    }

    n=model->FirstChild("projectile");
    if(!n)
        ok=false;
    else
    {
        n->ToElement()->Attribute("value", &f);
        param->puissanceProjectiles = f;
    }
    if(!ok||!m_IsValid(param))
    {
        delete param;
        return;
    }
    m_models[uid] = param;
}
bool UniteTerrestreModel::m_IsValid(UniteTerrestreParameters* param) const
{
    if(param->poidsCorp<m_poidMin||param->poidsCorp>m_poidMax)
        return false;
    if(param->tailleCanon<m_tailleCanonMin||param->tailleCanon>m_tailleCanonMax)
        return false;
    if(param->puissanceProjectiles<m_puissanceMin||param->puissanceProjectiles>m_puissanceMax)
        return false;
    return true;
}
const b2Vec2& UniteTerrestreModel::GetTailleUnit() const
{
    return m_tailleUnite;
}

float UniteTerrestreModel::GetDensiteRoue() const
{
    return m_densiteRoue;
}

float UniteTerrestreModel::GetFrictionRoue() const
{
    return m_frictionRoue;
}

float UniteTerrestreModel::GetPuissanceRoue() const
{
    return m_puissanceRoues;
}

float UniteTerrestreModel::GetTailleRoue() const
{
    return m_tailleRoues;
}
float UniteTerrestreModel::GetDensiteProjectile() const
{
    return m_densiteProjectile;
}
unsigned int UniteTerrestreModel::GetReloadTimeMin() const
{
    return m_tempReloadMin;
}
unsigned int UniteTerrestreModel::GetReloadTimeMax() const
{
    return m_tempReloadMax;
}
float UniteTerrestreModel::GetPuissanceMax() const
{
    return m_puissanceMax;
}
float UniteTerrestreModel::GetPuissanceMin() const
{
    return m_puissanceMin;
}
float UniteTerrestreModel::GetCanonMax() const
{
    return m_tailleCanonMax;
}
float UniteTerrestreModel::GetCanonMin() const
{
    return m_tailleCanonMin;
}
float UniteTerrestreModel::GetTailleXTotale() const
{
    return m_tailleRoues*2 + m_tailleUnite.x;
}
