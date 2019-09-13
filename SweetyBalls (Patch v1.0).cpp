#include "txlib.h"
#include <time.h>
#include <iostream>
#include <cmath>
void drawColoredBall(int x, int y, int r, int R,int G,int B);  //рисует 3D-шар радиуса r с центром в (x,y) цвета (R,G,B)

void drawOrientedBall(int x, int y, int lX, int lY, int r, COLORREF rgb); /*рисует 3D-шар радиуса r с центром в (x,y),
                                                                                цветом (R,G,B) и бликом в сторону источника с координатами (lX, lY) */

int get_rand(int min, int max);  //для получения случайных координат и цвета

void drawRingyBackground(int X, int Y, int size); //создаёт фон с тенью от источника (not done yet)

COLORREF getRandomRGB();

void taskWindow(int *szX, int *szY, int *num, int *lX, int *lY);

int main()
{
    int windowWidth = 0;
    int windowHeight = 0;
    int num = 0;
    int lightPosX = 0;
    int lightPosY = 0;
    taskWindow(&windowWidth, &windowHeight, &num, &lightPosX, &lightPosY);
    txCreateWindow(windowWidth, windowHeight);
    srand(time_t(0));
    txSetFillColor(getRandomRGB());
    txClear();
    int ballPosX, ballPosY;
    for( int i = 0; i < num; i++)
    {
        COLORREF ballColor = getRandomRGB();
        int rad = get_rand(50, 100);                 // определяем цвет шара и его координаты
        int trueWidth = abs(windowWidth-rad);
        int trueHeight = abs(windowHeight-rad);                 // trueX, trueY для генерации шара
        ballPosX = get_rand(rad, trueWidth);              // ЦЕЛИКОМ в окне
        ballPosY = get_rand(rad, trueHeight);
        for (double angle = 0.0; angle < 100.0; angle+=1) {                    //рисуем шар + анимация движения по дуге окружности
            txClear();
            int rotationRad = get_rand(75, 100);
            int movedPosX = floor(rotationRad*cos(2*3.14*angle/360))+ballPosX;
            int movedPosY = floor(rotationRad*sin(2*3.14*angle/360))+ballPosY;
            drawOrientedBall(movedPosX, movedPosY,lightPosX, lightPosY, rad, ballColor);
            txSleep(0.001);
            }
        }

    return 0;
}

void drawRingyBackground(int X, int Y, int size) {
    drawColoredBall(X,Y, 2*size, 255, 255, 255);
}
void drawColoredBall(int x, int y, int r, int R,int G,int B)
{
for(int i = r; i>0; i--)
    {
        txSetColor(RGB(R-(200*i*R)/(r*255), G-(G*i*200)/(r*255), B-(B*i*200)/(r*255)));
        txSetFillColor(RGB(R-(200*i*R)/(r*255), G-(G*i*200)/(r*255), B-(B*i*200)/(r*255)));
        txCircle(x, y, i);
    }
}
void drawOrientedBall(int x, int y, int lX, int lY, int r, COLORREF rgb)
{
    const int startX = x;
    const int startY = y;
    for(int i = r; i>0; i--)
    {
        COLORREF defaults = txGetFillColor();
        unsigned char R = GetRValue(rgb);
        unsigned char G = GetGValue(rgb);
        unsigned char B = GetBValue(rgb);
        unsigned char darkerR = R-(R*i*230)/(r*255);
        unsigned char darkerG = G-(G*i*230)/(r*255);
        unsigned char darkerB = B-(B*i*230)/(r*255);
        COLORREF shadedColor = RGB(darkerR, darkerG, darkerB);
        txSetColor(shadedColor);
        txSetFillColor(shadedColor);
        double sinus = (lY-y)/sqrt((lX-x)*(lX-x)+(lY-y)*(lY-y));
        double cosinus = (lX-x)/sqrt((lX-x)*(lX-x)+(lY-y)*(lY-y));
        txCircle(x-(i/2)*cosinus, y-(i/2)*sinus, i);
        txSetFillColor (defaults);
    }
}
int get_rand(int min, int max) {
    return rand() % (max - min + 1) + min;
}

COLORREF getRandomRGB() {
    unsigned char colR = get_rand(50, 255);
    unsigned char colG = get_rand(50, 255);
    unsigned char colB = get_rand(50, 255);
    return RGB(colR, colG, colB);
    }

void taskWindow(int *szX, int *szY, int *num, int *lX, int *lY) {
    std::cout << "window size: ";                 //ввод размера окна
    std::cin >> *szX >> *szY;
    std::cout << "number of sweety balls: ";  //ввод количества шаров
    std::cin >> *num;
    std::cout << "light position: ";     //ввод координат источника света
    std::cin >> *lX >> *lY;
}
