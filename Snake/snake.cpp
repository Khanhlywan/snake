#include "snake.h"
#include "sdl_check.h"

Snake::Snake()
 {
     auto res = SDL_Init(SDL_INIT_EVERYTHING);
     SDL_CHECK(res == 0, "SDL_Init");
     SDL_CreateWindowAndRenderer(Width, Height, SDL_WINDOW_BORDELESS, &window, &renderer);
     SDL_CHECK(window, "SDL_CreateWindowAndRenderer");
     SDL_CHECK(renderer, "SDL_CreateWindowAndRenderer");
 }
Snake::~Snake()
{
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}
int Snake::exec()
{
    for(auto done = false; !done;)
    {
        SDL_Event e;
        if((SDL_PollEvent(&e)))
        {
            switch (e.type)
            {
            case SDL_QUIT:
                done = true;
                break;
            }
        }
    }
    return 0
}
