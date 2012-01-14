#ifndef ENGINE_H
#define ENGINE_H
#include <SFML2/System.hpp>
#include "type.h"
class Engine
{
    public:
        Engine();
        virtual ~Engine();
        virtual EngineType GetEngineId() const = 0;
    protected:
    private:
};

#endif // ENGINE_H
