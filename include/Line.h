#ifndef LINE_H
#define LINE_H
#include <Point.h>
#include <Color.h>
#include<SDL2/SDL.h>

class Line
{
    public:
        Line();
        Line(Point start, Point end, Color color);
        Line(Point start, Point end, Color color, int antialias);
        void draw();
        virtual ~Line();

    protected:

    private:
        Point start;
        Point end;
        Color color;
        int antialias = 0;
};

#endif // LINE_H
