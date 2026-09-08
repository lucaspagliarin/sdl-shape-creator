#ifndef BEZIER_H
#define BEZIER_H
#include "Shape.h"
#include<SDL2/SDL.h>
#include <list>

using namespace std;

class Bezier : public Shape
{
    public:
        Bezier();
        Bezier(Point start, Point end, Point anchor1, Point anchor2, Color color);
        Bezier(list<Point> points, Color color);
        void draw() override;
        virtual ~Bezier();

    protected:

    private:
        list<Point> points;
        Point start;
        Point end;
        Point anchor1;
        Point anchor2;
        Color color;
};

#endif // BEZIER_H
