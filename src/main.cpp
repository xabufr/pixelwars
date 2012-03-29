#include "core/consolelogger.h"
#include "core/exception.h"

#include "engines/game/gameengine.h"
#include "engines/game/menus/menuprincipal.h"

int main()
{
    Logger::SetLogger(new ConsoleLogger);
    try
    {
        /*GameEngine game;
        game.Start();*/
        MenuPrincipal *menu = new MenuPrincipal;
        menu->Show();
        delete menu;
    }
    catch(AssertException &e)
    {
        std::cout<<e.What()<<std::endl;
        GraphicalEngine::Kill();
        return 0;
    }
    catch(Exception &e)
    {
        std::cout<<e.What()<<std::endl;
        return 0;
    }
    return 0;
}
