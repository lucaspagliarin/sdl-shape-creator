#include "Shape.h"
#include "Rectangle.h"
#include "Line.h"
#include "Context.h"
#include "Painter.h"
#include "Utils.h"
#include "math.h"
#include <cstdio>

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
    if (min.getX() < max.getX()){
        this->min = min;
        this->max = max;
    } else {
        this->max = min;
        this->min = max;
    }

    this->borderColor = color;
}

Point Rectangle::calculateCentroid(){
    int cx = (this->min.getX() + this->max.getX()) / 2.0;
    int cy = (this->min.getY() + this->max.getY()) / 2.0;
    return Point{cx, cy};
}

Point Rectangle::calculateFillPoint(){
    Point t1 = transform.apply(this->min);
    Point t2 = transform.apply(this->max);
    
    int cx = (t1.getX() + t2.getX()) / 2.0;
    int cy = (t1.getY() + t2.getY()) / 2.0;
    return Point{cx, cy};
}

void Rectangle::draw(Painter& p) {

    Point pivo = calculateCentroid();
    this->updateTransform(pivo);
    Point t1 = transform.apply(this->min);
    Point t2 = transform.apply(Point(this->max.getX(), this->min.getY()));
    Point t3 = transform.apply(this->max);
    Point t4 = transform.apply(Point(this->min.getX(), this->max.getY()));

    p.drawRectangle(t1, t2, t3, t4, borderColor);

    if (this->isFilled()) {

        Color paintColor = fillColor.deSaturateColor(50);

        Color oldColor = p.getColorAt(pivo.getX(), pivo.getY());

        Point inner1 = Utils::getInstance()->midPoint(min, pivo);
        Point inner2 = Utils::getInstance()->midPoint(max, pivo);

        p.floodFill(calculateFillPoint().getX(), calculateFillPoint().getY(), paintColor, oldColor);
        // p.floodFill(inner1.getX(), inner1.getY(), paintColor, oldColor);
        // p.floodFill(inner2.getX(), inner2.getY(), paintColor, oldColor);
    }

    if (isSelected()) {
        drawSelectionMarker(p,Point(t1.getX(), t1.getY()));
        drawSelectionMarker(p,Point(t2.getX(), t2.getY()));
        drawSelectionMarker(p,Point(t3.getX(), t3.getY()));
        drawSelectionMarker(p,Point(t4.getX(), t4.getY()));
    }
}

bool Rectangle::contains(Point p, int tolerance, bool onlyBorderSelect) {

    Point pivo = calculateCentroid();

    this->updateTransform(pivo);

    Point topLeft = transform.apply(this->min);
    Point topRight = transform.apply(Point(this->max.getX(), this->min.getY()));
    Point bottomRight = transform.apply(this->max);
    Point bottomLeft = transform.apply(Point(this->min.getX(), this->max.getY()));

    if (onlyBorderSelect && !isFilled()) {
        double d1 = distancePointToSegment(p, topLeft, topRight);
        double d2 = distancePointToSegment(p, topRight, bottomRight);
        double d3 = distancePointToSegment(p, bottomRight, bottomLeft);
        double d4 = distancePointToSegment(p, bottomLeft, topLeft);

        double minDist = d1;
        if (d2 < minDist) minDist = d2;
        if (d3 < minDist) minDist = d3;
        if (d4 < minDist) minDist = d4;
        
        return minDist <= tolerance;
    } else {
        if (p.getX() >= topLeft.getX() && p.getX() <= bottomRight.getX() &&
            p.getY() <= bottomRight.getY() && p.getY() >= topLeft.getY()) {
            return true;
        }
        return false;
    }

    
}

void Rectangle::translate(int dx, int dy) {

    int currentX = this->position.getX() + dx;
    int currentY = this->position.getY() + dy;
    this->setPoint(currentX, currentY);
}
