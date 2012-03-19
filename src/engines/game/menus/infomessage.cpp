#include "infomessage.h"
#include "../../graphics/graphicalengine.h"

InfoMessage::InfoMessage(const std::string &message)
{
    GuiTextItem *m_label = new GuiTextItem;
    m_label->SetText(message);

    GuiButtonItem *ok = new GuiButtonItem;
    ok->SetText("OK");

    GuiNode *m_node = GraphicalEngine::GetInstance()->GetGuiManager()->GetRootNode()->AddGuiNode();
    m_node->AddItem(m_label);
    m_node->AddItem(ok);

    ok->SetRelativePosition(0, 100);
    ok->SetData("this", this);
    ok->SetCallBack("clicked", CallbackOK);

    m_run = true;
    GraphicalEngine *engine = GraphicalEngine::GetInstance();
    sf::Event event;
    while(m_run)
    {
        while(engine->GetRenderWindow()->PollEvent(event))
        {
            engine->GetGuiManager()->HandleEvent(event);
        }
        engine->DrawScene();
    }
    m_node->Remove();
}

InfoMessage::~InfoMessage()
{
    //dtor
}
void InfoMessage::CallbackOK(GuiItem* item)
{
    InfoMessage *a = (InfoMessage*)item->GetData("this");
    a->m_run=false;
}