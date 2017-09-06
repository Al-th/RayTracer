#include "Ray.hpp"

Ray::Ray(Vec3<double> rayOrigin , Vec3<double> rayDirection) : origin(rayOrigin), direction(rayDirection){
    direction.normalize();
}

Ray::~Ray(){

}