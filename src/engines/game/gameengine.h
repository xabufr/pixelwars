#ifndef GAMEENGINE_H
#define GAMEENGINE_H
#include "../engine.h"

class GameEngine : public Engine
{
    public:
        GameEngine();
        virtual ~GameEngine();
        void Start();
        EngineType GetEngineId() const;
    protected:
    private:
};

#endif // GAMEENGINE_H
