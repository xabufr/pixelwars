#ifndef MENUOPTIONS_H
#define MENUOPTIONS_H

class GuiItem;
class GuiButtonItem;
class GuiContener;
class GuiColorSelector;
class MenuOptions
{
    public:
        MenuOptions();
        virtual ~MenuOptions();

    private:
        void Afficher();
        GuiContener *m_contener;
        GuiColorSelector *m_couleurJ1;
};

#endif // MENUOPTIONS_H
