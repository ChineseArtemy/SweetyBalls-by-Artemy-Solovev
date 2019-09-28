#include "TXLib.h"
#include "Windows.h"


const float t = 1,
            SPEEDMAX = 50;
const int thickness = 30;

struct ballType
{
    int x;
    int y;
    float speedX;
    float speedY;
    float accelerationX;
    float accelerationY;
    COLORREF color;
    int mass;
    int radius;
    int lightX;
    int lightY;
    int healPoints;
};

struct wallType
{
    int x;
    int y;
    int length;
    float speed;
    float angle;
};

void moveWall (wallType* wall)
{
    wall->x += floor(wall->speed * t);
}

void moveBall(ballType* ball)
{
    ball->x += floor(ball->speedX * t);
    ball->y += floor(ball->speedY * t);
}

void drawLightedBall(ballType ball)
{
    int norm = ball.radius;
    COLORREF defaults = txGetFillColor();
    for(int i = norm; i>0; i--)
    {
        unsigned char R = GetRValue(ball.color);
        unsigned char G = GetGValue(ball.color);
        unsigned char B = GetBValue(ball.color);
        unsigned char darkerR = R-(R*i*230)/(ball.radius*255);
        unsigned char darkerG = G-(G*i*230)/(ball.radius*255);
        unsigned char darkerB = B-(B*i*230)/(ball.radius*255);
        COLORREF shadedColor = RGB(darkerR, darkerG, darkerB);
        txSetColor(shadedColor);
        txSetFillColor(shadedColor);
        int x = ball.x;
        int y = ball.y;
        txCircle(x, y, i);
    }
    txSetFillColor (defaults);
}

void drawWall(wallType wall)
{
    txSetColor(TX_LIGHTGRAY);
    POINT poly[4] = {{wall.x, wall.y}, {wall.x + 10, floor(wall.y + thickness/2)}, {wall.x + wall.length - 10, floor(wall.y + thickness/2)}, {wall.x + wall.length, wall.y}};
    txPolygon(poly, 4);
    txFloodFill(wall.x + 2, wall.y + 1, TX_LIGHTGRAY);
    txSetColor(TX_GRAY);
    txLine(wall.x, wall.y, wall.x, wall.y + thickness);
    txLine(wall.x + wall.length, wall.y, wall.x + wall.length, wall.y + thickness);
    txLine(wall.x, wall.y + thickness, wall.x + 10, floor(wall.y + thickness/2));
    txLine(wall.x + wall.length, wall.y + thickness, wall.x + wall.length - 10, floor(wall.y + thickness/2));
    txSetColor(TX_DARKGRAY);
    txLine(wall.x, wall.y + thickness, wall.x + wall.length, wall.y + thickness);
    txFloodFill(wall.x + 2, floor(wall.y + thickness/2), TX_GRAY);
    txFloodFill(wall.x + wall.length - 2, floor(wall.y + thickness/2), TX_GRAY);
    txFloodFill(wall.x + floor(wall.length/2), wall.y + thickness - 1, TX_DARKGRAY);
}

bool mouseIsNotPressed()
{
    if (txMouseButtons() == 1)
        {
            return false;
        }
    return true;
}

bool theyTwoCollided(ballType ball1, ballType ball2)
{
    if (sqrt((ball1.x - ball2.x)*(ball1.x - ball2.x) + (ball1.y - ball2.y)*(ball1.y - ball2.y)) <= ball1.radius + ball2.radius)
    {
        return true;
    }
    else
    {
        return false;
    }
}

/* void controllOfBall_Collision(ballType* ball1, ballType* ball2)
{
    if (theyTwoCollided(*ball1, *ball2))
    {
        float pcX = (ball1->radius*ball1->speedX + ball2->radius*ball2->speedX)/(ball1->radius + ball2.radius);
        float pcY = (ball1->radius*ball1->speedY + ball2->radius*ball2->speedY)/(ball1->radius + ball2.radius);


    }
    (ball1.y - ball2.y)
      */
void controllOfBall_Mouse(ballType* ball)
{
    if (txMouseButtons() == 1)
    {
        int mouseX = txMouseX();
        int mouseY = txMouseY();
        ball->speedX = (mouseX - ball->x+1)/10;
        ball->speedY = (mouseY - ball->y+1)/10;
    }
}

