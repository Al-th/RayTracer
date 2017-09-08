#include "Material.hpp"

Material::Material()
{
    r = 255;
    g = 255;
    b = 255;


    this->setReflectionRatio(0);
    this->setTransparentProperty(0,0);
}

Material::~Material()
{
    //dtor
}


void Material::setReflectionRatio(double reflectionRatio){
    this->reflectionRatio = std::max(std::min(reflectionRatio,1.0),0.0);
}

double Material::getReflectionRatio(){
    return reflectionRatio;
}

double Material::getRefractionIndex(){
    return refractionIndex;
}

bool Material::isTransparent(){
    return (refractionRatio > 0);
}

double Material::getRefractionRatio(){
    return refractionRatio;
}

void Material::setTransparentProperty(double refractionIndex, double refractionRatio){
        this->refractionRatio = refractionRatio;
        this->refractionIndex = refractionIndex;
}