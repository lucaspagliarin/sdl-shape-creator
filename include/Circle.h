#ifndef CIRCLE_H
#define CIRCLE_H
#include "Shape.h"
#include<SDL2/SDL.h>
#include <list>

using namespace std;

class Circle : public Shape
{
    public:
        Circle();
        Circle(Point center, int radius, Color color);
        void draw(Painter& p) override;
        bool contains(Point p, int tolerance = 5) override;
        void translate(int dx, int dy) override;
        virtual ~Circle();

    protected:

    private:
        Point center;
        int radius;
        Color color;
};

#endif // CIRCLE_H
