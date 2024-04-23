#include <SDL2/SDL.h>
#include <iostream>
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
  TTF_Font *font;

  std::deque<std::pair<int, int> > segmentsList;
  unsigned ticks = 0;
  int dx = 1;
  int dy = 0;
  int fruitX;
  int fruitY;
  int score = 0;//Diem so
  void generateFruit();
  int snakeLength;
};

struct HighScore {
    int score;
    char name[30];
};
struct Point {
    int x,y,x0,y0;
};
//quan li nut bam
class Button {
public:
  Button(int x, int y, int w, int h, std::string text);
  void draw(SDL_Renderer *renderer);
  bool isClicked(int mouseX, int mouseY);

private:
  SDL_Texture *texture;
  SDL_Rect rect;
  TTF_Font *font;
  std::string text;
};

class Menu {
public:
  Menu();
  void draw(SDL_Renderer* renderer);
  bool handleEvents(SDL_Event* event);

private:
  Button playButton;
  Button exitButton;
  SDL_Texture *backgroundTexture;
};

