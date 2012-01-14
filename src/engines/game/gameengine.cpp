#include "gameengine.h"
#include "engines/graphics/graphicalengine.h"
#include "core/logger.h"
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
EngineType GameEngine::GetEngineId() const
{
    return EngineType::Game_engine;
}
void GameEngine::Start()
{
    GraphicalEngine* gengine = GraphicalEngine::GetInstance();
    sf::RenderWindow *app = gengine->CreateRenderWindow(sf::VideoMode(600,400), "Test");


    b2Vec2 gravity;
	gravity.Set(0.0f, -10.0f);
	m_world = new b2World(gravity);
	m_world->SetContactListener(&m_listner);
	m_carte = new Carte(m_world, sf::Vector2i(600,400), 0);

    b2BodyDef bd;
    //ground = m_world->CreateBody(&bd);
    b2EdgeShape shape;

    b2FixtureDef fd;
    fd.shape = &shape;
    fd.density = 0.0f;
    fd.friction = 0.6f;



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
        }
        m_world->Step(1.f/60.f, 8, 3);
        nodeVoiture->SetAbsolutePosition(10*corpVoiture->GetWorldCenter().x, -corpVoiture->GetWorldCenter().y*10);
        nodeVoiture->SetAbsoluteRotation(-corpVoiture->GetAngle()*180/3.14);
        gengine->DrawScene();
        GererExplosions();
        //Logger::Log()<<roue1->GetAngle()<<Logger::endl;
    }
    GraphicalEngine::Kill();
}
void GameEngine::GererExplosions()
{
    const std::vector<ExplosionPosition> &explosions = m_listner.GetExplosions();
    m_carte->DemarrerDestruction();
    for(const ExplosionPosition &exp : explosions)
    {
        m_carte->AjouterExplosion(exp.position, exp.radius);
    }
    m_carte->FinirDestruction();

    m_listner.Clear();
}
