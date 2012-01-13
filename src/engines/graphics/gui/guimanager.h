#ifndef GUIMANAGER_H
#define GUIMANAGER_H

#include "../scenemanager.h"
#include "guiitem.h"
#include "guinode.h"
#include <vector>
#include <map>
/// @todo Documenter
class GuiManager: public SceneManager
{
    public:
        GuiManager();
        virtual ~GuiManager();
        GuiItem* AddItem(GuiItem*);
        virtual void Draw();
        void HandleEvent(const sf::Event&);
        GuiNode* GetRootNode();
        virtual void RemoveNode(SceneNode*);

        static sf::Vector2f GetMousePosition();

        void LockEvent(GuiNode*);
        void UnlockEvent();

        void AddToRemoveNode(GuiNode*);

    private:
        GuiNode* m_guiRootNode;
        GuiNode* m_eventLockedBy;

        std::vector<GuiNode*> m_toRemove;
};

#endif // GUIMANAGER_H
