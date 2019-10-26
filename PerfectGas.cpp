#include "TXLib.h"
#include "Windows.h"
#include "fvectorlib.h"
#include <cmath>


const float t = 1,
            SPEEDMAX = 50;
const int thickness = 30;

struct ballType
{
    ffVector pos;
    ffVector speed;
    ffVector accel;
    COLORREF color = TX_YELLOW;
    int mass;
    float radius = 20;
    int lightX;
    int lightY;
    int healPoints = 10;
};

struct palletType
{
    int x;
    int y;
    int length;
    float speed;
    float angle;
};

int sign(int x)
{
    if (x >= 0)
    {
        return 1;
    }
    else
    {
        return -1;
    }
}

void movePallet (palletType* wall)
{
    wall->x += floor(wall->speed * t);
}

void moveBall(ballType* ball)
{
    ball->pos.x += floor(ball->speed.x * t);
    ball->pos.y += floor(ball->speed.y * t);
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
        int x = ball.pos.x;
        int y = ball.pos.y;
        txCircle(x, y, i);
    }
    txSetFillColor (defaults);
}

void drawPallet(palletType wall)
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
    if (sqrt((ball1.pos.x - ball2.pos.x)*(ball1.pos.x - ball2.pos.x) + (ball1.pos.y - ball2.pos.y)*(ball1.pos.y - ball2.pos.y)) <= ball1.radius + ball2.radius)
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
        ball->speed.x = (mouseX - ball->pos.x+1)/10;
        ball->speed.y = (mouseY - ball->pos.y+1)/10;
    }
}

void controllOfBall_Borders(ballType* ball, int screenWidth, int screenHeight)
{
    if ((ball->pos.x <= ball->radius)  or (screenWidth - ball->pos.x <= ball->radius))
    {
        if (ball->pos.x <= ball->radius)
        {
            ball->pos.x = ball->radius + 1;
        }
        else
        {
            ball->pos.x = screenWidth - (ball->radius + 1);
        }
        ball->speed.x *= -1;
    }

    if ((ball->pos.y <= ball->radius)  or (screenHeight - ball->pos.y <= ball->radius))
    {
        if (ball->pos.y <= ball->radius)
        {
            ball->pos.y = ball->radius + 1;
        }
        else
        {
            ball->pos.y = screenHeight - (ball->radius + 1);
        }
        ball->speed.y *= -1;
    }
}

void controllOfBall_Keys(ballType* ball)
{
    if (GetAsyncKeyState(VK_UP) and ball->speed.y != (-1)*SPEEDMAX)
    {
        ball->speed.y -= 2;
    }
    if (GetAsyncKeyState(VK_DOWN) and ball->speed.y != SPEEDMAX)
    {
        ball->speed.y += 2;
    }
    if (GetAsyncKeyState(VK_RIGHT) and ball->speed.x != SPEEDMAX)
    {
        ball->speed.x += 2;
    }
    if (GetAsyncKeyState(VK_LEFT) and ball->speed.x != (-1)*SPEEDMAX)
    {
        ball->speed.x -= 2;
    }
    if (GetAsyncKeyState(VK_SPACE))
    {
            ball->speed.x = 0;
            ball->speed.y = 0;
    }
}

void controllOfPallet_Keys(palletType* wall)
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

void controllOfPallet_Borders(palletType* wall, int screenWidth, int screenHeight)
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

bool collidedWithPallet(ballType ball, palletType wall)
{
    if ((abs(wall.y - ball.pos.y) <= ball.radius) and (ball.pos.x <= wall.x + wall.length) and (ball.pos.x >= wall.x))
    {
        return true;
    }
    return false;
}
void solvePalletCollision(ballType* ball, palletType wall)
{
    ball->pos.y = wall.y - ball->radius;
    ball->speed.y *= -1;
}

