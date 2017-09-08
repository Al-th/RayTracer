#include "Primitive.hpp"

Primitive::Primitive(Material primitiveMaterial, int type) : m(primitiveMaterial)
{
    this->type = type;
}

Primitive::~Primitive()
{
    //dtor
}


void Primitive::testCollision(Ray r, Vec3<double>* v1, Vec3<double>* v2, bool& hit){
    std::cout << "No test collision was implemented within this primitive" << std::endl;
}

int Primitive::getPrimitiveType(){
    return type;
}


//Reflection part
double Primitive::getReflectionRatio(){
    return m.getReflectionRatio();
}

//Refraction part
bool Primitive::isTransparent(){
    return m.isTransparent();
}

double Primitive::getRefractionRatio(){
    return m.getRefractionRatio();
}

double Primitive::getRefractionIndex(){
    return m.getRefractionIndex();
}