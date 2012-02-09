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
        GuiButtonItem *m_btn_droite[2], *m_btn_gauche[2], *m_btn_haut[2], *m_btn_bas[2];
        GuiTextItem *m_labelHaut[2], *m_labelBas[2], *m_labelGauche[2], *m_labelDroite[2];
};

#endif // MENUOPTIONS_H
