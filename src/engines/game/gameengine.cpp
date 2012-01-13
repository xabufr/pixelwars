#include "gameengine.h"
#include "engines/graphics/graphicalengine.h"
#include "core/logger.h"
#include <libnoise/noise.h>
#include <SFML2/System.hpp>
#include <Box2D/Box2D.h>
GameEngine::GameEngine()
{
    //ctor
}

GameEngine::~GameEngine()
{
    //dtor
}
inline void Fill(sf::Uint8 toFill[], int height)
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
        else
        {
            if(i<800)
            {
                toFill[i]=255;//a
                toFill[i-1]=0;//b
                toFill[i-2]=0;//g
                toFill[i-3]=255;//r
            }
            else{
                toFill[i]=255;
                toFill[i-1]=255;
                toFill[i-2]=0;
                toFill[i-3]=0;
            }
        }
    }
}
inline int DeterminerDeltaMin(int ordonnee, int listePts[], int nbPts)
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
inline int DeterminerSecondDeltaMin(int ordonnee, int listePts[], int nbPts)
{
    int delta=9999999, lastId, idRetour, retour1;

    for(lastId=0;lastId<nbPts;++lastId)
    {
        if(delta>abs(listePts[lastId]-ordonnee))
        {
            delta=abs(listePts[lastId]-ordonnee);
            idRetour=retour1;
            retour1=lastId;
        }
    }
    return idRetour;
}
inline int DeterminerDeltaValeurMax(int ordonnee, int listePts[], int nbPts, int deltaMax)
{
    int lastId;

    for(lastId=0;lastId<nbPts;++lastId)
    {
        if(deltaMax<abs(listePts[lastId]-ordonnee))
        {
            return lastId;
        }
    }
}
inline void CopierTableau(const int tab1[], int tab2[], const unsigned int Taille1)
{
    for(unsigned int i=0;i<Taille1; tab2[i]=tab1[i],++i);
}
inline void CreerTerrainBox(b2Body* body, SceneNodeTextureItem *itemTerrain)
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

    const sf::Image& img = itemTerrain->GetImage();
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
                body->CreateFixture(&fd);
            }


        }
        CopierTableau(ptsEnCours, lastPts, nbPtsCourant);
        nbPtsLast=nbPtsCourant;
    }
}
void GameEngine::Start()
{
    GraphicalEngine* gengine = GraphicalEngine::GetInstance();
    sf::RenderWindow *app = gengine->CreateRenderWindow(sf::VideoMode(600,400), "Test");

    SceneNode* worldNode = gengine->GetSceneManager()->GetRootNode()->AddSceneNode();
    SceneNodeTextureItem* worldItem = (SceneNodeTextureItem*) worldNode->AddItem(new SceneNodeTextureItem);
    worldItem->SetRelativePosition(0,0);
    worldItem->CreateTexture(sf::Vector2i(600,400));
    sf::Uint8 tabFill[400*4];

    noise::module::Perlin gen;

    srand(time(NULL));
    gen.SetSeed(rand());

    b2World* m_world;
    b2Vec2 gravity;
	gravity.Set(0.0f, -10.0f);
	m_world = new b2World(gravity);

    b2Body* ground = NULL;
    b2BodyDef bd;
    ground = m_world->CreateBody(&bd);
    b2EdgeShape shape;

    b2FixtureDef fd;
    fd.shape = &shape;
    fd.density = 0.0f;
    fd.friction = 0.6f;

    float y;
    for(int i=0;i<600;++i)
    {
        y = 200+150*gen.GetValue(float(i)*0.002,0.5,0.5);

        shape.Set(b2Vec2(float(i-1)*0.1, -y*0.1), b2Vec2(float(i)*0.1, -y*0.1));
        //ground->CreateFixture(&fd);

        Fill(tabFill, y);
        worldItem->UpdateTexture(tabFill, 1,400,i,0);
    }
    CreerTerrainBox(ground, worldItem);

    bd.position = b2Vec2(10, 0);
    bd.type=b2_dynamicBody;
    b2Body *corpVoiture = m_world->CreateBody(&bd);
    b2PolygonShape polyShape;
    polyShape.SetAsBox(3,0.5);
    fd.shape = &polyShape;
    fd.density=1.f;
    fd.friction=0.f;
    corpVoiture->CreateFixture(&fd);

    bd.position = b2Vec2(7, 0);
    b2Body *roue1 = m_world->CreateBody(&bd);

    b2CircleShape roueShape;
    roueShape.m_radius=2.f;
    fd.shape=&roueShape;
    fd.friction=1000.f;
    roue1->CreateFixture(&fd);

    bd.position = b2Vec2(13, 0);
    b2Body *roue2 = m_world->CreateBody(&bd);
    roue2->CreateFixture(&fd);

    b2WheelJointDef jd;
    b2Vec2 axis(0,1);
    jd.Initialize(corpVoiture, roue1, roue1->GetPosition(), axis);
    jd.motorSpeed=-0.f;
    jd.maxMotorTorque = 400.0f;
    jd.enableMotor = true;
    jd.frequencyHz = 4.f;
    jd.dampingRatio = 0.7;
    b2WheelJoint* jointure1 = (b2WheelJoint*)m_world->CreateJoint(&jd);

    jd.Initialize(corpVoiture, roue2, roue2->GetPosition(), axis);
    jd.motorSpeed=-0.f;
    jd.maxMotorTorque = 400.0f;
    jd.enableMotor = true;
    jd.frequencyHz = 4.f;
    jd.dampingRatio = 0.7;
    b2WheelJoint* jointure2 = (b2WheelJoint*)m_world->CreateJoint(&jd);

    SceneNode* nodeVoiture = (SceneNode*) gengine->GetSceneManager()->GetRootNode()->AddSceneNode();
    SceneNodeShapeItem* corpVoitureItem = (SceneNodeShapeItem*) nodeVoiture->AddItem(new SceneNodeShapeItem);
    corpVoitureItem->SetSize(60, 10);
    corpVoitureItem->SetColor(sf::Color(255,0,0));
    corpVoitureItem->SetRelativePosition(-30,-5);

    SceneNodeCircleShapeItem* r1 = (SceneNodeCircleShapeItem*)nodeVoiture->AddItem(new SceneNodeCircleShapeItem);
    r1->SetRadius(20);
    r1->SetRelativePosition(-50,-20);
    r1->SetColor(sf::Color(0,255,0));

    SceneNodeCircleShapeItem* r2 = (SceneNodeCircleShapeItem*)nodeVoiture->AddItem(new SceneNodeCircleShapeItem);
    r2->SetRadius(20);
    r2->SetRelativePosition(10,-20);
    r2->SetColor(sf::Color(0,255,0));


    sf::Event event;
    while(app->IsOpened())
    {
        while(app->PollEvent(event))
        {
            gengine->GetGuiManager()->HandleEvent(event);
            if(event.Type==sf::Event::Closed)
                app->Close();
            if(event.Type==sf::Event::KeyPressed)
            {
                if(event.Key.Code == sf::Keyboard::Key::Left)
                {
                    jointure1->SetMotorSpeed(10000);
                    jointure2->SetMotorSpeed(10000);
                }
                if(event.Key.Code == sf::Keyboard::Key::Right)
                {
                    jointure1->SetMotorSpeed(-10000);
                    jointure2->SetMotorSpeed(-10000);
                }
            }
            else if(event.Type==sf::Event::KeyReleased)
            {
                jointure1->SetMotorSpeed(0);
                jointure2->SetMotorSpeed(0);
            }
            else if(event.Type == sf::Event::MouseButtonPressed)
            {
                m_world->DestroyBody(ground);
                b2BodyDef bdtemp;
                ground = m_world->CreateBody(&bdtemp);
                worldItem->DrawCircle(50.f, sf::Color(0,0,0,0), GuiManager::GetMousePosition());
                CreerTerrainBox(ground, worldItem);
            }
        }
        m_world->Step(1.f/60.f, 8, 3);
        nodeVoiture->SetAbsolutePosition(10*corpVoiture->GetWorldCenter().x, -corpVoiture->GetWorldCenter().y*10);
        nodeVoiture->SetAbsoluteRotation(-corpVoiture->GetAngle()*180/3.14);
        gengine->DrawScene();
    }
    GraphicalEngine::Kill();
}
