#include "RayTracer.hpp"

RayTracer::RayTracer(SDL_Surface* SDLscreen) : screen(SDLscreen){
    this->screenWidthInPixels = screen->w;
    this->screenHeightInPixels = screen->h;
    this->image = new double*[this->screenWidthInPixels];
    for(int i = 0; i < this->screenWidthInPixels; i++){
        this->image[i] = new double[this->screenHeightInPixels];
    }

    this->screenWidthInMeters = 5.0;
    this->screenHeightInMeters = 5.0;

    this->topOffsetInMeters = - screenHeightInMeters/2.0;
    this->leftOffsetInMeters = - screenHeightInMeters/2.0;

    this->screenWidthMeterToPixelRatio = screenWidthInMeters/screenWidthInPixels;
    this->screenHeightMeterToPixelRatio = screenHeightInMeters/screenHeightInPixels;


    //Temporary world object declaration
    Material m;
    Vec3<double> center(0.1,0,0);
    double radius = 1.0;
    std::cout << "Defining new sphere" << std::endl;
    Sphere* s = new Sphere(m, center, radius);
    std::cout << "Sphere created" << std::endl;
    objectList.push_back(s);

}


RayTracer::~RayTracer(){
}


void RayTracer::convertScreenPixelToPosition(int i, int j, double* x, double* y, double* z){
    if(i < screenWidthInPixels && j < screenHeightInPixels ){
        *x = leftOffsetInMeters + i * screenWidthMeterToPixelRatio;
        *y = topOffsetInMeters + j * screenHeightMeterToPixelRatio;
        *z = 10;
    }
}

void RayTracer::computeFrame(){
    for(int i = 0; i < screenWidthInPixels; i++){
        for(int j = 0; j < screenHeightInPixels; j++){
            double x = 0;
            double y = 0;
            double z = 0;
            convertScreenPixelToPosition(i,j,&x,&y,&z);
            Vec3<double> rayOrigin(x,y,z);
            double offset = this->screenWidthInMeters / tan(60*DEG_TO_RAD_FACTOR);
            Vec3<double> rayDirection = rayOrigin - Vec3<double>(0,0,rayOrigin.z + offset);

            Vec3<double> k1(1,0,2);

            Vec3<double> k2 = k1;


            Ray* screenRay = new Ray(rayOrigin, rayDirection);
            double rayIntensity = getRayIntensity(*screenRay);
            image[i][j] = rayIntensity;
        }
    }
}

double RayTracer::getRayIntensity(Ray ray){
    double dist = INFINITY;
    Vec3<double>* finalHitPosition = NULL;
    Vec3<double>* finalHitNormal = NULL;

    double rayIntensity = 0;


    Vec3<double>* hitPosition = new Vec3<double>(0,0,0);
    Vec3<double>* hitNormal = new Vec3<double>(0,0,0);
    bool hit = false;
    //Detect closest collision;
    for(int i = 0; i < objectList.size(); i++){
        Primitive* primitive = objectList[i];
        hit = false;
        primitive->testCollision(ray, hitPosition, hitNormal, hit);

        if(hit == true){
            double hitDistance = (*hitPosition - ray.origin).length();
            if (hitDistance < dist){
                dist = hitDistance;

                finalHitPosition = hitPosition;
                finalHitNormal = hitNormal;
            }
        }
    }

    if(finalHitPosition != NULL){
        rayIntensity = 255.0;
    }

    //Spawn shadow rays
    //Spawn Perfect Reflection rays
    //Spawn Perfect Transmission ray
    //Spawn Diffuse Reflection Rays
    //Spawn Diffuse Transmission Rays

    return rayIntensity;
}

void RayTracer::drawFrame(SDL_Surface* screen){
    for(int i = 0; i < screenWidthInPixels; i++){
        for(int j = 0; j < screenHeightInPixels; j++){
            /* Get a pointer to the video surface's pixels in memory. */
            Uint32 *pixels = (Uint32*) screen->pixels;

            /* Calculate offset to the location we wish to write to */
            int offset = screen->w * i + j;

            /* Compose RGBA values into correct format for video surface and copy to screen */
            *(pixels + offset) = SDL_MapRGBA(screen->format, image[i][j], image[i][j], image[i][j], 255);    
        }
    }
}