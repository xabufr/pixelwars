#ifndef MENUPRINCIPAL_H
#define MENUPRINCIPAL_H

class GuiContener;
class GuiItem;
class GuiButtonItem;
class GuiSliderNumberItem;
class MenuPrincipal
{
    public:
        MenuPrincipal();
        void Show();
        virtual ~MenuPrincipal();

        static void CallbackQuitter(GuiItem*);
        static void CallbackOptions(GuiItem*);
        static void JouerScinde(GuiItem*);

    protected:
    private:
        GuiContener *m_contenerMenu;
        GuiButtonItem *m_btn_quitter, *m_btn_jouer_scinde, *m_btn_options;

        bool m_gameToStart, m_options;
};

#endif // MENUPRINCIPAL_H
