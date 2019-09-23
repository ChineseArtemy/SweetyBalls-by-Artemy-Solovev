#include "TXLib.h"
#include "Windows.h"


const float t = 1,
            SPEEDMAX = 50;


struct ballType{
    int x;
    int y;
    float speedX;
    float speedY;
    COLORREF color;
    int radius;
    int lightX;
    int lightY;
};

void moveBall(ballType* ball)
{
    ball->x += floor(ball->speedX * t);
    ball->y += floor(ball->speedY * t);
}


void drawBall(){
    int x, y, r;
    std::cin>>x>>y>>r;
    int norm = r;
    for (r; r>0; r--){
        txSetFillColor(RGB(255 - (r*255)/norm, 0, 0));
        txSetColor(RGB(255 - (r*255)/norm, 0, 0));
        txCircle(x,y,r);
    }
}


void drawSetOfBalls(){
    int number;
    std::cin>>number;
    for (number; number > 0; number--){
        drawBall();
        }
    }


void drawLightedBall(ballType ball){
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
        /*int lX = ball.lightX;
        int lY = ball.lightY;*/
        int x = ball.x;
        int y = ball.y;
        /*double sinus = (lY-y)/sqrt((lX-x)*(lX-x)+(lY-y)*(lY-y));
        double cosinus = (lX-x)/sqrt((lX-x)*(lX-x)+(lY-y)*(lY-y));
        txCircle(floor(x-(i/2)*cosinus), floor(y-(i/2)*sinus), i);*/
        txCircle(x, y, i);
    }
    txSetFillColor (defaults);
}

bool mouseIsNotPressed()
{
    if (txMouseButtons() == 1)
        {
            return false;
        }
    return true;
}


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

int main()
{
    txCreateWindow(1200,700);
    txSetFillColor (TX_BLACK);
    ballType myBall;
    myBall.color = TX_GRAY;
    myBall.x = 600;
    myBall.y = 350;
    myBall.speedX = 0;
    myBall.speedY = 0;
    myBall.radius = 50;
    myBall.lightX = 600;
    myBall.lightY = 350;
    HDC finalPicture = txLoadImage("C:\\Users\\Gangsta Goose\\Downloads\\646570_original.bmp");
    //std::cin >> posX >> posY >>lghtX >> lghtY
    for (;;)
    {
        controllOfBall_Mouse(&myBall);
        controllOfBall_Keys(&myBall);
        controllOfBall_Borders(&myBall, 1200, 700);
        moveBall(&myBall);
        txBegin();
        txClear();
        drawLightedBall(myBall);
        txEnd();
        txSleep(t);
        if (GetAsyncKeyState(VK_ESCAPE) )
        {
            txTextOut (100, 100, "œ–Œƒ¿Ã √¿–¿∆. +79197091224");
            txTextOut (myBall.x - 45, myBall.y, "¬Ò“‡¬Ë“¸ “Â Ò“");
            txBitBlt (txDC(), 100, 150, 400,800, finalPicture);
            break;
        }
    }
    txDeleteDC (finalPicture);
    return 0;
}


