#include "graphicalengine.h"
#include "core/logger.h"
#include "engines/graphics/scenemanager.h"
#include "engines/graphics/texturemanager.h"
template<>
GraphicalEngine *Singleton<GraphicalEngine>::m_singleton = 0;

GraphicalEngine::GraphicalEngine()
{
    Logger::Log()<<"Initialisation du moteur graphique" << Logger::endl;
    m_sceneManager = new SceneManager;
    m_guiManager = new GuiManager;
    m_cameraManager = new CameraManager;
    m_listeViews = &m_cameraManager->GetCamerasList();

    m_tempFramerate=1000;
}

sf::RenderWindow* GraphicalEngine::CreateRenderWindow(sf::VideoMode mode, const std::string &title, sf::Uint32 style, const sf::ContextSettings &settings)
{
    m_app = new sf::RenderWindow(mode, title, style, settings);
    m_app->SetFramerateLimit(60);
    m_framerate=0.f;
    m_timerFramerate.Reset();
    Logger::Log()<<"Création d'une fenêtre de rendu"<<Logger::endl;
    return m_app;
}
sf::RenderWindow* GraphicalEngine::GetRenderWindow()
{
    return m_app;
}
GraphicalEngine::~GraphicalEngine()
{
    delete m_sceneManager;
    delete m_guiManager;
    delete m_cameraManager;
    TextureManager::Kill();
    Logger::Log()<<"Destruction moteur graphique" << Logger::endl;
    delete m_app;
}
SceneManager* GraphicalEngine::GetSceneManager() const
{
    return m_sceneManager;
}
CameraManager* GraphicalEngine::GetCameraManager() const
{
    return m_cameraManager;
}
void GraphicalEngine::DrawScene()
{
    m_app->Clear();

    if(m_listeViews->size()==0)
    {
        sf::View* nCam = m_cameraManager->AddCamera();
        nCam->SetSize(m_app->GetWidth(), m_app->GetHeight());
        nCam->SetViewport(sf::FloatRect(0,0,1,1));
        nCam->SetCenter(nCam->GetSize()/2.f);
    }
    for(sf::View *view: *m_listeViews )
    {
        m_app->SetView(*view);
        m_sceneManager->Draw();
    }
    m_app->SetView(m_app->GetDefaultView());
    m_guiManager->Draw();
    m_app->Display();
    if(m_timerFramerate.GetElapsedTime()>m_tempFramerate)
    {
        m_framerate=(float(m_nbFrames)*1000.f)/float(m_tempFramerate);
        m_nbFrames=0;
        m_timerFramerate.Reset();
    }
    ++m_nbFrames;
}
void GraphicalEngine::SetFramerateCaptureTime(int temp)
{
    m_tempFramerate = temp;
}
float GraphicalEngine::GetFramerate()
{
    return m_framerate;
}
GuiManager* GraphicalEngine::GetGuiManager() const
{
    return m_guiManager;
}
