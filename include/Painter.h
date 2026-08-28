#ifndef PAINTER_H
#define PAINTER_H
#include <Point.h>
#include <Color.h>
#include<SDL2/SDL.h>

class Painter
{
    public:
        Painter();
        void setPixel(int x, int y, int r, int g, int b);
        void drawLine(Point start, Point end, Color color, int antialias);
        virtual ~Painter();

    protected:

    private:
        Color color;
        void setPixel(int x, int y, int r, int g, int b, int a);
        void setPixel(int x, int y, Color color);
        void setPixel(int x, int y, Uint32 cor);
        Uint32 getPixel(int x, int y);
        void bresenham(int x1, int y1, int x2, int y2, int r, int g, int b);
        void drawWuLine(int x0, int y0, int x1, int y1, Color color);

};

#endif // PAINTER_H
