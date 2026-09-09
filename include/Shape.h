#ifndef SHAPE_H
#define SHAPE_H
#pragma once
#include <Point.h>
#include <Color.h>
#include<SDL2/SDL.h>

class Shape
{
    public:
        Shape();
        virtual void draw();
        virtual bool contains(Point p, int tolerance = 5);
        virtual void translate(int dx, int dy);
        void setSelected(bool value);
        bool isSelected();
        void setFill(Color color, bool filled = true);
        bool isFilled();
        
        virtual ~Shape();

    protected:
        Point points;
        Color borderColor;
        Color fillColor;
        bool selected;
        bool filled;

        double distancePointToSegment(Point p, Point a, Point b);

        // desenha um pequeno marcador (handle) num ponto, usado para
        // indicar visualmente que o objeto esta selecionado
        void drawSelectionMarker(Point p);

    private:
};

#endif // SHAPE_H
