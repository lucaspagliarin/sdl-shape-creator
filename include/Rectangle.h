#ifndef RECTANGLE_H
#define RECTANGLE_H
#include "Shape.h"
#include<SDL2/SDL.h>
#include <list>

using namespace std;

class Rectangle : public Shape
{
    public:
        Rectangle();
        Rectangle(Point min, Point max, Color color);
        void draw() override;
        bool contains(Point p, int tolerance = 5) override;
        void translate(int dx, int dy) override;
        virtual ~Rectangle();

    protected:

    private:
        list<unique_ptr<Point>> points;
        Point min;
        Point max;
        Point calculateCentroid();
};

#endif // RECTANGLE_H
