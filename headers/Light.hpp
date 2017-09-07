#ifndef LIGHT_H
#define LIGHT_H

#include <Vec3.h> 
using namespace std;

class Light{
    public:
        Light(Vec3<double>, double);
        ~Light();

        Vec3<double> position;
        double intensity;
};

#endif