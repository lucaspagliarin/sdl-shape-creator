#ifndef PAINTER_H
#define PAINTER_H
#include <Point.h>
#include <Color.h>
#include<SDL2/SDL.h>
#include <list>

using namespace std;

class Painter
{
    public:
        Painter();
        void setPixel(int x, int y, int r, int g, int b);
        void drawLine(Point start, Point end, Color color, int antialias);
        void drawCircle(Point center, int radius, Color color);
        void drawRectangle(int x1, int y1, int x2, int y2, Color color);
        void drawPolygon(list<Point> points, Color cor);
        void drawBezier(Point start, Point end, Point anchor1, Point anchor2, Color color);
        void floodFill(int x, int y, Color newColor, Color oldColor);
        virtual ~Painter();

    protected:

    private:
        Color color;
        void setPixel(int x, int y, int r, int g, int b, int a);
        void setPixel(int x, int y, Color color);
        void setPixel(int x, int y, Uint32 cor);
        Uint32 getPixel(int x, int y);

        // Lines
        void bresenham(int x1, int y1, int x2, int y2, int r, int g, int b);
        void drawWuLine(int x0, int y0, int x1, int y1, Color color);

        // Circle
        void displayBresenhamCircle(int xc, int yc, int x, int y, Color color);

};

#endif // PAINTER_H
