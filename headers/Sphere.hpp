#ifndef SPHERE_H
#define SPHERE_H

#include "Primitive.hpp"
#include "Material.hpp"
#include "Vec3.h"

class Sphere : public Primitive{
    public:
        Sphere(Material m, Vec3<double> center, double radius);
        ~Sphere();

        virtual void testCollision(Ray, Vec3<double>*, Vec3<double>*, bool&);

        Vec3<double> center;
        double radius;
        double radiusSquared; 
        int index;

    private:
};


#endif