#ifndef Plane_H
#define Plane_H

#include <Vec3.h>
#include "Primitive.hpp"
#include "Ray.hpp"

using namespace std;

class Plane : public Primitive{
    public:
        Plane(Material m, Vec3<double>, Vec3<double>);
        ~Plane();


        virtual void testCollision(Ray, Vec3<double>*, Vec3<double>*, bool&);

    private:
        Vec3<double> point;
        Vec3<double> normal;
};

#endif