#ifndef MENUOPTIONS_H
#define MENUOPTIONS_H

class GuiItem;
class GuiButtonItem;
class GuiContener;
class GuiColorSelector;
class GuiTextInputItem;
class GuiTextItem;
class MenuOptions
{
    public:
        MenuOptions();
        virtual ~MenuOptions();

    private:
        void Afficher();
        GuiContener *m_contener, *contJ2, *contJ1;
        GuiColorSelector *m_couleurJ1, *m_couleurJ2;
        GuiTextInputItem *m_nameJ1, *m_nameJ2;
        GuiTextItem *m_labelJ1, *m_labelJ2;
};

#endif // MENUOPTIONS_H
