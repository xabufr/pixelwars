#include "engine.h"
#include "engineevent.h"

Engine::Engine()
{
    //ctor
}

Engine::~Engine()
{
    //dtor
}
void Engine::ClearEvents()
{
    for(EngineEvent* event : m_engineEvents)
        delete event;
    m_engineEvents.clear();
}
const std::vector<EngineEvent*>& Engine::GetEvents() const
{
    return m_engineEvents;
}
void Engine::AddEvent(EngineEvent *event)
{
    m_engineEvents.push_back(event);
}
