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
  std::vector<std::pair<int, int>> obstacles;
  unsigned ticks = 0;
  int dx = 1;
  int dy = 0;
  int fruitX;
  int fruitY;
  void generateFruit();
  int snakeLength;

};

