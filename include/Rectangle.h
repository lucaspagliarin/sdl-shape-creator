#ifndef RECTANGLE_H
#define RECTANGLE_H
#include "Shape.h"
#include<SDL2/SDL.h>
#include <list>
#include <memory>
using namespace std;

class Rectangle : public Shape
{
    public:
        Rectangle();
        Rectangle(Point min, Point max, Color color);
        void draw(Painter& p) override;
        bool contains(Point p, int tolerance = 5, bool onlyBorderSelect = false) override;
        void translate(int dx, int dy) override;
        virtual ~Rectangle();

    protected:

    private:
        list<unique_ptr<Point>> points;
        Point min;
        Point max;
        Point calculateCentroid();
        Point calculateFillPoint();
};

#endif // RECTANGLE_H
