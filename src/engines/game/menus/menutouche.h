#ifndef MENUTOUCHE_H
#define MENUTOUCHE_H
#include <string>
#include <SFML/Graphics.hpp>

class GuiTextItem;
class GuiNode;
class MenuTouche
{
    public:
        MenuTouche(const std::string& = "Appuyez sur une touche, ou echap pour annuler");
        sf::Keyboard::Key GetTouche();
        virtual ~MenuTouche();
    protected:
    private:
        GuiTextItem* m_label;
        GuiNode* m_node;
};

#endif // MENUTOUCHE_H
