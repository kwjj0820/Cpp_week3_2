#include "console.h"
#include <string>

#define BOARD_SIZE 20
#define MOVE_DELAY 15
#define WALL_VERTICAL_STRING "┃"
#define WALL_HORIZONTAL_STRING "━"
#define WALL_RIGHT_TOP_STRING "┓"
#define WALL_LEFT_TOP_STRING "┏"
#define WALL_RIGHT_BOTTOM_STRING "┛"
#define WALL_LEFT_BOTTOM_STRING "┗"
#define SNAKE_STRING "■"
#define SNAKE_BODY_STRING "■"
#define APPLE_STRING "●"

console::Key dir;
int x, y, appleX, appleY, bodyCount, score, timer, index;
bool gameover = false;
int bodyX[BOARD_SIZE];
int bodyY[BOARD_SIZE];
std::string scoreString = "";
int isEmptyX[BOARD_SIZE * BOARD_SIZE];
int isEmptyY[BOARD_SIZE * BOARD_SIZE];

void setup()
{
  index = 0;
  timer = 0;
  x = BOARD_SIZE / 2;
  y = BOARD_SIZE / 2;
  gameover = false;
  appleX = rand() % (BOARD_SIZE - 2) + 1;
  appleY = rand() % (BOARD_SIZE - 2) + 1;
  score = 0;
  bodyCount = 1;
}

void handleInput() {
  if(x >= BOARD_SIZE - 1 || y >= BOARD_SIZE - 1 || x <= 0 || y <= 0)
  {
    gameover = true;
  }
  if (console::key(console::K_LEFT) && (dir != console::K_RIGHT || bodyCount == 1)) {
    dir = console::K_LEFT;
  }
  if (console::key(console::K_RIGHT) && (dir != console::K_LEFT || bodyCount == 1)) {
    dir = console::K_RIGHT;
  }
  if (console::key(console::K_UP) && (dir != console::K_DOWN || bodyCount == 1)) {
    dir = console::K_UP;
  }
  if (console::key(console::K_DOWN) && (dir != console::K_UP || bodyCount == 1)) {
    dir = console::K_DOWN;
  }
  if (console::key(console::K_ESC))
  {
    exit(0);
  }
  switch(dir)
  {
    case console::K_LEFT:
    {
      x--;
      break;
    }
    case console::K_RIGHT:
    {
      x++;
      break;
    }
    case console::K_UP:
    {
      y--;
      break;
    }
    case console::K_DOWN:
    {
      y++;
      break;
    }
  }
}

void restrictInScreen() {
  // x, y 위치를 화면의 최대 크기에서 벗어나지 않게 한다.
  if (x < 1)
    x++;
  if (x >= BOARD_SIZE - 1)
    x = BOARD_SIZE - 2;
  if (y < 1)
    y++;
  if (y >= BOARD_SIZE - 1)
    y = BOARD_SIZE - 2;
}

void drawBoard()
{
  index = 0;
  for (int i = 0; i < BOARD_SIZE; i++)
  {
    for (int j = 0; j < BOARD_SIZE; j++)
    {
    if(i == x && j == y)
    {
       console::draw(i, j, SNAKE_STRING);
    }
    else if(i == appleX && j == appleY)
    {
      if(i == x && j == y) console::draw(i, j, SNAKE_STRING);
      else console::draw(i, j, APPLE_STRING);
    }
    else if (i == 0)
    {
      if(j == 0) console::draw(i, j, WALL_LEFT_TOP_STRING);
      else if(j == BOARD_SIZE - 1) console::draw(i, j, WALL_LEFT_BOTTOM_STRING);
      else console::draw(i, j, WALL_VERTICAL_STRING);
    }
    else if (i == BOARD_SIZE - 1)
    {
      if (j == 0) console::draw(i, j, WALL_RIGHT_TOP_STRING);
      else if (j == BOARD_SIZE - 1) console::draw(i, j, WALL_RIGHT_BOTTOM_STRING);
      else console::draw(i, j, WALL_VERTICAL_STRING);
    }
    else if (j == 0 || j == BOARD_SIZE - 1) console::draw(i, j, WALL_HORIZONTAL_STRING);
    else
    {
      bool print = false;
      for(int k = 0; k < bodyCount; k++)
      {
        if(bodyX[k] == i && bodyY[k] == j)
        {
          console::draw(i, j, SNAKE_BODY_STRING);
          print = true;
        }
      }
      if(!print)
      {
        isEmptyX[index] = i;
        isEmptyY[index] = j;
        index++;
        console::draw(i, j, " ");
      }
    }
   }
  }
  scoreString = "Score: " + std::to_string(score);
  console::draw((BOARD_SIZE - scoreString.size())/2, BOARD_SIZE, scoreString);
}

void logic()
{
  if(bodyCount >= (BOARD_SIZE - 2) * (BOARD_SIZE - 2))
  {
    console::draw(BOARD_SIZE / 2 - 1, BOARD_SIZE / 2 - 1, "WIN!");
  }

  // 뱀이 사과에 닿았을 때
  if(x == appleX && y == appleY)
  {
    score += 10;
    int rand_num = rand() % index;
    appleX = isEmptyX[rand_num];
    appleY = isEmptyY[rand_num];
    bodyCount++;
  }

  // 꼬리 로직
  int prevX = bodyX[0];
  int prevY = bodyY[0];
  int prev2X, prev2Y;
  bodyX[0] = x;
  bodyY[0] = y;
  for(int i = 1; i < bodyCount; i++)
  {
    prev2X = bodyX[i];
    prev2Y = bodyY[i];
    bodyX[i] = prevX;
    bodyY[i] = prevY;
    prevX = prev2X;
    prevY = prev2Y;
  }

  // 뱀이 몸통에 닿았을 때
  for(int i = 1; i < bodyCount; i++)
  {
    if(bodyX[i] == x && bodyY[i] == y)
    gameover = true;
  }
}

void game() {
  // 화면을 지운다.
  console::clear();
  drawBoard();
  if(timer >= MOVE_DELAY)
  {
    handleInput();
    logic();
    timer = 0;
  }
  else timer++;
  if(gameover)
  {
    console::draw(BOARD_SIZE / 2 - 4, BOARD_SIZE / 2, "YOU LOSE!");
    console::draw(BOARD_SIZE / 2 - 9, BOARD_SIZE / 2 + 1,"Try again? (Enter)");
  }
}

int main()
{
  console::init();
  setup();
  while(true)
  {
    while(gameover)
    {
      if(console::key(console::K_ENTER))
      {
        gameover = false;
        setup();
      }
      else if(console::key(console::K_ESC)) exit(0);
    }
    game();
    console::wait();
  }
  return 0;
}