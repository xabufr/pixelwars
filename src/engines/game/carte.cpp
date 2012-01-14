#include "carte.h"
#include <libnoise/noise.h>

Carte::Carte(b2World* world, const sf::Vector2i& taille, int seed)
{
    m_world=world;
    m_nodeTerrain = GraphicalEngine::GetInstance()->GetSceneManager()->GetRootNode()->AddSceneNode();
    m_itemTerrain = new SceneNodeTextureItem;
    m_nodeTerrain->AddItem(m_itemTerrain);
    m_destruction = false;
    m_modified = false;
    Generer(taille, seed);
}

Carte::~Carte()
{
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
        RecreerTerrain();
}
void Carte::Generer(const sf::Vector2i& taille, int seed)
{
    m_itemTerrain->CreateTexture(taille);
    noise::module::Perlin gen;
    gen.SetSeed(seed);
    sf::Uint8 filler[taille.y*4];
    for(int y,x=0;x<taille.x;++x)
    {
        y = taille.y*0.5+((taille.y*0.25)*gen.GetValue(float(x)*0.0015, 0.5,0.5));
        Fill(filler, y);
        m_itemTerrain->UpdateTexture(filler, 1, taille.y, x, 0);
    }
    b2BodyDef bd;
    m_bodyTerrain=m_world->CreateBody(&bd);
    RecalculerTerrain();
}

void Carte::AjouterExplosion(sf::Vector2f position, float radius)
{
    m_itemTerrain->DrawCircle(radius, sf::Color(0,0,0,0), position);
    if(!m_destruction)
    {
        RecreerTerrain();
    }
    else
        m_modified = true;
}
void Carte::RecreerTerrain()
{
    m_world->DestroyBody(m_bodyTerrain);
    b2BodyDef bd;
    m_bodyTerrain=m_world->CreateBody(&bd);
    RecalculerTerrain();
}
void Carte::RecalculerTerrain()
{
    const unsigned int maxPtsLevel = 10; //Nombre de points stockable en mémoire au max
    unsigned int nbPtsLast=1, nbPtsCourant;
    int lastPts[maxPtsLevel], ptsEnCours[maxPtsLevel]; //Ordonnée des derniers points stockés
    bool lastTransparent; //Détecter les changement

    lastPts[0]=0;

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
        nbPtsCourant=0;
        lastTransparent=true;
        for(y=0; y<h&&nbPtsCourant<maxPtsLevel; ++y)
        {
            if((img.GetPixel(x,y).a==0&&!lastTransparent)||
                (img.GetPixel(x,y).a==255&&lastTransparent))
            {
                ptsEnCours[nbPtsCourant++]=y;
                lastTransparent=!lastTransparent;//On a changé d'état
            }

        }
        for(unsigned int indexPt=0;indexPt<nbPtsCourant;++indexPt)
        {
            if(nbPtsLast>=nbPtsCourant)
            {
                int id = DeterminerDeltaMin(ptsEnCours[indexPt], lastPts, nbPtsLast);

                shape.Set(b2Vec2((float(x)-1.f)*0.1, -float(lastPts[id])*0.1), b2Vec2(float(x)*0.1, -float(ptsEnCours[indexPt])*0.1));
                m_bodyTerrain->CreateFixture(&fd);
            }


        }
        CopierTableau(ptsEnCours, lastPts, nbPtsCourant);
        nbPtsLast=nbPtsCourant;
    }
}
void Carte::Fill(sf::Uint8 toFill[], int height)
{
    height*=4;
    for(int i=400*4-1;i>=0;i-=4)
    {
        if(i<height)
        {
            toFill[i]=0;
            toFill[i-1]=0;
            toFill[i-2]=0;
            toFill[i-3]=0;
        }
        else if(i-12<height)
        {
            toFill[i]=255;//a
            toFill[i-1]=0;//b
            toFill[i-2]=255;//g
            toFill[i-3]=0;//r
        }
        else
        {

            if(i>height*2)
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