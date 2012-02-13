#ifndef MENUOPTIONS_H
#define MENUOPTIONS_H
#include <string>
#include <unordered_map>

class GuiItem;
class GuiButtonItem;
class GuiContener;
class GuiColorSelector;
class GuiTextInputItem;
class GuiTextItem;
struct btnToucheData
{
    std::string cible;
    int index;
    GuiButtonItem *item;
};
class MenuOptions
{
    public:
        MenuOptions();
        virtual ~MenuOptions();
        void CallMenuTouches(btnToucheData param);
        static void CallbackChangerTouche(GuiItem*);

    private:
        void Afficher();
        GuiContener *m_contener, *contJ2, *contJ1;
        GuiColorSelector *m_couleurJ1, *m_couleurJ2;
        GuiTextInputItem *m_nameJ1, *m_nameJ2;
        GuiTextItem *m_labelJ1, *m_labelJ2;
        GuiButtonItem *m_btn_droite[2], *m_btn_gauche[2], *m_btn_haut[2], *m_btn_bas[2], *m_btn_tirer[2], *m_btn_zoom[2], *m_btn_dezoom[2];
        GuiTextItem *m_labelHaut[2], *m_labelBas[2], *m_labelGauche[2], *m_labelDroite[2], *m_labelTirer[2], *m_labelZoom[2], *m_labelDezoom[2];

        std::unordered_map<GuiItem*, btnToucheData> m_dataBtnTouches;

        btnToucheData m_dataMenuTouches;
        bool m_changerTouche;

        void LoadTouches();
};

#endif // MENUOPTIONS_H
