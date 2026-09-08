#include "Shape.h"
#include "Rectangle.h"
#include "Line.h"
#include "Context.h"
#include "Painter.h"
#include "math.h"

using namespace std;

Rectangle::Rectangle()
{
    //ctor
}

Rectangle::~Rectangle()
{
    //dtor
}

Rectangle::Rectangle(Point min, Point max, Color color) {
    this->min = min;
    this->max = max;
    this->color = color;
}

void Rectangle::draw() {
    
    Painter p = Painter();

    p.drawRectangle(
        this->min.getX(), 
        this->min.getY(), 
        this->max.getX(), 
        this->max.getY(), 
        color);
}
