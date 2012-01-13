#ifndef GUIWINDOWITEM_H
#define GUIWINDOWITEM_H

#include "guinode.h"
#include "engines/graphics/gui/guinode.h"
#include "../scenenodeshapeitem.h"
#include "guibuttonitem.h"
#include "guitextitem.h"

class GuiWindowNode: public GuiNode
{
    public:
        GuiWindowNode(SceneManager*, SceneNode*);
        virtual ~GuiWindowNode();
        virtual void HandleEvent(const sf::Event&);

        void SetClosable(bool);
        bool IsClosable()const;
        static void CloseWindowCallBack(GuiItem*);

        void SetWindowTitle(const sf::String&);

    protected:
        SceneNodeShapeItem *m_windowShape;
        GuiButtonItem *m_btnClose;
        GuiTextItem *m_windowTitle;
        bool m_moving, m_closable;
        sf::Vector2f m_posClickMoving;

        void ClosableChanged();

};

#endif // GUIWINDOWITEM_H
