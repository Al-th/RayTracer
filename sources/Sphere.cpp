#include "Sphere.hpp"

Sphere::Sphere(Material m, Vec3<double> center, double radius) : Primitive(m,0){
    this->center = center;
    this->radius = radius;
    this->radiusSquared = radius*radius;
}

Sphere::~Sphere(){
    
}

void Sphere::testCollision(Ray ray, Vec3<double>* hitPosition, Vec3<double>* hitNormal, bool& hit){
    Vec3<double> OC = this->center - ray.origin;
    double OCLengthSquared = OC.lengthSq();
    
    double tca = OC.dotProduct(ray.direction);
    bool isOutOfSphere = OCLengthSquared > this->radiusSquared;
    
    if(tca < 0 && isOutOfSphere){
        return;
    }

    double thcSquared = this->radiusSquared - OCLengthSquared + tca*tca;

    if (thcSquared < 0){
        return;
    }

    double t = 0;
    if(isOutOfSphere){
        t = tca - sqrt(thcSquared);
    }
    else{
        t = tca + sqrt(thcSquared);
    }

    if(abs(t) < 1e-10){
        return;
    }

    hit = true;

    Vec3<double> hitPos = ray.origin + t*ray.direction;
    hitPosition->x = hitPos.x;
    hitPosition->y = hitPos.y;
    hitPosition->z = hitPos.z;

    Vec3<double> hitNorm = hitPos - this->center;
    hitNorm.normalize();
    hitNormal->x = hitNorm.x;
    hitNormal->y = hitNorm.y;
    hitNormal->z = hitNorm.z;
    

    return;
}