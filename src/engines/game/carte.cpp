#include "carte.h"
#include <libnoise/noise.h>
#include "bodytype.h"
#include <SFML2/Graphics.hpp>

Carte::Carte(b2World* world, const sf::Vector2i& taille, float valMoy, float diff, int seed)
{
    m_world=world;
    m_nodeTerrain = GraphicalEngine::GetInstance()->GetSceneManager()->GetRootNode()->AddSceneNode();
    m_nodeCiel = GraphicalEngine::GetInstance()->GetSceneManager()->GetRootNode()->AddSceneNode();
    m_nodeCiel->SetLevel(-10);
    m_nodeCiel->SetAbsolutePosition(taille.x/2, taille.y/2);
    m_itemTerrain = new SceneNodeTextureItem;
    m_itemDroite = new SceneNodeShapeItem;
    m_itemGauche = new SceneNodeShapeItem;
    m_nodeTerrain->AddItem(m_itemTerrain);
    m_nodeTerrain->AddItem(m_itemDroite);
    m_nodeTerrain->AddItem(m_itemGauche);
    m_itemDessousTerrain = new SceneNodeShapeItem;
    m_nodeTerrain->AddItem(m_itemDessousTerrain);

    m_fondCiel = new SceneNodeCircleShapeItem;
    m_fondCielTransition = new SceneNodeCircleShapeItem;
    m_fondCiel->SetRadius(taille.x*2);
    m_fondCielTransition->SetRadius(taille.x*3);
    m_nodeCiel->AddItem(m_fondCiel);
    m_nodeCiel->AddItem(m_fondCielTransition);
    m_fondCiel->SetRelativePosition(-taille.x*2,-taille.x*2);
    m_fondCielTransition->SetRelativePosition(-taille.x*3,-taille.x*3);

    m_itemSoleil = new SceneNodeSpriteItem;
    m_itemLune = new SceneNodeSpriteItem;
    m_itemSoleil->SetImage("data/sun.png");
    m_itemLune->SetImage("data/lune.png");

    m_nodeCiel->AddItem(m_itemSoleil);
    m_nodeCiel->AddItem(m_itemLune);
    m_itemSoleil->SetRelativePosition((-m_itemSoleil->GetSize().x/2)+(taille.x/2),-m_itemSoleil->GetSize().y/2);
    m_itemLune->SetRelativePosition((m_itemLune->GetSize().x/2)-(taille.x*3/4),-m_itemLune->GetSize().y/2);



    m_destruction = false;
    m_modified = false;
    m_tailleZone = 100;
    Generer(taille, seed, valMoy, diff);
    m_dayDuration = 5*60*1000; //Cycles de 5 minutes
    m_time.Restart();
}

Carte::~Carte()
{
    delete (BodyType*) m_bodyTerrain->GetUserData();
    m_world->DestroyBody(m_bodyTerrain);
    GraphicalEngine::GetInstance()->GetSceneManager()->RemoveNode(m_nodeTerrain);
    GraphicalEngine::GetInstance()->GetSceneManager()->RemoveNode(m_nodeCiel);
}
void Carte::DemarrerDestruction()
{
    m_destruction = true;
    m_modified = false;
}
void Carte::FinirDestruction()
{
    m_destruction = false;
    if(m_modified)
        RegenererZonesModifiees();
}
void Carte::Generer(const sf::Vector2i& taille, int seed, float valMoy, float diff)
{
    m_taille=taille;
    m_nb_zones = (taille.x/m_tailleZone)+(taille.x%m_tailleZone!=0?1:0);
    m_fixturesMapZone.resize(m_nb_zones);

    m_itemTerrain->CreateTexture(taille);
    m_itemDessousTerrain->SetSize(taille.x, 100000);
    m_itemDessousTerrain->SetColor(sf::Color(128,128,128));
    m_itemDessousTerrain->SetRelativePosition(0, taille.y);

    m_itemDroite->SetSize(100000, taille.y+100000);
    m_itemGauche->SetSize(100000, taille.y+100000);
    m_itemDroite->SetColor(sf::Color(0,0,0));
    m_itemGauche->SetColor(sf::Color(0,0,0));

    m_itemGauche->SetRelativePosition(-100000, -50000);
    m_itemDroite->SetRelativePosition(taille.x, -50000);

    noise::module::Perlin gen;
    gen.SetSeed(seed);
    sf::Uint8 filler[taille.y*4];
    for(int y,x=0;x<taille.x;++x)
    {
        y = valMoy+(diff*gen.GetValue(float(x)*0.0015, 0.5,0.5));
        Fill(filler, y);
        m_itemTerrain->UpdateTexture(filler, 1, taille.y, x, 0);
    }
    b2BodyDef bd;
    bd.userData = (void*) new BodyType(BodyTypeEnum::TerrainE, (void*)this);
    m_bodyTerrain=m_world->CreateBody(&bd);
    RecalculerTerrain();
}

