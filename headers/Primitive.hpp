#ifndef PRIMITIVE_H
#define PRIMITIVE_H

#include "Material.hpp" 
#include "Ray.hpp"
#include <Vec3.h>

class Primitive
{
    public:
        Primitive(Material m);
        ~Primitive();

        virtual void testCollision(Ray, Vec3<double>*, Vec3<double>*, bool&);

        Material m;
    protected:
    private:
};

#endif // PRIMITIVE_H
