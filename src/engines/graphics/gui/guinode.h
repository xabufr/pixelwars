#ifndef GUINODE_H
#define GUINODE_H

#include "../scenenode.h"

class GuiWindowNode;
class GuiNode : public SceneNode
{
    public:
        GuiNode(SceneManager* mng, SceneNode* parent=0);
        virtual ~GuiNode();
        virtual void HandleEvent(const sf::Event&);
        GuiWindowNode* AddWindow();

    protected:
        void m_RemoveMeNextDraw();
};

#endif // GUINODE_H
