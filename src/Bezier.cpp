#include "Shape.h"
#include "Bezier.h"
#include "Line.h"
#include "Context.h"
#include "Painter.h"
#include "math.h"

using namespace std;

Bezier::Bezier()
{
    //ctor
}

Bezier::~Bezier()
{
    //dtor
}

Bezier::Bezier(Point start, Point end, Point anchor1, Point anchor2, Color color) {
    this->start = start;
    this->end = end;
    this->anchor1 = anchor1;
    this->anchor2 = anchor2;
    this->points = {start, end, anchor1, anchor2};
    this->color = color;
}

Bezier::Bezier(list<Point> points, Color color) {
    this->points = points;
    this->start = points.front();
    points.pop_front();
    this->end = points.front();
    points.pop_front();
    this->anchor1 = points.front();
    points.pop_front();
    this->anchor2 = points.front();
    points.pop_front();
    this->color = color;
}

void Bezier::draw() {
    
    Painter p = Painter();

    p.drawBezier(
        this->start,
        this->end,
        this->anchor1,
        this->anchor2,
        color);

    
    p.drawCircle(this->start, 5, color);
    p.drawCircle(this->end, 5, color);
    p.drawCircle(this->anchor1, 5, color);
    p.drawCircle(this->anchor2, 5, color);

    p.drawLine(this->start, this->anchor1, color, 1);
    p.drawLine(this->end, this->anchor2, color, 1);
}
