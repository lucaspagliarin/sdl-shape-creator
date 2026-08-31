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
