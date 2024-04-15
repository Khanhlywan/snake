#include "snake.h"
#include <iostream>
#include <stdexcept>


int main(int argc, char **argv)
{
    /*SDL_Surface *screen;
    SDL_Surface *image;

    if (SDL_Init(SDL_INIT_EVERYTHING) == -1){
        return 1;
    }

    screen = SDL_SetVideoMode(1200, 600, 32, SDL_SWSURFACE);
    image = LoadImage("Garden-Background.png")

    SDL_BlitSurface(image, NULL, screen, NULL);

    SDL_Flip(screen);
    SDL_Delay(5000);
    SDL_FreeSurface(image);
    SDL_Quit;*/

    try
    {
        Snake s;
        return s.exec();
    } catch (std::exception &e)
    {
        std::cerr << e.what() << std::endl;
        return 1;
    }
}
