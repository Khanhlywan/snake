#include <SDL2/SDL.h>

class Snake
{
public:
    Snake();
    ~Snake();
    int exec():
private:
    SDL_Window *window;
    SDL_Renderer *renderer;
    SDL_Texture *sprites;
};
