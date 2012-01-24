#ifndef MENUOPTIONSPARTIE_H
#define MENUOPTIONSPARTIE_H

enum TypePartie{
    ECRAN_SCINDE
};
class GuiItem;
class MenuOptionsPartie
{
    public:
        MenuOptionsPartie(TypePartie);
        virtual ~MenuOptionsPartie();
        static void ChangerTailleCarte(GuiItem*);
        static void CommencerPartie(GuiItem*);
    protected:
    private:
        void StartMenuEcranScinde();
        float m_tailleCarte;
        bool m_start;
};

#endif // MENUOPTIONSPARTIE_H
