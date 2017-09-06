#include "Primitive.hpp"

Primitive::Primitive(Material primitiveMaterial) : m(primitiveMaterial)
{
}

Primitive::~Primitive()
{
    //dtor
}


void Primitive::testCollision(Ray r, Vec3<double>* v1, Vec3<double>* v2, bool& hit){
    std::cout << "No test collision was implemented within this primitive" << std::endl;
}