void solveBallCollision(ballType* ball1, ballType* ball2)
{
    ffVector v1, v2;
        v1 = ball1->speed;
        v2 = ball2->speed;
    ffVector distance;
        distance.x = ball2->pos.x - ball1->pos.x;
        distance.y = ball2->pos.y - ball1->pos.y;
    ffVector r, n;
        r = distance.normalize();
        n.x = r.y;
        n.y = -r.x;
    ffVector vc;
        vc = (v1 * ball1->radius + v2 * ball2->radius)/(ball1->radius + ball2->radius);

        if (dotProd(v1-v2, r) > 0)
        {
            ball2->pos = ball1->pos - (distance + r);

            ball1->speed = /*mult(*/vc * 2 - r * dotProd(r, v1) + n * dotProd(n, v1); //, sign(cos(angle(r,v1))));
            ball2->speed = /*mult(*/vc * 2 - r * dotProd(r, v2) + n * dotProd(n, v2);//, sign(cos(angle(r,v2))));
        }
        //ball1->speed.x = dotProd( mult( n, dotProd(v1, n) ), e1 ) - dotProd( mult( r, dotProd(v1, r) ), e1);
        //ball1->speed.y = dotProd( mult( n, dotProd(v1, n) ), e2 ) - dotProd( mult( r, dotProd(v1, r) ), e2);
        //ball2->speed.x = dotProd( mult( n, dotProd(v2, n) ), e1 ) - dotProd( mult( r, dotProd(v2, r) ), e1);
        //ball2->speed.y = dotProd( mult( n, dotProd(v2, n) ), e2 ) - dotProd( mult( r, dotProd(v2, r) ), e2);


        //ball1->pos.x += sign( dotProd(v1, n) ) * ball1->radius * dotProd( e1, r );
        //ball1->pos.y += sign( dotProd(v1, n) ) * ball1->radius * dotProd( e2, r );
        //ball2->pos.x += -sign( dotProd(v2, n) ) * ball2->radius * dotProd( e1, r );
        //ball2->pos.y += -sign( dotProd(v2, n) ) * ball2->radius * dotProd( e2, r )
}
int main()
{
    txCreateWindow(1200,700);
    txSetFillColor (TX_BLACK);
    int num = 4;
    ballType* balls = new ballType[num];
    for (int i = 0; i < num; i++)
    {
        balls[i].pos.x = i*200;
        balls[i].pos.y = 200;
        balls[i].speed.x = 0;
        balls[i].speed.y = 0;
    }

    /*ballType Player1, Player2;
    Player1.color = TX_GRAY;
    Player2.color = TX_RED;
    Player1.pos.x = 200;
    Player1.pos.y = 350;
    Player2.pos.x = 1000;
    Player2.pos.y = 350;
    Player1.speed.x = 0;
    Player1.speed.y = 0;
    Player2.speed.x = 0;
    Player2.speed.y = 0;
    Player1.radius = 50;
    Player2.radius = 5;
    Player1.healPoints = 10;
    palletType testWall;
    testWall.x = 400;
    testWall.y = 500;
    testWall.length = 300;
    testWall.speed = 10;   */
    HDC finalPicture = txLoadImage("C:\\Users\\Gangsta Goose\\Downloads\\646570_original.bmp");
    for (;;)
    {
        /*controllOfBall_Keys(&Player1);
        controllOfBall_Borders(&Player1, 1200, 700);
        controllOfPallet_Keys(&testWall);
        controllOfPallet_Borders(&testWall, 1200, 700);
        moveBall(&Player1);
        movePallet(&testWall);
        if (collidedWithPallet(Player1, testWall))
        {
            solvePalletCollision(&Player1, testWall);
            Player1.healPoints -= 1;
            Player1.radius -= 5;
        }    */
        controllOfBall_Keys(&balls[0]);
        for(int i = 0; i < num; i++)
        {
            for (int j = i+1; j < num; j++)
            {
                if (theyTwoCollided(balls[i], balls[j]))
                {
                    solveBallCollision(&balls[i], &balls[j]);
                }
            }
            controllOfBall_Borders(&balls[i], 1200, 700);
            moveBall(&balls[i]);
        }
        txBegin();
        txClear();
        for (int i = 0;i < num; i++)
        {
            drawLightedBall(balls[i]);
        }
        //drawLightedBall(Player1);
        //drawPallet(testWall);
        txEnd();
        txSleep(t);
        if (GetAsyncKeyState(VK_ESCAPE) /*or (Player1.healPoints <= 0)*/)
        {
            txTextOut (100, 100, "ÏÐÎÄÀÌ ÃÀÐÀÆ. +79197091224");
            //txTextOut (Player1.pos.x - 45, Player1.pos.y, "GAME OVER");
            txBitBlt (txDC(), 100, 150, 400,800, finalPicture);
            break;
        }
    }
    delete [] balls;
    txDeleteDC (finalPicture);
    return 0;
}


