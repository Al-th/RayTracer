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
    m.setReflectionRatio(0);
    m.setTransparentProperty(1.5,1);
    
    Material m2;
    m2.setReflectionRatio(0);
    m2.setTransparentProperty(0,0);


    Vec3<double> center2(0,-2,0);
    double radius2 = 1;
    Sphere* s2 = new Sphere(m, center2, radius2);
    objectList.push_back(s2);

    Vec3<double> c3(0,-1,0);
    double r3 = 1.0;
    Sphere* s3 = new Sphere(m2, c3, r3);
    objectList.push_back(s3);



    for(int i = 0; i < 10; i+=2){
        for(int j = 0; j < 10; j+=2){
            Vec3<double> center(i-4,(j)/3.0,j-5);
            double radius = 0.8;
            Sphere* s = new Sphere(m2, center, radius);
            objectList.push_back(s);
        }
    }


    
    //LeftPlane
    Vec3<double> lplanePoint(-10,0,0);
    Vec3<double> lplaneNormal(1,0,0);
    Plane* lPlane = new Plane(m2, lplanePoint, lplaneNormal);
    objectList.push_back(lPlane);
    //RightPlane
    Vec3<double> rplanePoint(10,0,0);
    Vec3<double> rplaneNormal(-1,0,0);
    Plane* rPlane = new Plane(m2, rplanePoint, rplaneNormal);
    objectList.push_back(rPlane);
    //TopPlane
    Vec3<double> tplanePoint(0,-15,0);
    Vec3<double> tplaneNormal(0,-1,0);
    Plane* tPlane = new Plane(m2, tplanePoint, tplaneNormal);
    objectList.push_back(tPlane);
    //BottomPlane
    Vec3<double> bplanePoint(0,5,0);
    Vec3<double> bplaneNormal(0,1,0);
    Plane* bPlane = new Plane(m2, bplanePoint, bplaneNormal);
    objectList.push_back(bPlane);
    //BackPlane
    Vec3<double> bbplanePoint(0,0,-10);
    Vec3<double> bbplaneNormal(0,0,1);
    Plane* backPlane = new Plane(m2, bbplanePoint, bbplaneNormal);
    objectList.push_back(backPlane);
    
}

void RayTracer::createWorldLigths(){
    Vec3<double> lightPosition(0,-10,0);
    Light* l = new Light(lightPosition,1);
    lightList.push_back(l);
}

void RayTracer::animate(double t){
    Sphere* s = (Sphere*)objectList.front();
    double frequencySphereMotion = 0.03;
    s->center = Vec3<double>(3*cos(2*PI*frequencySphereMotion*t),-1.2,3*sin(2*PI*frequencySphereMotion*t));
}


void RayTracer::convertScreenPixelToPosition(int i, int j, double* x, double* y, double* z){
    if(i < screenWidthInPixels && j < screenHeightInPixels ){
        *x = leftOffsetInMeters + i * screenWidthMeterToPixelRatio;
        *y = topOffsetInMeters + j * screenHeightMeterToPixelRatio;
        *z = 8;
    }
}

void RayTracer::computeFrame(){
    #pragma omp parallel for collapse(2)
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
    Primitive* finalHitPrimitive;

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
                finalHitPrimitive = primitive;
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
            double distanceToLightSquared = (light->position - shadowRay.origin).lengthSq();
            bool shadowRayBlocked = false;
            for(int i = 0; i < objectList.size(); i++){
                bool hit = false;
                Vec3<double> hitPosition;
                Vec3<double> hitNormal;
        
                Primitive* primitive = objectList[i];
                hit = false;
                primitive->testCollision(shadowRay, &hitPosition, &hitNormal, hit);
                double hitDistanceSquared =(hitPosition - shadowRay.origin).lengthSq();
                if(hit == true && hitDistanceSquared < distanceToLightSquared){
                    shadowRayBlocked |= true;
                    break;
                }
            }
            if(!shadowRayBlocked){
                rayIntensity += (255*light->intensity) * abs(shadowRay.direction.dotProduct(finalHitNormal));
            }
        }

        //Spawn Perfect Reflection rays
        if(finalHitPrimitive->getReflectionRatio() > 0){
            Vec3<double> reflectionRayOrigin = finalHitPosition - 1e-5*(finalHitPosition - ray.origin) ;
            Vec3<double> reflectioNRayDirection = ray.direction - 2*(ray.direction.dotProduct(finalHitNormal))*finalHitNormal;
            Ray reflectionRay(reflectionRayOrigin, reflectioNRayDirection);
            rayIntensity += finalHitPrimitive->getReflectionRatio() * getRayIntensity(reflectionRay, depth+1);
        }
        //Spawn Perfect Transmission ray
        if(finalHitPrimitive->isTransparent()){
            Vec3<double> refractionRayOrigin = finalHitPosition + 1e-5*(finalHitPosition - ray.origin);
            
            double eta = 1.0/finalHitPrimitive->getRefractionIndex();
            double c1 = finalHitNormal.dotProduct(ray.direction);
            double c2 = eta*eta*(1-(c1*c1));
            if(c2<=1){
                double c3 = eta*c1-sqrt(1 - c2);
                Vec3<double> refractionRayDirection = eta * ray.direction +  c3 * finalHitNormal;
                Ray refractionRay(refractionRayOrigin, refractionRayDirection);
                rayIntensity += finalHitPrimitive->getRefractionRatio() * getRayIntensity(refractionRay, depth+1);
           }
        }
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