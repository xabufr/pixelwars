#ifndef GUICOLORSELECTOR_H
#define GUICOLORSELECTOR_H

#include "guicontener.h"
#include <SFML2/Graphics.hpp>

class SceneManager;
class SceneNode;
class GuiSliderNumberItem;
class GuiTextItem;
class SceneNodeShapeItem;
class GuiColorSelector: protected GuiContener
{
    public:
        GuiColorSelector(SceneManager*, SceneNode*);
        virtual ~GuiColorSelector();

        virtual void HandleEvent(const sf::Event&);

        static void CallbackCouleurChanged(GuiItem*);

        void SetColor(const sf::Color&);

    protected:
    private:
        GuiSliderNumberItem *m_selectR, *m_selectG, *m_selectB;
        GuiTextItem *m_textR, *m_textG, *m_textB, *m_levelR, *m_levelG, *m_levelB;
        SceneNodeShapeItem *m_affCouleur;

        void Update();
};

#endif // GUICOLORSELECTOR_H
