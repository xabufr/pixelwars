#ifndef BODYTYPE_H
#define BODYTYPE_H

enum BodyTypeEnum{
        Terrain,
        Projectile,
        Unite
        };

struct BodyType
{
    public:
        BodyType(BodyTypeEnum t=BodyTypeEnum::Terrain, void* ptr = 0);
        virtual ~BodyType();
        BodyTypeEnum type;
        void *proprietaire;

};

#endif // BODYTYPE_H
