#ifndef ENGINE_H
#define ENGINE_H
#include <SFML/System.hpp>
#include "type.h"
class EngineEvent;
class Engine
{
    public:
        Engine();
        virtual ~Engine();
        virtual EngineType GetEngineId() const = 0;
        virtual void Work() = 0;
        virtual const std::vector<EngineEvent*>& GetEvents() const;
        virtual void ClearEvents();
        virtual void HandleEngineEvent(EngineEvent*) = 0;
        virtual void AddEvent(EngineEvent*);
    protected:
        std::vector<EngineEvent*> m_engineEvents;
};

#endif // ENGINE_H
