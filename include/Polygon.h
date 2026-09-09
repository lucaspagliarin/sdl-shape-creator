#ifndef POLYGON_H
#define POLYGON_H
#include "Shape.h"
#include<SDL2/SDL.h>
#include <list>

using namespace std;

class Polygon : public Shape
{
    public:
        Polygon();
        Polygon(list<Point> points, Color color);
        void draw(Painter& p) override;
        bool contains(Point p, int tolerance = 5) override;
        void translate(int dx, int dy) override;
        virtual ~Polygon();

    protected:

    private:
        list<Point> points;
        Color color;
        Point calculateCentroid();
};

#endif // POLYGON_H
