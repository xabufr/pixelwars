#ifndef NETWORKENGINE_H
#define NETWORKENGINE_H
#include "../engine.h"

class NetworkEngine : public Engine
{
    public:
        NetworkEngine();
        virtual ~NetworkEngine();
        EngineType GetEngineId() const;
    protected:
    private:
};

#endif // NETWORKENGINE_H
