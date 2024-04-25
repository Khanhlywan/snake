#include "snake.h"
#include "sdl_check.h"
#include "graphics.h"

Snake::Snake()
{
     SDL_Init(SDL_INIT_EVERYTHING);
     window =SDL_CreateWindow("Snake",SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED,1260,600,SDL_WINDOW_SHOWN);
     renderer=SDL_CreateRenderer(window,-1,SDL_RENDERER_ACCELERATED|SDL_RENDERER_PRESENTVSYNC);
     SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");
     SDL_RenderSetLogicalSize(renderer, 1260, 600);

     if (Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 4096) == -1) {
        std::cerr << "Failed to initialize audio: " << Mix_GetError() << std::endl;
        return;
    }

    // Tải các tệp âm thanh
    eatSound = Mix_LoadWAV("eatFood.wav");
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

     auto menuSurface = IMG_Load("menu.png");
     SDL_CHECK(backgroundSurface, "Failed to load menu image");
     menuTexture = SDL_CreateTextureFromSurface(renderer, menuSurface);
     SDL_CHECK(backgroundTexture, "Failed to create menu texture");
     SDL_FreeSurface(menuSurface);

     auto surface = SDL_LoadBMP("sprites.bmp");
     //auto surface = SDL_LoadBMP("Garden-Background.png");
     SDL_CHECK(surface, "SDL_LoadBMP(\"sprites.bmp\")");
     sprites = SDL_CreateTextureFromSurface(renderer, surface);
     SDL_CHECK(sprites, "SDL_CreateTextureFromSurface");
     SDL_FreeSurface(surface);


     // Tải hình ảnh "Play Again" vào `SDL_Texture`
     auto playAgainSurface = IMG_Load("Play Again.png");
     SDL_CHECK(playAgainSurface, "Failed to load play Again image");
     playAgainTexture = SDL_CreateTextureFromSurface(renderer,playAgainSurface);
     SDL_CHECK(playAgainTexture, "Failed to create play again texture");
     SDL_FreeSurface(playAgainSurface);

     lastMoveTime = SDL_GetTicks();

     segmentsList.push_back(std::make_pair(5, 5));
     segmentsList.push_back(std::make_pair(5, 6));
     segmentsList.push_back(std::make_pair(4, 6));
     generateFruit();


}

