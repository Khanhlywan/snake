#include "snake.h"
#include "textobject.h"
#include <iostream>
#include <stdexcept>

enum GameState { MENU, PLAYING };

SDL_Renderer *renderer;
Menu menu;
GameState gameState = MENU;

int main(int argc, char **argv)
{
    /*try
    {
        Menu m;
        Snake s;
        return s.exec();
    } catch (std::exception &e)
    {
        std::cerr << e.what() << std::endl;
        return 1;
    }
    if (!renderer) {
     std::cerr << "Failed to create renderer" << std::endl;
     return 1;
    }*/
}