void controllOfBall_Borders(ballType* ball, int screenWidth, int screenHeight)
{
    if ((ball->x <= ball->radius)  or (screenWidth - ball->x <= ball->radius))
    {
        if (ball->x <= ball->radius)
        {
            ball->x = ball->radius + 1;
        }
        else
        {
            ball->x = screenWidth - (ball->radius + 1);
        }
        ball->speedX *= -1;
    }

    if ((ball->y <= ball->radius)  or (screenHeight - ball->y <= ball->radius))
    {
        if (ball->y <= ball->radius)
        {
            ball->y = ball->radius + 1;
        }
        else
        {
            ball->y = screenHeight - (ball->radius + 1);
        }
        ball->speedY *= -1;
    }
}

void controllOfBall_Keys(ballType* ball)
{
    if (GetAsyncKeyState(VK_UP) and ball->speedY != (-1)*SPEEDMAX)
    {
        ball->speedY -= 2;
    }
    if (GetAsyncKeyState(VK_DOWN) and ball->speedY != SPEEDMAX)
    {
        ball->speedY += 2;
    }
    if (GetAsyncKeyState(VK_RIGHT) and ball->speedX != SPEEDMAX)
    {
        ball->speedX += 2;
    }
    if (GetAsyncKeyState(VK_LEFT) and ball->speedX != (-1)*SPEEDMAX)
    {
        ball->speedX -= 2;
    }
}

void controllOfWall_Keys(wallType* wall)
{
     if (GetAsyncKeyState(VK_NUMPAD6) and wall->speed != SPEEDMAX)
    {
        wall->speed += 2;
    }
    if (GetAsyncKeyState(VK_NUMPAD4) and wall->speed != (-1)*SPEEDMAX)
    {
        wall->speed -= 2;
    }
}

void controllOfWall_Borders(wallType* wall, int screenWidth, int screenHeight)
{
    if ((wall->x <= 0)  or (screenWidth - wall->x <= wall->length))
    {
        if (wall->x <= 0)
        {
            wall->x = 1;
        }
        else
        {
            wall->x = screenWidth - (wall->length + 1);
        }
        wall->speed *= -1;
    }
}

bool collidedWithPallet(ballType ball, wallType wall)
{
    if ((abs(wall.y - ball.y) <= ball.radius) and (ball.x <= wall.x + wall.length) and (ball.x >= wall.x))
    {
        return true;
    }
    return false;
}
void solvePalletCollision(ballType* ball, wallType wall)
{
    ball->y = wall.y - ball->radius;
    ball->speedY *= -1;
}
int main()
{
    txCreateWindow(1200,700);
    txSetFillColor (TX_BLACK);
    ballType Player1, Player2;
    Player1.color = TX_GRAY;
    Player2.color = TX_RED;
    Player1.x = 200;
    Player1.y = 350;
    Player2.x = 1000;
    Player2.y = 350;
    Player1.speedX = 0;
    Player1.speedY = 0;
    Player2.speedX = 0;
    Player2.speedY = 0;
    Player1.radius = 50;
    Player2.radius = 5;
    Player1.healPoints = 10;
    wallType testWall;
    testWall.x = 400;
    testWall.y = 500;
    testWall.length = 300;
    testWall.speed = 10;
    HDC finalPicture = txLoadImage("C:\\Users\\Gangsta Goose\\Downloads\\646570_original.bmp");
    for (;;)
    {
        controllOfBall_Keys(&Player1);
        controllOfBall_Borders(&Player1, 1200, 700);
        controllOfWall_Keys(&testWall);
        controllOfWall_Borders(&testWall, 1200, 700);
        moveBall(&Player1);
        moveWall(&testWall);
        if (collidedWithPallet(Player1, testWall))
        {
            solvePalletCollision(&Player1, testWall);
            Player1.healPoints -= 1;
            Player1.radius -= 5;
        }
        txBegin();
        txClear();
        drawLightedBall(Player1);
        drawWall(testWall);
        txEnd();
        txSleep(t);
        if (GetAsyncKeyState(VK_ESCAPE) or (Player1.healPoints <= 0))
        {
            txTextOut (100, 100, "ÏÐÎÄÀÌ ÃÀÐÀÆ. +79197091224");
            txTextOut (Player1.x - 45, Player1.y, "GAME OVER");
            txBitBlt (txDC(), 100, 150, 400,800, finalPicture);
            break;
        }
    }
    txDeleteDC (finalPicture);
    return 0;
}


