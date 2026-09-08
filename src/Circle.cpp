#include "Shape.h"
#include "Circle.h"
#include "Context.h"
#include "Painter.h"
#include "math.h"

using namespace std;

Circle::Circle()
{
    //ctor
}

Circle::~Circle()
{
    //dtor
}

Circle::Circle(Point center, int radius, Color color) {
    this->center = center;
    this->radius = radius;
    this->color = color;
}

void Circle::draw() {
    
    Painter p = Painter();

    p.drawCircle(
        this->center,
        this->radius,
        color);

    p.floodFill(
        this->center.getX(),
        this->center.getY(),
        color,
        Color(255,255,255)
    );
}
