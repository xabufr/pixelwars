#include "random.h"
#include <cstdlib>

Random::Random()
{
    //ctor
}

Random::~Random()
{
    //dtor
}
int Random::Random(int min, int max)
{
    if(min==max)
        return min;
    return int(Random(float(min), float(max)));
}

float Random::Random(float min, float max)
{
    static bool inited=false;
    if(!inited)
    {
        srand(time(NULL));
        inited=true;
    }
    if(min==max)
        return min;
    return (rand()%(max-min+1)+min);
}
