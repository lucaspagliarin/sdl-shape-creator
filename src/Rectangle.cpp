#include "Shape.h"
#include "Rectangle.h"
#include "Line.h"
#include "Context.h"
#include "Painter.h"
#include "Utils.h"
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

void Rectangle::draw() {
    
    Painter p = Painter();

    p.drawRectangle(
        this->min.getX(), 
        this->min.getY(), 
        this->max.getX(), 
        this->max.getY(), 
        borderColor);

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
        drawSelectionMarker(Point(min.getX(), min.getY()));
        drawSelectionMarker(Point(max.getX(), min.getY()));
        drawSelectionMarker(Point(max.getX(), max.getY()));
        drawSelectionMarker(Point(min.getX(), max.getY()));
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
