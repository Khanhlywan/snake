#include <SDL2/SDL.h>
#include <deque>
#include <windows.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

class Snake
{
public:
    Snake();
    ~Snake();
    int exec();
    bool tick();
    void draw();
    void drawMenu();//them ham ve menu
    void handleMenuEvent(const SDL_Event &event);//them ham su ly su kien cho menu

    static const auto HeadOpenMouth = 0;
    static const auto Tail = 1;
    static const auto Turn = 2;
    static const auto Straight = 3;
    static const auto Head = 4;
    static const auto Fruit = 5;
    const static auto Width = 1280;
    const static auto Height = 720;
private:
  SDL_Window *window;
  SDL_Renderer *renderer;
  SDL_Texture *sprites;
  SDL_Texture *backgroundTexture;
  SDL_Texture *playButtonTexture; // Texture của nút "Play"
  SDL_Texture *exitButtonTexture; // Texture của nút "Exit"
  SDL_Rect playButtonRect; // Vị trí và kích thước của nút "Play"
  SDL_Rect exitButtonRect; // Vị trí và kích thước của nút "Exit"
  TTF_Font *font; // Font cho văn bản
  std::deque<std::pair<int, int> > segmentsList;
  unsigned ticks = 0;
  int dx = 1;
  int dy = 0;
  int fruitX;
  int fruitY;
  void generateFruit();
};
