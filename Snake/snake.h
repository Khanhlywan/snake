#include <SDL2/SDL.h>
#include <iostream>
#include <sstream>
#include <vector>
#include <fstream>
#include <set>
#include <map>
#include <deque>
#include <windows.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <ctime>
#include <SDL2/SDL_mixer.h>

class Snake
{
public:
    Snake();
    ~Snake();
    int exec();
    bool tick();
    void draw();
    void showTextBackground(int x,int y,char *str,int color);
    void generateObstacles(int x);
    bool isOnSnake(int x, int y);
    bool isOnFruit(int x, int y);
    void setFixedObstacles();
    bool checkCollision(int x, int y);


    static const auto HeadOpenMouth = 0;
    static const auto Tail = 1;
    static const auto Turn = 2;
    static const auto Straight = 3;
    static const auto Head = 4;
    static const auto Fruit = 5;
    const static auto Width = 1260;
    const static auto Height = 600;
private:
  SDL_Window *window;
  SDL_Renderer *renderer;
  SDL_Texture *sprites;
  SDL_Texture *backgroundTexture;
  SDL_Texture *obstacleTexture; // Texture cho chướng ngại vật
  TTF_Font *font;

  std::deque<std::pair<int, int> > segmentsList;
  std::map<std::pair<int, int>, bool> obstacles;
  unsigned ticks = 0;
  int dx = 1;
  int dy = 0;
  int fruitX;
  int fruitY;
  int score = 0;//Diem so
  void generateFruit();
  int snakeLength;
};

class Menu {
public:
    void displayMenu(SDL_Renderer *renderer);
    void handleEvents(SDL_Event &event, bool &play, bool &quit);
    SDL_Rect playButtonRect;
    SDL_Rect exitButtonRect;
private:
    SDL_Texture *backgroundTexture;
    SDL_Texture *playButtonTexture;
    SDL_Texture *exitButtonTexture;
};

