#ifndef MATERIAL_H
#define MATERIAL_H

#include <algorithm> 

class Material
{
    public:
        Material();
        ~Material();

        //Material properties
        short r,g,b;
        //Color
        //Specular Reflection
        double getReflectionRatio();
        void setReflectionRatio(double);
        //Diffuse Reflection
        //Specular Transmission
        //Diffuse Transmission
    protected:
    private:
        double reflectionRatio;
};

#endif // MATERIAL_H
