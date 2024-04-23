#include "snake.h"
#include "sdl_check.h"


Button::Button(int x, int y, int w, int h, std::string text) {
  rect.x = x;
  rect.y = y;
  rect.w = w;
  rect.h = h;
  this->text = text;

  font = TTF_OpenFont("font.ttf", 24);
  if (!font) {
    std::cerr << "Failed to load font" << std::endl;
    return;
  }

  SDL_Surface *textSurface = TTF_RenderText_Solid(font, text.c_str(), {255, 255, 255});
  if (!textSurface) {
    std::cerr << "Failed to render text" << std::endl;
    return;
  }

  texture = SDL_CreateTextureFromSurface(renderer, textSurface);
  SDL_FreeSurface(textSurface);
}
void Button::draw(SDL_Renderer *renderer) {
  SDL_RenderFillRect(renderer, &rect, {0, 0, 0});
  SDL_RenderCopy(renderer, texture, NULL, &rect);
}
bool Button::isClicked(int mouseX, int mouseY) {
  return mouseX >= rect.x && mouseX <= rect.x + rect.w &&
         mouseY >= rect.y && mouseY <= rect.y + rect.h;
}

Menu::Menu() {
  playButton = Button(Width / 4 - 50, Height / 2 - 30, 100, 60, "Play");
  exitButton = Button(Width / 4 * 3 - 50, Height / 2 - 30, 100, 60, "Exit");

  SDL_Surface *backgroundSurface = IMG_Load("menu.png");
  if (!backgroundSurface) {
    std::cerr << "Failed to load background image" << std::endl;
    return;
  }
  backgroundTexture = SDL_CreateTextureFromSurface(renderer, backgroundSurface);
  SDL_FreeSurface(backgroundSurface);
}
void Menu::draw(SDL_Renderer *renderer) {
  SDL_RenderCopy(renderer, backgroundTexture, NULL, NULL);
  playButton.draw(renderer);
  exitButton.draw(renderer);
}
bool Menu::handleEvents(SDL_Event *event) {
  switch (event->type) {
    case SDL_MOUSEMOTION:
      playButton.onMouseMove(event->motion.x, event->motion.y);
      exitButton.onMouseMove(event->motion.x, event->motion.y);
      break;
    case SDL_MOUSEBUTTONDOWN:
      if (event->button.button == SDL_BUTTON_LEFT) {
        int mouseX = event->button.x;
        int mouseY = event->button.y;
        if (playButton.isClicked(mouseX, mouseY)) {
          // Start the game! (Replace with your game logic)
          return true; // Indicate game start
        } else if (exitButton.isClicked(mouseX, mouseY)) {
          // Exit the game! (Replace with exit logic)
          return false; // Indicate game exit
        }
      }
      break;
  }
  return false; // Continue showing the menu
}


Snake::Snake()
 {
     auto res = SDL_Init(SDL_INIT_EVERYTHING);
     SDL_CHECK(res == 0, "SDL_Init");
     SDL_CreateWindowAndRenderer(Width, Height, SDL_WINDOW_BORDERLESS, &window, &renderer);
     SDL_CHECK(window, "SDL_CreateWindowAndRenderer");
     SDL_CHECK(renderer, "SDL_CreateWindowAndRenderer");
     //SDL_Surface* LoadImage(std::string "background.png")

     SDL_SetWindowPosition(window, 65, 65);

    // std::srand(static_cast<unsigned>(std::time(nullptr)));//khoi tao random seed

     /*font = TTF_OpenFont("font.ttf", 24); // Thay "font.ttf" bằng đường dẫn đến font bạn muốn sử dụng
     if (!font)
     {
      // Xử lý lỗi nếu không thể mở font
      std::cerr << "Khong the mo tep font!" <<TTF_GetError() << std::endl;
      return; // Hoặc thực hiện các hành động xử lý lỗi khác, ví dụ như thoát khỏi chương trình
     }*/


     auto backgroundSurface = IMG_Load("background.png");
     SDL_CHECK(backgroundSurface, "Failed to load background image");
     backgroundTexture = SDL_CreateTextureFromSurface(renderer, backgroundSurface);
     SDL_CHECK(backgroundTexture, "Failed to create background texture");
     SDL_FreeSurface(backgroundSurface);
     SDL_RenderCopy(renderer, backgroundTexture, NULL, NULL);

     auto surface = SDL_LoadBMP("sprites.bmp");
     //auto surface = SDL_LoadBMP("Garden-Background.png");
     SDL_CHECK(surface, "SDL_LoadBMP(\"sprites.bmp\")");
     sprites = SDL_CreateTextureFromSurface(renderer, surface);
     SDL_CHECK(sprites, "SDL_CreateTextureFromSurface");
     SDL_FreeSurface(surface);

     segmentsList.push_back(std::make_pair(5, 5));
     segmentsList.push_back(std::make_pair(5, 6));
     segmentsList.push_back(std::make_pair(4, 6));
     generateFruit();


}

