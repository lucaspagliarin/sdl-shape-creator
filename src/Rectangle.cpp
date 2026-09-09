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

void Rectangle::draw(Painter& p) {
    int centerX = (min.getX() + max.getX()) / 2;
    int centerY = (min.getY() + max.getY()) / 2;
    Point pivo(centerX, centerY);
    this->updateTransform(pivo);
    Point t1 = transform.apply(this->min);
    Point t2 = transform.apply(Point(this->max.getX(), this->min.getY()));
    Point t3 = transform.apply(this->max);
    Point t4 = transform.apply(Point(this->min.getX(), this->max.getY()));

    p.drawRectangle(t1, t2, t3, t4, borderColor);
    printf("Original min:(%d,%d) -> Transformado t1:(%d,%d)\n", min.getX(), min.getY(), t1.getX(), t1.getY());

    if (this->isFilled()) {
        // Ajustando a cor caso seja igual a da borda.
        Color paintColor = fillColor.deSaturateColor(50);

        Point centroid = calculateCentroid();

        Color oldColor = p.getColorAt(centroid.getX(), centroid.getY());

        Point inner1 = Utils::getInstance()->midPoint(min, centroid);
        Point inner2 = Utils::getInstance()->midPoint(max, centroid);

        p.floodFill(centroid.getX(), centroid.getY(), paintColor, oldColor);
        p.floodFill(inner1.getX(), inner1.getY(), paintColor, oldColor);
        p.floodFill(inner2.getX(), inner2.getY(), paintColor, oldColor);
    }

    if (isSelected()) {
        drawSelectionMarker(p,Point(min.getX(), min.getY()));
        drawSelectionMarker(p,Point(max.getX(), min.getY()));
        drawSelectionMarker(p,Point(max.getX(), max.getY()));
        drawSelectionMarker(p,Point(min.getX(), max.getY()));
    }
}

bool Rectangle::contains(Point p, int tolerance) {
    Point topLeft(min.getX(), min.getY());
    Point topRight(max.getX(), min.getY());
    Point bottomRight(max.getX(), max.getY());
    Point bottomLeft(min.getX(), max.getY());

    double d1 = distancePointToSegment(p, topLeft, topRight);
    double d2 = distancePointToSegment(p, topRight, bottomRight);
    double d3 = distancePointToSegment(p, bottomRight, bottomLeft);
    double d4 = distancePointToSegment(p, bottomLeft, topLeft);

    double minDist = d1;
    if (d2 < minDist) minDist = d2;
    if (d3 < minDist) minDist = d3;
    if (d4 < minDist) minDist = d4;

    return minDist <= tolerance;
}

void Rectangle::translate(int dx, int dy) {
    this->min.setX(this->min.getX() + dx);
    this->min.setY(this->min.getY() + dy);
    this->max.setX(this->max.getX() + dx);
    this->max.setY(this->max.getY() + dy);
}
