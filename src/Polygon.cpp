#include "Shape.h"
#include "Polygon.h"
#include "Line.h"
#include "Context.h"
#include "Painter.h"
#include "math.h"

using namespace std;

Polygon::Polygon()
{
    //ctor
}

Polygon::~Polygon()
{
    //dtor
}

Polygon::Polygon(list<Point> points, Color color) {
    this->points = points;
    this->color = color;
}

void Polygon::draw() {
    
    Painter p = Painter();

    p.drawPolygon(
        this->points, 
        color);
}
