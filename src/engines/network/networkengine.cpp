#include "networkengine.h"

NetworkEngine::NetworkEngine()
{
    //ctor
}

NetworkEngine::~NetworkEngine()
{
    //dtor
}
EngineType NetworkEngine::GetEngineId() const
{
    return EngineType::Network_engine;
}
