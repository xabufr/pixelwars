#ifndef GUICONTENER_H
#define GUICONTENER_H
#include "guinode.h"
#include <SFML2/Graphics.hpp>
#include <deque>
#include "guielement.h"

class GuiContener: public GuiNode, public GuiElement
{
    public:
        GuiContener(SceneManager* mng, SceneNode* parent);
        virtual void AjouterItem(GuiElement*, int, int);
        virtual sf::Vector2f GetSize() const;
        void SetMaxSize(const sf::Vector2f&);
        const sf::Vector2f & GetMaxSize();
        void SetEspacement(const sf::Vector2f&);
        virtual ~GuiContener();
        void HandleEvent(const sf::Event&);
        virtual GuiElementType GetType() const;

    private:
        void CalculerPositions();

        sf::Vector2f m_maxSize, m_size, m_espacement;
        std::deque<std::deque<GuiElement*>> m_items;
};

#endif // GUICONTENER_H
