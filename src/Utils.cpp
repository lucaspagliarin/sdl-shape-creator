#include "Utils.h"
#include <cmath>

Utils * Utils::_instance = nullptr;

Utils::Utils()
{
    //ctor
}

Utils::~Utils()
{
    //dtor
}

Utils * Utils::getInstance()
{
    if(_instance == 0) {
        _instance = new Utils();
    }
    return _instance;
}

Point Utils::midPoint(Point p1, Point p2) {
    int newX = (p1.getX() + p2.getX()) / 2;
    int newY = (p1.getY() + p2.getY()) / 2;
    return Point(newX, newY);
}

double Utils::distance(Point a, Point b) {
    double dx = a.getX() - b.getX();
    double dy = a.getY() - b.getY();
    return sqrt(dx * dx + dy * dy);
}
