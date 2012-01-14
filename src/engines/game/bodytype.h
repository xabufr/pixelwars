#ifndef BODYTYPE_H
#define BODYTYPE_H

enum BodyTypeEnum{
        TerrainE=0,
        ProjectileE=1,
        UniteE=2
        };

struct BodyType
{
    public:
        BodyType(BodyTypeEnum t=BodyTypeEnum::TerrainE, void* ptr = 0);
        virtual ~BodyType();
        BodyTypeEnum type;
        void *proprietaire;

};

#endif // BODYTYPE_H
