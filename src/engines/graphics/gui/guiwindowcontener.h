#ifndef GUIWINDOWCONTENER_H
#define GUIWINDOWCONTENER_H
#include "guicontener.h"

class GuiWindowContener: public GuiContener
{
    public:
        GuiWindowContener(SceneManager*, SceneNode*);
        virtual ~GuiWindowContener();
        virtual void AjouterItem(GuiItem*, int, int);
    protected:
    private:
};

#endif // GUIWINDOWCONTENER_H
