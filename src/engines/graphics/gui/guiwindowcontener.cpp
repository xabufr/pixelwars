#include "guiwindowcontener.h"
#include "guiwindownode.h"

GuiWindowContener::GuiWindowContener(SceneManager *mng, SceneNode* parent): GuiContener(mng, parent)
{

}

GuiWindowContener::~GuiWindowContener()
{
    //dtor
}
void GuiWindowContener::AjouterItem(GuiItem* item, int x, int y)
{
    GuiContener::AjouterItem(item, x, y);
    ((GuiWindowNode*)m_parent)->CalculerTaille();
}
