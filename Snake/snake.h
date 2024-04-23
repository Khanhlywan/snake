#include <SDL2/SDL.h>
#include <iostream>
#include <sstream>
#include <vector>
#include <set>
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
    void classic();//Di xuyen tuong
    bool isEmpty();//luu diem cao
    bool checkPoint();
    void showHighScore();//doc thong tin file diem cao va xuat ra thong tin
    void getHighScore();//ghi lai thong tin diem cao + luu
    bool checkHighScore(int score);//nhan diem cao cua nguoi choi, lay thong tin nguoi choi neu nguoi choi dat thanh tich cao
    void showText(int x,int y,char *str);
    void showTextBackground(int x,int y,char *str,int color);
    void generateObstacles(int x);
    bool isOnSnake(int x, int y);
    bool isOnFruit(int x, int y);
    void setFixedObstacles();


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
  int score = 0;//Diem so
  void generateFruit();
  int snakeLength;
};
//chuong ngai vat
//std::vector<std::pair<int, int>> obstacles;

struct HighScore {
    int score;
    char name[30];
};
struct Point {
    int x,y,x0,y0;
};

