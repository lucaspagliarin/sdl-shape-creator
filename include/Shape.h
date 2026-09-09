#ifndef SHAPE_H
#define SHAPE_H
#pragma once
#include <Point.h>
#include <Color.h>
#include<SDL2/SDL.h>
#include "Point.h"
#include "Painter.h"
#include "Transform.h"

class Shape
{
    public:
        Shape();
        virtual void draw(Painter& p);
        virtual bool contains(Point p, int tolerance = 5);
        virtual void translate(int dx, int dy);
        void setSelected(bool value);
        bool isSelected();
        void setFill(Color color, bool filled = true);
        bool isFilled();
        void drawSelectionMarker(Painter& painter, Point p);
        void setAngle(double angle);
        void setScale(double sx, double sy);
        void setPoint(int x, int y);
        void updateTransform(Point pivot);

        virtual ~Shape();

    protected:
        Point points;
        Color borderColor;
        Color fillColor;

        bool selected;
        bool filled;
        double angle;
        double scaleX,scaleY;
        Transform transform;

        double distancePointToSegment(Point p, Point a, Point b);
        void drawSelectionMarker(Point p);

    private:
};

#endif // SHAPE_H
