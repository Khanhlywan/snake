/*#include "textobject.h"
#include "BaseObject.h"

TextObject::TextObject()
{
    ;
}

TextObject::~TextObject()
{

}

void TextObject::SetColor(const int& type)
{
    if(type == RED_TEXT)
    {
        SDL_Color color = {225,0,0};
        text_color = color;
    }
    else if(type == WHILE_TEXT)
    {
        SDL_Color color = {255, 255, 255};
        text_color = color;
    }
    else
    {
       SDL_Color color = {0,0,0};
       text_color = color;
    }

}

void TextObject::CreateGameText(TTF_Font* font, SDL_Surface* des)
{
    p_object = TTF_RenderText_Solid(font, str_val.c_str(),text_color);//ham TTF_RenderText_Solid de tao ra 1 doi tuong suface tu mot ki tu cho vao, mau sac, va font chu cho phep
    Show(des);
}*/
