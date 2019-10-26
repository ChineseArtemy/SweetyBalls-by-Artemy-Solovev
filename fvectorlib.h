#ifndef FVECTORLIB_H
#define FVECTORLIB_H
#include <cmath>
struct ffVector;

float getX (ffVector v);

float getY (ffVector v);

ffVector plus(ffVector v1, ffVector v2);

ffVector minus(ffVector v1, ffVector v2);

ffVector mult(ffVector v, float k);

ffVector div(ffVector v, float k);

float len(ffVector v);

float dotProd(ffVector v1, ffVector v2);

float angle(ffVector v1, ffVector v2);

ffVector normalize(ffVector v);

#endif

struct ffVector
{
    float x = 0;
    float y = 0;

    float len()
    {
        return sqrt(this->x*this->x + this->y*this->y);
    }
    ffVector()
    {
    }
    ffVector(float x, float y)
    {
        this->x = x;
        this->y = y;
    }

    ffVector normalize();

    ffVector operator+(ffVector v2);

    ffVector operator-(ffVector v2);

    ffVector operator*(float k);

    ffVector operator/(float k);

};

ffVector ffVector::operator+(ffVector v2)
{
    return ffVector(x+v2.x, y+v2.y);
}

ffVector ffVector::operator-(ffVector v2)
{
    ffVector res;
    res.x = x - v2.x;
    res.y = y - v2.y;
    return res;
}

ffVector ffVector::operator*(float k)
{
    ffVector res;
    res.x = k * x;
    res.y = k * y;
    return res;
}

ffVector ffVector::operator/(float k)
{
    ffVector res;
    res.x = x / k;
    res.y = y / k;
    return res;
}

float dotProd(ffVector v1, ffVector v2)
{
    return v1.x * v2.x + v2.y * v2.y;
}

float len(ffVector v)
{
    return sqrt(dotProd(v,v));
}

float angle(ffVector v1, ffVector v2)
{
    if ((len(v1) > 0) and (len(v2) > 0))
    {
        return acos(dotProd(v1,v2) / (len(v1) * len(v2)));
    }
    else
    {
        return 0;
    }
}

ffVector ffVector::normalize()
{
    ffVector res;
    res.x = this->x / this->len();
    res.y = this->y / this->len();
    return res;
}
