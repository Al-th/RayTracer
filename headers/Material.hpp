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
        void setTransparentProperty(double refractionIndex, double refractionRatio);
        double getRefractionIndex();
        double getRefractionRatio();
        bool isTransparent();
        //Diffuse Transmission
    protected:
    private:
        double reflectionRatio;

        double refractionRatio;
        double refractionIndex;
};

#endif // MATERIAL_H
