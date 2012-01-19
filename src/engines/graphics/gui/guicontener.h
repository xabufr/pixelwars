#ifndef GUICONTENER_H
#define GUICONTENER_H
#include "guinode.h"
#include <SFML2/Graphics.hpp>
#include <deque>

class GuiItem;
class GuiContener: public GuiNode
{
    public:
        GuiContener(SceneManager* mng, SceneNode* parent);
        void AjouterItem(GuiItem*, int, int);
        sf::Vector2f & GetSize();
        void SetMaxSize(const sf::Vector2f&);
        const sf::Vector2f & GetMaxSize();
        void SetEspacement(const sf::Vector2f&);
        virtual ~GuiContener();
        void HandleEvent(const sf::Event&);
    private:
        void CalculerPositions();

        sf::Vector2f m_maxSize, m_size, m_espacement;
        std::deque<std::deque<GuiItem*>> m_items;
};

#endif // GUICONTENER_H