void Carte::AjouterExplosion(sf::Vector2f position, float radius)
{
    m_itemTerrain->DrawCircle(radius, sf::Color(0,0,0,0), position);
    AjouterZoneListeRegen(DeterminerZone(position.x+radius));
    AjouterZoneListeRegen(DeterminerZone(position.x-radius));
    AjouterZoneListeRegen(DeterminerZone(position.x));

    if(!m_destruction)
    {
        RegenererZonesModifiees();
    }
    else
        m_modified = true;
}
void Carte::RecreerTerrain()
{
    delete (BodyType*) m_bodyTerrain->GetUserData();
    m_world->DestroyBody(m_bodyTerrain);
    b2BodyDef bd;
    bd.userData = (void*) new BodyType(BodyTypeEnum::TerrainE, (void*)this);
    m_bodyTerrain=m_world->CreateBody(&bd);
    RecalculerTerrain();
}
void Carte::RecalculerTerrain()
{
    bool lastTransparent; //Détecter les changement

    b2EdgeShape shape;

    b2FixtureDef fd;
    fd.shape = &shape;
    fd.density = 0.0f;
    fd.friction = 0.6f;

    const sf::Image& img = m_itemTerrain->GetImage();
    unsigned int w = img.GetWidth();
    unsigned int h = img.GetHeight();
    unsigned int x,y;

    for(x=0; x<w;++x)
    {
        lastTransparent=true;
        for(y=0; y<h; ++y)
        {
            if((img.GetPixel(x,y).a==0&&!lastTransparent)||
                (img.GetPixel(x,y).a==255&&lastTransparent))
            {
                shape.Set(b2Vec2((float(x))*0.1, -float(y)*0.1), b2Vec2((float(x)+1)*0.1, -float(y)*0.1));
                m_fixturesMapZone[DeterminerZone(x)].push_back(m_bodyTerrain->CreateFixture(&fd));
                lastTransparent=!lastTransparent;
            }
            else if(img.GetPixel(x,y).a==255&&x>0&&x<w-1)
            {
                if(img.GetPixel(x-1,y).a==0)
                {
                    shape.Set(b2Vec2((float(x))*0.1, -(float(y)-0.5)*0.1), b2Vec2((float(x))*0.1, -(float(y)+0.5)*0.1));
                    m_fixturesMapZone[DeterminerZone(x)].push_back(m_bodyTerrain->CreateFixture(&fd));
                }
                if(img.GetPixel(x+1,y).a==0)
                {
                    shape.Set(b2Vec2((float(x)+1)*0.1, -(float(y)-0.5)*0.1), b2Vec2((float(x)+1)*0.1, -(float(y)+0.5)*0.1));
                    m_fixturesMapZone[DeterminerZone(x+1)].push_back(m_bodyTerrain->CreateFixture(&fd));
                }
            }

        }
    }

    shape.Set(b2Vec2(0, -m_taille.y*0.1), b2Vec2(0, 25000));
    m_bodyTerrain->CreateFixture(&fd);
    shape.Set(b2Vec2(float(m_taille.x)*0.1, -m_taille.y*0.1), b2Vec2(float(m_taille.x)*0.1, 25000));
    m_bodyTerrain->CreateFixture(&fd);
    shape.Set(b2Vec2(0, 25000), b2Vec2(float(m_taille.x)*0.1, 25000));
    m_bodyTerrain->CreateFixture(&fd);
}
void Carte::Fill(sf::Uint8 toFill[], int height)
{
    height=(m_taille.y-height)*4;
    int limiteRoche = height+m_taille.y*1.75;
    for(int i=m_taille.y*4-1;i>=0;i-=4)
    {
        if(i<height)
        {
            toFill[i]=0;
            toFill[i-1]=0;
            toFill[i-2]=0;
            toFill[i-3]=0;
        }
        else if(i-12<height)//3 pixels
        {
            toFill[i]=255;//a
            toFill[i-1]=0;//b
            toFill[i-2]=255;//g
            toFill[i-3]=0;//r
        }
        else
        {
            if(i>limiteRoche)
            {
                toFill[i]=255;//a
                toFill[i-1]=128;//b
                toFill[i-2]=128;//g
                toFill[i-3]=128;//r
            }
            else{
                toFill[i]=255;
                toFill[i-1]=0;
                toFill[i-2]=65;
                toFill[i-3]=108;
            }
        }
    }
}
void Carte::CopierTableau(const int tab1[], int tab2[], const unsigned int Taille1)
{
    for(unsigned int i=0;i<Taille1; tab2[i]=tab1[i],++i);
}
inline int Carte::DeterminerDeltaMin(int ordonnee, int listePts[], int nbPts)
{
    int delta=9999999, lastId, idRetour;

    for(lastId=0;lastId<nbPts;++lastId)
    {
        if(delta>abs(listePts[lastId]-ordonnee))
        {
            delta=abs(listePts[lastId]-ordonnee);
            idRetour=lastId;
        }
    }
    return idRetour;
}
int Carte::DeterminerPotionVerticale(int x, int listePts[], int nbPts)
{
    for(int i=0;i<nbPts;++i)
    {
        if(listePts[i]==x)
            return i;
    }
    return -1;
}
int Carte::DeterminerZone(int x)
{
    return x/m_tailleZone;
}
void Carte::AjouterZoneListeRegen(int zone)
{
    if(zone>=0&&zone<m_nb_zones)
    {
        for(int i : m_zonesARegenrer)
        {
            if(i==zone)
                return;
        }
        m_zonesARegenrer.push_back(zone);
    }
}
void Carte::RegenererZone(int zone)
{
    bool lastTransparent; //Détecter les changement

    b2EdgeShape shape;

    b2FixtureDef fd;
    fd.shape = &shape;
    fd.density = 0.0f;
    fd.friction = 0.6f;

    const sf::Image& img = m_itemTerrain->GetImage();
    unsigned int w = img.GetWidth()-1;
    unsigned int h = img.GetHeight();
    unsigned int x,y;
    if(w>(unsigned int)((zone+1)*m_tailleZone))
        w=(zone+1)*m_tailleZone;

    for(b2Fixture *fix : m_fixturesMapZone[zone])
        m_bodyTerrain->DestroyFixture(fix);
    m_fixturesMapZone[zone].clear();

    for(x=zone*m_tailleZone; x<w;++x)
    {
        lastTransparent=true;
        for(y=0; y<h; ++y)
        {
            if((img.GetPixel(x,y).a==0&&!lastTransparent)||
                (img.GetPixel(x,y).a==255&&lastTransparent))
            {
                shape.Set(b2Vec2((float(x))*0.1, -float(y)*0.1), b2Vec2((float(x)+1)*0.1, -float(y)*0.1));
                m_fixturesMapZone[DeterminerZone(x)].push_back(m_bodyTerrain->CreateFixture(&fd));
                lastTransparent=!lastTransparent;
            }
            else if(img.GetPixel(x,y).a==255&&x>0&&x<w)
            {
                if(img.GetPixel(x-1,y).a==0)
                {
                    shape.Set(b2Vec2((float(x))*0.1, -(float(y)-0.5)*0.1), b2Vec2((float(x))*0.1, -(float(y)+0.5)*0.1));
                    m_fixturesMapZone[DeterminerZone(x)].push_back(m_bodyTerrain->CreateFixture(&fd));
                }
                if(img.GetPixel(x+1,y).a==0)
                {
                    shape.Set(b2Vec2((float(x)+1)*0.1, -(float(y)-0.5)*0.1), b2Vec2((float(x)+1)*0.1, -(float(y)+0.5)*0.1));
                    m_fixturesMapZone[DeterminerZone(x+1)].push_back(m_bodyTerrain->CreateFixture(&fd));
                }
            }

        }
    }
}
void Carte::RegenererZonesModifiees()
{
    for(int i: m_zonesARegenrer)
        RegenererZone(i);
    m_zonesARegenrer.clear();
}
int Carte::Width()
{
    return m_taille.x;
}
int Carte::YMin(int x_debut, int x_fin)
{
    const sf::Image &img = m_itemTerrain->GetImage();
    int y_return = img.GetHeight();
    int h = img.GetHeight();
    for(int x = x_debut; x <= x_fin; ++x)
    {
        for(int y=0;y<h;++y)
        {
            if(img.GetPixel(x,y).a!=0)
            {
                if(y<y_return)
                    y_return=y;
                break;
            }
        }
    }
    return y_return;
}
void Carte::Work()
{
    m_UpdateTime();
    float ratio = float(m_time.GetElapsedTime().AsMilliseconds()%m_dayDuration)/float(m_dayDuration);
    m_nodeCiel->SetAbsoluteRotation(180+360*ratio);
}
#include "../../core/logger.h"

