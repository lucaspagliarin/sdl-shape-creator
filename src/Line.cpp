#include "Shape.h"
#include "Line.h"
#include "Context.h"
#include "Painter.h"
#include "math.h"

Line::Line()
{
    //ctor
}

Line::~Line()
{
    //dtor
}

Line::Line(Point start, Point end, Color color) {
    this->start = start;
    this->end = end;
    this->color = color;
    this->antialias = 0;
}

Line::Line(Point start, Point end, Color color, int antialias) {
    this->start = start;
    this->end = end;
    this->color = color;
    this->antialias = antialias;
}

void Line::draw() {
    Painter p = Painter();
    p.drawLine(
        this->start,
        this->end,
        this->color,
        this->antialias);
}

bool Line::contains(Point p, int tolerance) {
    return distancePointToSegment(p, this->start, this->end) <= tolerance;
}

void Line::translate(int dx, int dy) {
    this->start.setX(this->start.getX() + dx);
    this->start.setY(this->start.getY() + dy);
    this->end.setX(this->end.getX() + dx);
    this->end.setY(this->end.getY() + dy);
}
