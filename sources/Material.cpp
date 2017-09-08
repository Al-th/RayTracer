#include "Material.hpp"

Material::Material()
{
    r = 255;
    g = 255;
    b = 255;
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