void Snake::generateFruit()
{
  auto done = false;
  do
  {
    fruitX = rand() % (Width / 64);
    fruitY = rand() % (Height / 64);
    done = true;
    for (const auto &segment: segmentsList)
    {
      if (fruitX == segment.first && fruitY == segment.second)
      {
        done = false;
        break;
      }
    }
  } while (!done);
}
Snake::~Snake()
{
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

int Snake::exec()
{
     auto oldTick = SDL_GetTicks();
        //auto oldTick = SDL_GetTicks();
         for (auto done = false; !done;)
          {
            SDL_Event e;
            if (SDL_PollEvent(&e))
          {
             switch (e.type)
              {
                case SDL_KEYDOWN:
               {
                 switch (e.key.keysym.sym)
                 {
                   case SDLK_UP:
                    dx = 0;
                    dy = -1;
                    break;
                   case SDLK_RIGHT:
                    dx = 1;
                    dy = 0;
                    break;
                   case SDLK_DOWN:
                    dx = 0;
                    dy = 1;
                    break;
                   case SDLK_LEFT:
                    dx = -1;
                    dy = 0;
                    break;
                  }
                 }
                 break;
          case SDL_QUIT:
            done = true;
            break;
         }
       }
        //SDL_SetRenderDrawColor(renderer, 0x00, 0x7f, 0x00, 0xff);
       SDL_RenderClear(renderer);

       auto currentTick = SDL_GetTicks();
       for (auto t = oldTick; t < currentTick; ++t)
       if (!tick())
        return 1;
       oldTick = currentTick;
       draw();
      SDL_RenderPresent(renderer);
  }
    return 0;
}
bool Snake::tick()
{
  if (ticks++ % 250 == 0)
  {
    auto p = segmentsList.front();
    p.first += dx;
    p.second += dy;

    if (p.first < 0) {
      p.first = Width / 64 - 1;
    } else if (p.first >= Width / 64) {
      p.first = 0;
    }
    if (p.second < 0) {
      p.second = Height / 64 - 1;
    } else if (p.second >= Height / 64) {
      p.second = 0;
    }
      //return false;
    for (const auto &segment: segmentsList)
      if (p == segment)
        return false;
    segmentsList.push_front(p);
    if (p.first != fruitX || p.second != fruitY)
      segmentsList.pop_back();
    else
      generateFruit();
  }
   /*if (segmentsList.front().first < 0)
        segmentsList.front().first = Width / 64 - 1;
    else if (segmentsList.front().first >= Width / 64)
        segmentsList.front().first = 0;
    if (segmentsList.front().second < 0)
        segmentsList.front().second = Height / 64 - 1;
    else if (segmentsList.front().second >= Height / 64)
        segmentsList.front().second = 0;*/

  return true;
}

void Snake::draw()
{
  SDL_RenderCopy(renderer, backgroundTexture, NULL, NULL);

  SDL_Rect src;
  src.x = 0;
  src.y = 0;
  src.w = 64;
  src.h = 64;
  SDL_Rect dest;
  dest.w = 64;
  dest.h = 64;
  int ds[][3] = {
    { -1, 0, 0 },
    { 0, -1, 90 },
    { 1, 0, 180 },
    { 0, 1, -90 },
  };
  for (auto iter = std::begin(segmentsList); iter != std::end(segmentsList); ++iter)
  {
    float direction = 0;
    const auto &segment = *iter;
    if (iter == std::begin(segmentsList))
    {
      if (iter->first + dx == fruitX && iter->second + dy == fruitY)
        src.x = HeadOpenMouth * 64;
      else
        src.x = Head * 64;
      if (iter + 1 != std::end(segmentsList))
      {
        const auto &nextSegment = *(iter + 1);
        for (const auto &d: ds)
        {
          if (segment.first + d[0] == nextSegment.first && segment.second + d[1] == nextSegment.second)
          {
            direction = d[2];
            break;
          }
        }
      }
    }
    else if (iter + 1 == std::end(segmentsList))
    {
      src.x = Tail * 64;
      const auto &prevSegment = *(iter - 1);
      for (const auto &d: ds)
      {
        if (segment.first == prevSegment.first + d[0] && segment.second == prevSegment.second + d[1])
        {
          direction = d[2];
          break;
        }
      }
    }
    else
    {
      const auto &nextSegment = *(iter + 1);
      const auto &prevSegment = *(iter - 1);
      if (nextSegment.first == prevSegment.first)
      {
        src.x = Straight * 64;
        direction = 90;
      }
      else if (nextSegment.second == prevSegment.second)
      {
        src.x = Straight * 64;
        direction = 0;
      }
      else
      {
        src.x = Turn * 64;
        bool up = false;
        if (segment.first == nextSegment.first && segment.second - 1 == nextSegment.second)
          up = true;
        if (segment.first == prevSegment.first && segment.second - 1 == prevSegment.second)
          up = true;
        bool right = false;
        if (segment.first + 1 == nextSegment.first && segment.second == nextSegment.second)
          right = true;
        if (segment.first + 1 == prevSegment.first && segment.second == prevSegment.second)
          right = true;
        bool down = false;
        if (segment.first == nextSegment.first && segment.second + 1 == nextSegment.second)
          down = true;
        if (segment.first == prevSegment.first && segment.second + 1 == prevSegment.second)
          down = true;
        bool left = false;
        if (segment.first - 1 == nextSegment.first && segment.second == nextSegment.second)
          left = true;
        if (segment.first - 1 == prevSegment.first && segment.second == prevSegment.second)
          left = true;
        if (up && right)
          direction = 0;
        else if (right && down)
          direction = 90;
        else if (down && left)
          direction = 180;
        else if (left && up)
          direction = 270;
      }
    }

    dest.x = 64 * segment.first;
    dest.y = 64 * segment.second;
    SDL_RenderCopyEx(renderer, sprites, &src, &dest, direction, nullptr, SDL_FLIP_NONE);

  }
  src.x = Fruit * 64;
  dest.x = fruitX * 64;
  dest.y = fruitY * 64;
  SDL_RenderCopyEx(renderer, sprites, &src, &dest, 0, nullptr, SDL_FLIP_NONE);

}

