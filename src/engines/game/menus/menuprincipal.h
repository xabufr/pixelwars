#ifndef MENUPRINCIPAL_H
#define MENUPRINCIPAL_H
#include "engines/graphics/graphicalengine.h"


class MenuPrincipal
{
    public:
        MenuPrincipal();
        void Show();
        virtual ~MenuPrincipal();

        static void CallbackQuitter(GuiItem*);
        static void JouerScinde(GuiItem*);

    protected:
    private:
        GuiContener *m_contenerMenu;
        GuiButtonItem *m_btn_quitter, *m_btn_jouer_scinde;

        bool m_gameToStart;
};

#endif // MENUPRINCIPAL_H
