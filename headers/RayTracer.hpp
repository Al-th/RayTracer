#ifndef RAYTRACER_H
#define RAYTRACER_H

#include <vector>
#include <Vec3.h>
#include <SDL/SDL.h>
#include "Primitive.hpp"
#include "Sphere.hpp"
#include "Material.hpp"
#include "Ray.hpp"

using namespace std;

class RayTracer{
    public:
        RayTracer(SDL_Surface*);
        ~RayTracer();

        void computeFrame();
        void drawFrame(SDL_Surface*);
    private:
        //Pixel definition
        int screenWidthInPixels;
        int screenHeightInPixels;
        SDL_Surface* screen;
        double** image;
        
        //Real word definition
        double screenWidthInMeters;
        double screenHeightInMeters;
        double topOffsetInMeters;
        double leftOffsetInMeters;
        std::vector<Primitive*> objectList;

        
        //Translation between screen and real world
        double screenWidthMeterToPixelRatio;
        double screenHeightMeterToPixelRatio;

        void convertScreenPixelToPosition(int , int , double*, double*, double*);
        double getRayIntensity(Ray);
};

#endif