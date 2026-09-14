#include "Shape.h"
#include "Line.h"
#include "Context.h"
#include "Painter.h"
#include "math.h"
#include "Utils.h"
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

void Line::draw(Painter& p) {
    this->updateTransform(Utils::midPoint(this->start,this->end));
    p.drawLine(
        Point(transform.apply(this->start)),
        Point(transform.apply(this->end)),
        this->color,
        this->antialias);
        if(isSelected())
        {
          drawSelectionMarker(p,Point(transform.apply(this->start)));
          drawSelectionMarker(p,Point(transform.apply(this->end)));
        }
}

bool Line::contains(Point p, int tolerance, bool onlyBorderSelect) {
    return distancePointToSegment(p, this->transform.apply(this->start), this->transform.apply(this->end)) <= tolerance;
}

void Line::translate(int dx, int dy) {
   int currentX = this->position.getX() + dx;
    int currentY = this->position.getY() + dy;
    this->setPoint(currentX, currentY);
}
