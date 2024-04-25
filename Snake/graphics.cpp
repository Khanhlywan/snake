#include"graphics.h"

void ultis::SDLInit()
{
   SDL_Init(SDL_INIT_EVERYTHING);
   window =SDL_CreateWindow("snake",SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED,1260,600,SDL_WINDOW_SHOWN);

   renderer=SDL_CreateRenderer(window,-1,SDL_RENDERER_ACCELERATED|SDL_RENDERER_PRESENTVSYNC);

   SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");
    SDL_RenderSetLogicalSize(renderer, 1260, 600);

}
void ultis::close()
{
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    SDL_Quit();
}
void ultis::waitkeypressed()
{
    SDL_Event e;
    while (true)
        {
           if ( SDL_WaitEvent(&e) != 0 &&(e.type == SDL_KEYDOWN || e.type == SDL_QUIT) )return;
           SDL_Delay(100);
        }
 }
SDL_Texture* ultis::loadtexture(string path)
{
    SDL_Texture* newtexture=nullptr;
    newtexture =IMG_LoadTexture(renderer,path.c_str());
        if(newtexture==nullptr)
            std::cout << "Unable to load image " << path << "! SDL_image Error: " << IMG_GetError() << std::endl;

    return newtexture;
}
bool ultis::inside(int &x,int &y,SDL_Rect rect)
{
    bool Inside=true;

        // Chuột nằm bên trái nút
        if( x < rect.x )
        {
            Inside = false;
        }
        //Chuột ở bên phải nút
        else if( x > rect.x + rect.w)
        {
            Inside = false;
        }
        //Chuột lên trên nút
        else if( y < rect.y )
        {
            Inside = false;
        }
        //Chuột bên dưới nút
        else if( y > rect.y + rect.h )
        {
            Inside = false;
        }
    return Inside;
}
void ultis::free(SDL_Texture* &texture)
{
    SDL_DestroyTexture(texture);
    texture=nullptr;
}
void ultis::blitRect(SDL_Texture *texture, SDL_Rect *src, int x, int y)
    {
        SDL_Rect dest;

        dest.x = x;
        dest.y = y;
        dest.w = 900;
        dest.h = 200;

        SDL_RenderCopy(renderer, texture, src, &dest);
    }
void ultis::renderTexture(SDL_Texture *texture, int x, int y,int w,int h)
    {
        SDL_Rect dest;

        dest.x = x;
        dest.y = y;
        SDL_QueryTexture(texture, NULL, NULL, &dest.w, &dest.h);
        dest.w/=w;
        dest.h/=h;
        SDL_RenderCopy(renderer, texture, NULL, &dest);
    }
SDL_Rect ultis::toado(SDL_Texture *texture, int x, int y,int w,int h)
{

        SDL_Rect dest;

        dest.x = x;
        dest.y = y;
        SDL_QueryTexture(texture, NULL, NULL, &dest.w, &dest.h);
        dest.w/=w;
        dest.h/=h;

        return dest;
}



