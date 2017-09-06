#ifndef RAY_H
#define RAY_H

#include <Vec3.h>

class Ray{
    public:
        Ray(Vec3<double>,Vec3<double>);
        ~Ray();


        Vec3<double> origin;
        Vec3<double> direction;
    private:

};

#endif 