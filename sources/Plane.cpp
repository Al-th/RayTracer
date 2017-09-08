#include "Plane.hpp"

Plane::Plane(Material m, Vec3<double> point, Vec3<double> normal): Primitive(m,1){
    this->point = point;
    this->normal = normal;
    this->normal.normalize();
}

Plane::~Plane(){

}

void Plane::testCollision(Ray ray, Vec3<double>* hitPosition, Vec3<double>* hitNormal, bool& hit){
    double t = -(ray.origin - this->point).dotProduct(this->normal) / (ray.direction.dotProduct(this->normal));
    
    if(t < 0){
        hit = false;
        return;
    }

    else{
        Vec3<double> hitPos = ray.origin + t*ray.direction;
        hitPosition->x = hitPos.x;
        hitPosition->y = hitPos.y;
        hitPosition->z = hitPos.z;
    
        Vec3<double> hitNorm = this->normal;
        hitNormal->x = hitNorm.x;
        hitNormal->y = hitNorm.y;
        hitNormal->z = hitNorm.z;

        hit = true;
    }

    
}