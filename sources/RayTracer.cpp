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



    createWorldObjects();
    createWorldLigths();

}


RayTracer::~RayTracer(){
    for(int i = 0; i < this->screenWidthInPixels; i++){
        delete [] image[i];
    }
    delete [] image;
}

void RayTracer::createWorldObjects(){
    std::cout << "Defining Scene" << std::endl;
    Material m;

    for(int i = 0; i < 5; i++){
        for(int j = 0; j < 5; j++){
            Vec3<double> center(i-2.5,(j-2.5)/3.0,j-2.5);
            double radius = 0.45;
            Sphere* s = new Sphere(m, center, radius);
            objectList.push_back(s);
        }
    }

    Vec3<double> center2(0,-5,0);
    double radius2 = 1;
    Sphere* s2 = new Sphere(m, center2, radius2);
    objectList.push_back(s2);
}

void RayTracer::createWorldLigths(){
    Vec3<double> lightPosition(0,-50,0);
    Light* l = new Light(lightPosition,1);
    lightList.push_back(l);
}

void RayTracer::animate(double t){
    Sphere* s = (Sphere*)objectList.back();
    double frequencySphereMotion = 0.3;
    s->center = Vec3<double>(3*cos(2*PI*frequencySphereMotion*t),-5,3*sin(2*PI*frequencySphereMotion*t));

}


void RayTracer::convertScreenPixelToPosition(int i, int j, double* x, double* y, double* z){
    if(i < screenWidthInPixels && j < screenHeightInPixels ){
        *x = leftOffsetInMeters + i * screenWidthMeterToPixelRatio;
        *y = topOffsetInMeters + j * screenHeightMeterToPixelRatio;
        *z = 4;
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


            Ray screenRay(rayOrigin, rayDirection);
            double rayIntensity = getRayIntensity(screenRay,0);
            image[i][j] = rayIntensity;

        }
    }
}

double RayTracer::getRayIntensity(Ray ray, int depth){
    if(depth > 2){
        return 0;
    }

    double dist = INFINITY;
    Vec3<double> finalHitPosition;
    Vec3<double> finalHitNormal;

    double rayIntensity = 0;



    bool hasAtLeastOneHit = false;
    //Detect closest collision;
    for(int i = 0; i < objectList.size(); i++){

        bool hit = false;
        Vec3<double> hitPosition;
        Vec3<double> hitNormal;

        Primitive* primitive = objectList[i];
        hit = false;
        primitive->testCollision(ray, &hitPosition, &hitNormal, hit);

        if(hit == true){
            hasAtLeastOneHit = true;
            double hitDistance = (hitPosition - ray.origin).length();
            if (hitDistance < dist){
                dist = hitDistance;
                finalHitPosition = hitPosition;
                finalHitNormal = hitNormal;
            }
        }
    }

    if(hasAtLeastOneHit){
        //Spawn shadow rays
        for(int i = 0; i < lightList.size(); i++){
            Light* light = lightList[i];
            Vec3<double> shadowRayOrigin(finalHitPosition - 1e-5*(finalHitPosition - ray.origin) );
            Vec3<double> shadowRayDirection(light->position - finalHitPosition);
            Ray shadowRay(shadowRayOrigin, shadowRayDirection);
            bool shadowRayBlocked = false;
            for(int i = 0; i < objectList.size(); i++){
                bool hit = false;
                Vec3<double> hitPosition;
                Vec3<double> hitNormal;
        
                Primitive* primitive = objectList[i];
                hit = false;
                primitive->testCollision(shadowRay, &hitPosition, &hitNormal, hit);
        
                if(hit == true){
                    shadowRayBlocked |= true;
                    break;
                }
            }
            if(!shadowRayBlocked){
                rayIntensity += (255*light->intensity) * abs(shadowRay.direction.dotProduct(finalHitNormal));
            }
        }

        //Spawn Perfect Reflection rays
        Vec3<double> reflectionRayOrigin = finalHitPosition;
        Vec3<double> reflectioNRayDirection = ray.direction - 2*(ray.direction.dotProduct(finalHitNormal))*finalHitNormal;
        Ray reflectionRay(reflectionRayOrigin, reflectioNRayDirection);
        rayIntensity += getRayIntensity(reflectionRay, depth+1);
        //Spawn Perfect Transmission ray
        //Spawn Diffuse Reflection Rays
        //Spawn Diffuse Transmission Rays

    }


    
    return rayIntensity;
}

void RayTracer::drawFrame(SDL_Surface* screen){
    SDL_LockSurface(screen);
    std::vector<uint8_t> pixels(screenHeightInPixels * screen->pitch,0); 
    int pitch = screen->pitch;
    for(int y = 0; y < screenHeightInPixels; y++){
        for(int x = 0; x < screenWidthInPixels; x ++){
            pixels[(x*3) + (y*pitch)+0] = image[x][y];
            pixels[(x*3) + (y*pitch)+1] = image[x][y];
            pixels[(x*3) + (y*pitch)+2] = image[x][y];
        }
    }
    memcpy(screen->pixels, pixels.data(), screen->pitch * screen->h);
    SDL_UnlockSurface(screen);
}