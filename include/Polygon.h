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
        void draw() override;
        virtual ~Polygon();

    protected:

    private:
        list<Point> points;
        Color color;
};

#endif // POLYGON_H