void Carte::m_UpdateTime()
{
    int demiDay = m_dayDuration/2;
    int currTime = m_time.GetElapsedTime().AsMilliseconds()%m_dayDuration;
    m_itemSoleil->SetColor(sf::Color(255,225,0));
    if(currTime>demiDay)
    {
        m_fondCiel->SetColor(sf::Color(0,0,0));//Nuit
    }
    else //Calcul du dégradé jour
    {
        float ratio;
        if(currTime<demiDay/4)
        {
            if(currTime<demiDay/8)
            {
                ratio = float(currTime)/float(demiDay/8);
                m_fondCiel->SetColor(sf::Color(255*ratio,121*ratio,31*ratio, 255*ratio));
                m_fondCielTransition->SetColor(sf::Color(255*ratio,121*ratio,31*ratio, 128*ratio));
                ratio*=0.5;
                m_itemSoleil->SetColor(sf::Color(255,76+149*ratio,0));
            }
            else
            {
                ratio = float(currTime-(demiDay/8))/float(demiDay/8);
                m_fondCiel->SetColor(sf::Color(255*(1-ratio),70*(ratio)+121,224*ratio+31));
                m_fondCielTransition->SetColor(sf::Color(255*(1-ratio),70*(ratio)+121,224*ratio+31, 128));
                ratio=ratio*0.5+0.5;
                m_itemSoleil->SetColor(sf::Color(255,76+149*ratio,0));
            }
        }
        else if(currTime>demiDay*3/4)
        {
            if(currTime>demiDay*7/8)
            {
                ratio = float(currTime-(demiDay*7/8))/float(demiDay/8);
                m_fondCiel->SetColor(sf::Color(255*(1-ratio),121*(1-ratio),31*(1-ratio)));
                m_fondCielTransition->SetColor(sf::Color(255*(1-ratio),121*(1-ratio),31*(1-ratio), 128*(1-ratio)));
                ratio=ratio*0.5+0.5;
                m_itemSoleil->SetColor(sf::Color(255,76+149*(1-ratio),0));
            }
            else
            {
                ratio = float(currTime-(demiDay*3/4))/float(demiDay/8);
                m_fondCiel->SetColor(sf::Color(255*(ratio),70*(1-ratio)+121,224*(1-ratio)+31));
                m_fondCielTransition->SetColor(sf::Color(255*(ratio),70*(1-ratio)+121,224*(1-ratio)+31, 128));
                ratio*=0.5;
                m_itemSoleil->SetColor(sf::Color(255,76+149*(1-ratio),0));
            }

        }
        else
        {
            m_fondCiel->SetColor(sf::Color(0,191,255));
            m_fondCielTransition->SetColor(sf::Color(0,191,255, 128));
            m_itemSoleil->SetColor(sf::Color(255,225,0));
        }
    }
}
