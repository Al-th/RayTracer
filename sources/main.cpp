#include <cstdlib>
#include <SDL/SDL.h>
#include <stdlib.h>
#include <time.h>

//#include "SDL_draw.h"
#include <cmath>
#include <ctime>
#include <iostream>
#include <cstdio>

#include "RayTracer.hpp" 


using namespace std;


int initSDL(){
    // initialize SDL video
    if ( SDL_Init( SDL_INIT_VIDEO ) < 0 )
    {
        printf( "Unable to init SDL: %s\n", SDL_GetError() );
        return 0;
    }

    // make sure SDL cleans up before exit
    atexit(SDL_Quit);

    return 1;
}

int processEvents(int* mouseX, int* mouseY){
    int result = 0;
    // message processing loop
    SDL_Event event;
    while (SDL_PollEvent(&event))
    {
        // check for messages
        switch (event.type)
        {
            // exit if the window is closed
        case SDL_QUIT:
            std::cout << "EVENT_QUIT" << std::endl;
            result = 1;
            break;

            // check for keypresses
        case SDL_KEYDOWN:
                // exit if ESCAPE is pressed
                if (event.key.keysym.sym == SDLK_ESCAPE){
                    result = 1;
                }
                break;
        }
    }

    return result;
}


int main ( int argc, char** argv )
{
    /**********************/

    srand(time(NULL));
    clock_t startingClock;

    startingClock = clock();

    bool done = false;
    int mouseX = 0;
    int mouseY = 0;

    if(!initSDL()){
        return 1;
    }

    // create a new window
    SDL_Surface* screen = SDL_SetVideoMode(250, 250, 24, SDL_HWSURFACE|SDL_DOUBLEBUF);
    if ( !screen )
    {
        std::cout << "Unable to set 500x500 video: " << SDL_GetError() << std::endl;
        return 0;
    }
    else{
        std::cout << "Opened SDL screen sucessfully" << std::endl;
    }



    RayTracer* rt = new RayTracer(screen);
    int iteration = 0;
    while (!done)
    {
        int event = processEvents(&mouseX, &mouseY);
        switch(event){
            case 1:
                done = true;
                break;
            default:
                break;
        }

        double t = (clock() - startingClock)/(double)CLOCKS_PER_SEC;
        rt->animate(t);

        // clear screen
        SDL_FillRect(screen, 0, SDL_MapRGB(screen->format, 0, 0, 0));

        rt->computeFrame();
        rt->drawFrame(screen);

        SDL_Flip(screen);

        
        // Activate to save a bitmap of the output result
        /*string s = "./output/test" + to_string(iteration) + ".bmp";
        SDL_SaveBMP(screen, s.c_str());
        iteration++;*/
        
    


    } // end main loop
    delete rt;

    printf("Exited cleanly\n");
    return 0;
}
