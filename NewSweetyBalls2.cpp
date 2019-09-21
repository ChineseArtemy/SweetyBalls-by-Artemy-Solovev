#include "TXLib.h"
#include "Windows.h"
struct ballType{
    int positionX;
    int positionY;
    COLORREF color;
    int radius;
    int lightX;
    int lightY;
};

void moveBall(ballType ballName, int newX, int newY)
{
    ballName.positionX = newX;
    ballName.positionY = newY;
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
        int lX = ball.lightX;
        int lY = ball.lightY;
        int x = ball.positionX;
        int y = ball.positionY;
        double sinus = (lY-y)/sqrt((lX-x)*(lX-x)+(lY-y)*(lY-y));
        double cosinus = (lX-x)/sqrt((lX-x)*(lX-x)+(lY-y)*(lY-y));
        txCircle(floor(x-(i/2)*cosinus), floor(y-(i/2)*sinus), i);
    }
    txSetFillColor (defaults);
}


void movingBallByMouse(ballType ball){
    while (txMouseButtons() != 2){
        while (txMouseButtons() == 1){
            //moveBall(ball, txMouseX(), txMouseY());
            ball.positionX = txMouseX();
            ball.positionY = txMouseY();
            txBegin();
            drawLightedBall(ball);
            txEnd();
            txSleep (0);
            txClear();
        }
    }
}


void movingBallByKeys(ballType ball){
    while (!GetAsyncKeyState( 32 )){
        while (GetAsyncKeyState( 38 )){
            //moveBall(ball, x, y-2);
            ball.positionY -= 2;
            txBegin();
            txClear();
            drawLightedBall(ball);
            txEnd();
            txSleep(0.01);
        }
        while (GetAsyncKeyState( 40 )){
            ball.positionY += 2;
            //moveBall(ball, x, y+2);
            txBegin();
            txClear();
            drawLightedBall(ball);
            txEnd();
            txSleep(0.01);
        }
        while (GetAsyncKeyState( 39 )){
            ball.positionX += 2;
            txBegin();
            txClear();
            drawLightedBall(ball);
            txEnd();
            txSleep(0.01);
        }
        while (GetAsyncKeyState( 37 )){
            ball.positionX -= 2;
            txBegin();
            txClear();
            drawLightedBall(ball);
            txEnd();
            txSleep(0.01);
            txClear();
        }
    }
}

int main(){
    txCreateWindow(1200,700);
    txSetFillColor (TX_BLACK);
    ballType myBall;
    myBall.positionX = 600;
    myBall.positionY = 350;
    myBall.radius = 100;
    myBall.lightX = 600;
    myBall.lightY = 350;
    //std::cin >> posX >> posY >>lghtX >> lghtY
    movingBallByMouse(myBall);
    movingBallByKeys(myBall);
    return 0;
}


