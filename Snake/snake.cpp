#include "snake.h"
#include "sdl_check.h"
#include "graphics.h"


Snake::Snake()
{
     ultis
     setFixedObstacles();
     SDL_Init(SDL_INIT_EVERYTHING);
     window =SDL_CreateWindow("snake",SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED,1260,600,SDL_WINDOW_SHOWN);
     renderer=SDL_CreateRenderer(window,-1,SDL_RENDERER_ACCELERATED|SDL_RENDERER_PRESENTVSYNC);
     SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");
     SDL_RenderSetLogicalSize(renderer, 1260, 600);

     if (Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 4096) == -1) {
        std::cerr << "Failed to initialize audio: " << Mix_GetError() << std::endl;
        return;
    }

    // Tải các tệp âm thanh
    eatSound = Mix_LoadWAV("eat.wav");
    deathSound = Mix_LoadWAV("death.wav");
    if (!eatSound || !deathSound) {
        std::cerr << "Failed to load sound effects: " << Mix_GetError() << std::endl;
        return;
    }

     auto backgroundSurface = IMG_Load("background.png");
     SDL_CHECK(backgroundSurface, "Failed to load background image");
     backgroundTexture = SDL_CreateTextureFromSurface(renderer, backgroundSurface);
     SDL_CHECK(backgroundTexture, "Failed to create background texture");
     SDL_FreeSurface(backgroundSurface);
     SDL_RenderCopy(renderer, backgroundTexture, NULL, NULL);

     auto obstacleSurface = IMG_Load("stone.png");
     SDL_CHECK(obstacleSurface, "Failed to load obstacle image");
     obstacleTexture = SDL_CreateTextureFromSurface(renderer, obstacleSurface);
     SDL_CHECK(obstacleTexture, "Failed to create obstacle texture");
     SDL_FreeSurface(obstacleSurface);


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


void Snake::setFixedObstacles() {
    // Thêm các tọa độ chướng ngại vật khác vào map obstacles
    std::vector<std::pair<int, int>> fixedObstacles = {
        {30, 20},
        {50, 30},
        {70, 20},
    };


    // Khởi tạo danh sách obstacles
    obstacles.clear();
    for (const auto &coord : fixedObstacles) {
        obstacles.push_back(coord);
    }

}

/*bool Snake::checkCollision(int x, int y) {
    // Kiểm tra xem tọa độ đầu con rắn có va chạm với chướng ngại vật hay không
    // Nếu tọa độ (x, y) tồn tại trong map obstacles
    if (obstacles.count({x, y}) > 0) {
        return true; // Va chạm
    }
    return false; // Không va chạm
*/

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


   /* // Giải phóng bộ nhớ và thoát
    SDL_DestroyTexture(backgroundTexture);
    SDL_DestroyTexture(playButtonTexture);
    SDL_DestroyTexture(exitButtonTexture);*/

}

bool Snake::tick()
{
  setFixedObstacles();

  // Cập nhật tọa độ đầu con rắn

  if (ticks++ % 250 == 0)
  {
    for (const auto &obstacle : obstacles) {
      if (segmentsList.front().first == obstacle.first && segmentsList.front().second == obstacle.second) {
        return false; // Game over
      }
    }

    auto p = segmentsList.front();
    p.first += dx;
    p.second += dy;

    //Ran di xuyen qua man hinh
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
    for (auto &obstacle : obstacles) {
      if (p.first == obstacle.first && p.second == obstacle.second) {
        return false; // Kết thúc trò chơi nếu rắn đâm vào chướng ngại vật
      }
    }

    for (const auto &segment : segmentsList) {
      if (p == segment) {
        return false;
      }
    }
    segmentsList.push_front(p);
    if (p.first != fruitX || p.second != fruitY) {
      segmentsList.pop_back();
    } else {
      generateFruit();
      Mix_PlayChannel(-1, eatSound, 0);
    }
  }

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
  // Vẽ chướng ngại vật
    for (const auto &obstacle: obstacles) {
        dest.x = obstacle.first * 12;
        dest.y = obstacle.second* 12;
        SDL_RenderCopy(renderer, obstacleTexture, NULL, &dest);
        //SDL_RenderFillRect(renderer, &dest);
    }

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
  SDL_RenderPresent(renderer);

}

