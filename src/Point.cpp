#include "Point.h"

Point::Point()
{
    //ctor
}

Point::Point(int x, int y)
{
    this->x = x;
    this->y = y;
}

Point::~Point()
{
    //dtor
}


int Point::getX()const
{
    return this->x;
}

int Point::getY()const
{
    return this->y;
}

void Point::setX(int x)
{
    this->x = x;
}

void Point::setY(int y)
{
    this->y = y;
}

