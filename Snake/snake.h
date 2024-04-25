#include <SDL2/SDL.h>
#include <iostream>
#include <sstream>
#include <vector>
#include <fstream>
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
    void init();
    bool tick();
    void draw();
    void displayPlayAgainScreen();
    void resetGame();
    void exitGame();
    void onFruitEaten();

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
  SDL_Texture *playAgainTexture;
  SDL_Texture *menuTexture;
  Uint32 lastMoveTime;

  std::deque<std::pair<int, int> > segmentsList;
  std::vector<std::pair<int, int>> obstacles;
  unsigned ticks = 0;
  int dx = 1;
  int dy = 0;
  int fruitX;
  int fruitY;
  void generateFruit();
  int snakeLength;
  float initialSpeed = 0.1f; // Tốc độ ban đầu của con rắn (tính bằng giây)
  float currentSpeed = initialSpeed; // Tốc độ hiện tại của con rắn


   Mix_Chunk *eatSound;
   Mix_Chunk *deathSound;

};

