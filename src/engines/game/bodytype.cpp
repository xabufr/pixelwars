#include "bodytype.h"

BodyType::BodyType(BodyTypeEnum t, void* ptr): proprietaire(ptr)
{
    type=t;
}

BodyType::~BodyType()
{
    //dtor
}
