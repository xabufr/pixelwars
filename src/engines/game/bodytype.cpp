#include "bodytype.h"

BodyType::BodyType(BodyTypeEnum t, void* ptr): type(t), proprietaire(ptr)
{
    //ctor
}

BodyType::~BodyType()
{
    //dtor
}
