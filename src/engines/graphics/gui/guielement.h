#ifndef GUIELEMENT_H
#define GUIELEMENT_H

#include <SFML2/Graphics.hpp>
enum GuiElementType{TypeNode, TypeItem, TypeContener};
class GuiElement
{
    public:
        GuiElement();
        virtual ~GuiElement();
        virtual sf::Vector2f GetSize() const= 0;
        virtual GuiElementType GetType() const= 0;
    protected:
    private:
};

#endif // GUIELEMENT_H