void Snake::onFruitEaten() {
    // Tăng tốc độ di chuyển của con rắn (giảm thời gian giữa các lần di chuyển)
    currentSpeed -= 0.01f; // Giảm 10% thời gian giữa các lần cập nhật vị trí của con rắn
    // Giới hạn tốc độ tối thiểu để đảm bảo trò chơi vẫn playable
    if (currentSpeed < 0.02f) {
        currentSpeed = 0.02f;
    }

    // Thực hiện các hành động khác khi con rắn ăn quả
    generateFruit();
    Mix_PlayChannel(-1, eatSound, 0);
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
    // Giải phóng âm thanh
    Mix_FreeChunk(eatSound);
    Mix_FreeChunk(deathSound);
    // Đóng SDL_Mixer
    Mix_CloseAudio();
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
                        if (dy != 1) {
                            dx = 0;
                            dy = -1;
                        }
                        break;
                    case SDLK_RIGHT:
                        if (dx != -1) {
                            dx = 1;
                            dy = 0;
                        }
                        break;
                    case SDLK_DOWN:
                        if (dy != -1) {
                            dx = 0;
                            dy = 1;
                        }
                        break;
                    case SDLK_LEFT:
                        if (dx != 1) {
                            dx = -1;
                            dy = 0;
                        }
                        break;
                     }
                 }
                 break;
          case SDL_QUIT:
            done = true;
            break;
         }
       }
       if(done){
        break;
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
    SDL_DestroyTexture(backgroundTexture);
    SDL_DestroyTexture(playAgainTexture);
    SDL_DestroyTexture(menuTexture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;


}
void Snake::displayPlayAgainScreen() {
    // Hiển thị hình ảnh "Play Again" trên màn hình
    SDL_Rect menuRect  = {1260,600};
    SDL_RenderCopy(renderer,menuTexture,NULL,NULL);
    SDL_Rect playAgainRect = {Width / 2 - 80, Height / 2 - 50, 200, 100 };  // Vị trí và kích thước của hình ảnh
    SDL_RenderCopy(renderer, playAgainTexture, NULL, &playAgainRect);

    //Hien thi noi dung
    SDL_RenderPresent(renderer);
    // Chờ sự kiện từ người chơi
    SDL_Event event;
    while (true) {
        if (SDL_PollEvent(&event)) {
            if (event.type == SDL_KEYDOWN) {
                // Kiểm tra nếu người dùng nhấn phím Enter
                if (event.key.keysym.sym == SDLK_RETURN) {
                    resetGame();  // Bắt đầu lại trò chơi
                    break;
                }
            } else if (event.type == SDL_MOUSEBUTTONDOWN) {
                // Kiểm tra nếu người dùng nhấn vào nút chuột
                int x, y;
                SDL_GetMouseState(&x, &y);

                // Kiểm tra xem vị trí chuột có ở trên nút "Play Again" hay không
                if (x >= playAgainRect.x && x <= playAgainRect.x + playAgainRect.w &&
                    y >= playAgainRect.y && y <= playAgainRect.y + playAgainRect.h) {
                    resetGame();  // Bắt đầu lại trò chơi
                    break;
                }

            } else if (event.type == SDL_QUIT) {
                // Người dùng muốn thoát trò chơi bằng cách nhấn nút đóng cửa sổ
                exitGame();  // Thoát trò chơi
                break;
            }
        }
    }
}

void Snake::exitGame() {
    // Giải phóng tài nguyên
    Mix_FreeChunk(eatSound);
    Mix_FreeChunk(deathSound);
    Mix_CloseAudio();

    // Giải phóng kết cấu và cửa sổ
    SDL_DestroyTexture(backgroundTexture);
    SDL_DestroyTexture(playAgainTexture);
    SDL_DestroyTexture(menuTexture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);

    // Đóng SDL
    SDL_Quit();

    // Thoát ứng dụng
    exit(0);
}

void Snake::resetGame() {
    // Thiết lập lại trạng thái trò chơi
    segmentsList.clear();
    segmentsList.push_back(std::make_pair(5, 5));
    segmentsList.push_back(std::make_pair(5, 6));
    segmentsList.push_back(std::make_pair(4, 6));
    dx = 1;
    dy = 0;
    //score = 0;
    generateFruit();  // Tạo quả mới

    exec();
}



bool Snake::tick()
{
  static Uint32 lastMoveTime = SDL_GetTicks();
  static float initialSpeed = 200.0f; // Tốc độ ban đầu
  static float speedIncrement = 2.0f; // Lượng tăng tốc độ khi ăn quả
  static float minSpeed = 30.0f; // Tốc độ tối thiểu (ban đầu)

 /*if (ticks++ % 250 == 0)
  {*/
  Uint32 currentTime = SDL_GetTicks();
    if (currentTime - lastMoveTime >= currentSpeed) {
        lastMoveTime = currentTime;  // Cập nhật thời gian di chuyển cuối cùng

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

    for (const auto &segment : segmentsList) {
      if (p == segment) {
        Mix_PlayChannel(-1, deathSound, 0);
        displayPlayAgainScreen();
        return false;
      }
    }
    segmentsList.push_front(p);
    if (p.first != fruitX || p.second != fruitY) {
      segmentsList.pop_back();
    } else {
      generateFruit();
      Mix_PlayChannel(-1, eatSound, 0);
      /*currentSpeed = std::max(initialSpeed - speedIncrement * segmentsList.size(), 50.0f);

            // In giá trị currentSpeed để kiểm tra
            std::cout << "Current speed: " << currentSpeed << std::endl;*/

    } // Tăng tốc độ của con rắn bằng cách giảm giá trị `currentSpeed`
            currentSpeed = std::max(initialSpeed - speedIncrement * (segmentsList.size() - 1), minSpeed);

            // In giá trị currentSpeed để kiểm tra
            std::cout << "Current speed: " << currentSpeed << std::endl;
        }
 //}
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
  SDL_RenderPresent(renderer);

}

