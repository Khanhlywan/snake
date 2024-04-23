#include "snake.h"
#include "sdl_check.h"


Point snake[100];

bool Snake::isEmpty(){
	FILE *fp;
	long size;
	fp = fopen("highscore.txt","r");
	if(fp){
		fseek (fp, 0, SEEK_END);
        size = ftell(fp);
		fclose(fp);
	}
	return(size == 0);
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

void Snake::classic(){
    for (int i = 0; i < snakeLength; i++) {
        if (i == 0) {
            snake[0].x0 = snake[0].x;snake[0].y0 = snake[0].y;
			snake[0].x += direction.x;
			snake[0].y += direction.y;
        }else{
            snake[i].x0 = snake[i].x;snake[i].y0 = snake[i].y;
			snake[i].x = snake[i-1].x0;snake[i].y = snake[i-1].y0;
        }

        if (snake[i].x >= MAXX) snake[i].x = MINX + 10;
        if (snake[i].x <= MINX) snake[i].x = MAXX - 10;
        if (snake[i].y >= MAXY) snake[i].y = MINY + 10;
        if (snake[i].y <= MINY) snake[i].y = MAXY - 10;

        if (i != 0 && (snake[0].x == snake[i].x && snake[0].y == snake[i].y)) endGame = true;
    }
    if (snake[0].x == food.x && snake[0].y == food.y){
		snake[snakeLength].x = snake[snakeLength-1].x0;snake[snakeLength].y = snake[snakeLength-1].y0;
		snakeLength++;
		PlaySound(TEXT("eatFood.wav"), NULL, SND_ASYNC);
		//Random again food if sanke ate food
		srand ( time(NULL));
        do{
        	food.x = (rand() % (39) + 3)*10;
    		food.y = (rand() % (19) + 3)*10;
		}while (checkPoint() == false);
	}
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
    if (p.first < 0 || p.first >= Width / 64 ||
        p.second < 0 || p.second >= Height / 64)
      return false;
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

  /*SDL_Color textColor = {255, 255, 255};
    SDL_Surface *textSurface = TTF_RenderText_Solid(font, ("Score: " + std::to_string(score)).c_str(), textColor);
    SDL_Texture *textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
    SDL_Rect textRect = {10, 10, textSurface->w, textSurface->h};
    SDL_RenderCopy(renderer, textTexture, nullptr, &textRect);
    SDL_FreeSurface(textSurface);
    SDL_DestroyTexture(textTexture);

    // Cập nhật màn hình
    SDL_RenderPresent(renderer);*/
}

void Snake::showHighScore(){
	FILE *f;
	f = fopen("highscore.txt", "r");
	char ch[20];
	settextstyle (1,0,5);
	setcolor (15);
	outtextxy (150,50,"HIGH SCORE");
	settextstyle (1,0,4);
	int y = 150,count = 0;
	while (!feof(f)){
		if (count == 10) break;
		count++;
		fscanf(f, "%s", ch);
   		if (count%2 == 1){
   			setcolor (10);
   			outtextxy (180,y,ch);
   			y+=50;
		}else{
			setcolor (12);
			outtextxy (500,y-50,ch);
		}
	}
	fclose(f);
}
void Snake::getHighScore (){
	FILE *f;
	f = fopen("highscore.txt", "w");
	for (int i = 0;i < 5;i++){
		fputs(highscore[i].name,f);
		fputs(" ",f);
		fprintf(f,"%d",highscore[i].score);
		fputs("\n",f);
	}
	fclose(f);
}
void Snake::checkHighScore (int _score){
	char _name[20]={""};
	for (int i = 0;i < 5;i++){
		if (_score > highscore[i].score){
			//to do sth
			settextstyle(1,0,3);
				for (int j = 0;j < 50;j++){
					if (j%2 == 0){
						setcolor (14);
						if (i == 0)
						outtextxy(460,100,"BEST SCORE");
						else
						outtextxy(460,100,"HIGH SCORE");
						delay(100);
					}else{
						setcolor (9);
						if (i == 0)
						outtextxy(460,100,"BEST SCORE");
						else
						outtextxy(460,100,"HIGH SCORE");
						delay(100);
					}
				}
				settextstyle(1,0,2);
				setcolor (4);outtextxy(430,150,"Player:");
				delay(100);
				char ch1;
				int x = 0;
				char str[2];
				str[1] = 0;
				while (ch1 != 13 && x < 10){
					do{
						ch1 = getch();
					}while (ch1 < 65 && ch1 >90 || ch1 < 97 && ch1 > 132);
					x++;
					str[0] = ch1;
					strcat(_name,str);
					outtextxy(540,150,_name);
				}
			for (int j = 4;j > i;j--){
				strcpy(highscore[j].name,highscore[j-1].name);
				highscore[j].score = highscore[j-1].score;
			}
			strcpy(highscore[i].name,_name);
			highscore[i].score = _score;
			break;
		}
	}
	getHighScore();
}

/*SDL_Surface* LoadImage(std::string file_path)
{
  SDL_Surface * load_image = NULL;
  SDL_Surface* optimize_image = NULL;
  load_image = IMG_Load(file_path.c_str());
  if (load_image != NULL)
  {
    optimize_image= SDL_DisplayFormat(load_image);
    SDL_FreeSurface(load_image);
  }
  return optimize_image;
}

void ApplySurface(SDL_Surface* src, SDL_Surface* des, int x, int y)
{
  SDL_Rect offset;
  offset.x = x;
  offset.y = y;
  SDL_BlitSurface(src, NULL, des, &offset);
}

void CleanUp()
{
  SDL_FreeSurface(g_screen);
  SDL_FreeSurface(g_bkground);
}*/

