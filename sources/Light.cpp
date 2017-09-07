#include "Light.hpp"


Light::Light(Vec3<double> position, double intensity){
    this->position = position;
    this->intensity = intensity;
}

Light::~Light(){
    
}