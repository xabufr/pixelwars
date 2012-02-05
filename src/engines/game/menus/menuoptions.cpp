#include "menuoptions.h"
#include "engines/graphics/graphicalengine.h"

MenuOptions::MenuOptions()
{
    m_contener = GraphicalEngine::GetInstance()->GetGuiManager()->GetRootNode()->AddContener();
    m_couleurJ1 = (GuiColorSelector*)m_contener->AddSceneNode((SceneNode*)new GuiColorSelector(GraphicalEngine::GetInstance()->GetGuiManager(), m_contener));
    Afficher();
}

MenuOptions::~MenuOptions()
{
    GraphicalEngine::GetInstance()->GetGuiManager()->RemoveNode(m_contener);
}
void MenuOptions::Afficher()
{
    sf::Event event;
    GraphicalEngine *engine = GraphicalEngine::GetInstance();
    sf::RenderWindow *app = engine->GetRenderWindow();
    bool cont = true;
    while(cont)
    {
        while(app->PollEvent(event))
        {
            engine->GetGuiManager()->HandleEvent(event);
            if(event.Type==sf::Event::Closed)
                cont = false;
        }
        GraphicalEngine::GetInstance()->DrawScene();
    }
}
