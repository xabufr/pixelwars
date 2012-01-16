#include "core/consolelogger.h"
#include "core/exception.h"

#include "engines/game/gameengine.h"

int main()
{
    Logger::SetLogger(new ConsoleLogger);
    try
    {
        GameEngine game;
        game.Start();
    }
    catch(AssertException &e)
    {
        std::cout<<e.What();
    }
    catch(Exception &e)
    {
        std::cout<<e.What();
    }
    return 0;
}
