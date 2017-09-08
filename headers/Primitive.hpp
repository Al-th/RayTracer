#ifndef PRIMITIVE_H
#define PRIMITIVE_H

#include "Material.hpp" 
#include "Ray.hpp"
#include <Vec3.h>

class Primitive
{
    public:
        Primitive(Material, int);
        ~Primitive();

        virtual void testCollision(Ray, Vec3<double>*, Vec3<double>*, bool&);
        int getPrimitiveType();
        double getReflectionRatio();

        double getRefractionRatio();
        double getRefractionIndex();
        bool isTransparent();

        Material m;
    protected:
    private:
        int type;
};

#endif // PRIMITIVE_H
