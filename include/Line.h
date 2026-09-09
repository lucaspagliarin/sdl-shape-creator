#ifndef LINE_H
#define LINE_H
#include "Shape.h"
#include<SDL2/SDL.h>

class Line : public Shape
{
    public:
        Line();
        Line(Point start, Point end, Color color);
        Line(Point start, Point end, Color color, int antialias);
        void draw() override;
        bool contains(Point p, int tolerance = 5) override;
        void translate(int dx, int dy) override;
        virtual ~Line();

    protected:

    private:
        Point start;
        Point end;
        Color color;
        int antialias = 0;
};

#endif // LINE_H
