#include "menutouche.h"
#include "../../graphics/graphicalengine.h"

MenuTouche::MenuTouche(const std::string& txt)
{
    m_label = new GuiTextItem;
    m_label->SetText(txt);

    m_node = GraphicalEngine::GetInstance()->GetGuiManager()->GetRootNode()->AddGuiNode();
    m_node->AddItem(m_label);
}

MenuTouche::~MenuTouche()
{
    m_node->Remove();
}

sf::Keyboard::Key MenuTouche::GetTouche()
{
    bool continuer=true;
    GraphicalEngine *engine = GraphicalEngine::GetInstance();
    sf::Event event;
    while(continuer)
    {
        while(engine->GetRenderWindow()->PollEvent(event))
        {
            engine->GetGuiManager()->HandleEvent(event);
            if(event.Type==sf::Event::EventType::KeyPressed)
            {
                return event.Key.Code;
            }
        }
        engine->DrawScene();
    }
    return sf::Keyboard::Key::KeyCount;
}
