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
        virtual ~Shape();

    protected:
        Point points;
        Color color;

    private:
};

#endif // SHAPE_H
