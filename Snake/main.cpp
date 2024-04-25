#include "snake.h"
#include "textobject.h"
#include <iostream>
#include <stdexcept>
#include "graphics.h"

SDL_Renderer *renderer;


int main(int argc, char **argv) {
    ultis graphic;
    graphic.SDLInit();
    SDL_Texture* menu = graphic.loadtexture("menu.png");
    SDL_RenderCopy(graphic.renderer,menu,NULL,NULL);


    SDL_Texture* play = graphic.loadtexture("play.png");
    graphic.renderTexture(play,530,300,1,1);
    SDL_Texture* exit = graphic.loadtexture("exit.png");
    graphic.renderTexture(exit,530,400,1,1);
    SDL_RenderPresent(graphic.renderer);

    SDL_Event e;
    bool m_isRunning=true;
    while (m_isRunning)
    {
      // Uint32 windowID = SDL_GetWindowID(graphics.window);
        while( SDL_PollEvent(&e) != 0)
        {
            if(e.type == SDL_QUIT)m_isRunning=false;
            int x,y;
            if(e.type==SDL_MOUSEBUTTONDOWN)
            {
                if(e.button.button==SDL_BUTTON_LEFT)
                {
                    SDL_GetMouseState(&x,&y);
                    if(graphic.inside(x,y,graphic.toado(play,530,300,1,1)))
                    {
                        graphic.close();
                         try {
                            Snake s;
                            return s.exec();
                         } catch (std::exception &e) {
                           std::cerr << e.what() << std::endl;
                            return 1;
                         }
                    }
                    if(graphic.inside(x,y,graphic.toado(exit,530,400,1,1)))
                    {
                        m_isRunning=false;

                    }
                }
            }
        }
    }
        graphic.close();



}
