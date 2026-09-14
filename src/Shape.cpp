#include "Shape.h"
#include "Context.h"
#include "Painter.h"
#include "math.h"
#include <iostream>

Shape::Shape()
{
    selected = false;
    filled = false;
    this->scaleX=1.0f;
    this->scaleY=1.0f;
    this->angle = 0.0f;
    this->position = Point(0,0);
}

Shape::~Shape()
{
    //dtor
}

void Shape::draw(Painter& p){
    // override
}

bool Shape::contains(Point p, int tolerance, bool onlyBorderSelect) {

    return false;
}

void Shape::translate(int dx, int dy) {

}

void Shape::setSelected(bool value) {
    selected = value;
}

bool Shape::isSelected() {
    return selected;
}

void Shape::setFill(Color color, bool filled){
    fillColor = color;
    this->filled = filled;
}

bool Shape::isFilled() {
    return filled;
}

double Shape::distancePointToSegment(Point p, Point a, Point b) {
    double px = p.getX(), py = p.getY();
    double ax = a.getX(), ay = a.getY();
    double bx = b.getX(), by = b.getY();

    double dx = bx - ax;
    double dy = by - ay;
    double lengthSq = dx * dx + dy * dy;

    double t = (lengthSq == 0) ? 0.0 : ((px - ax) * dx + (py - ay) * dy) / lengthSq;
    if (t < 0) t = 0;
    if (t > 1) t = 1;

    double closestX = ax + t * dx;
    double closestY = ay + t * dy;

    double distX = px - closestX;
    double distY = py - closestY;

    return sqrt(distX * distX + distY * distY);
}

void Shape::drawSelectionMarker(Painter& painter, Point p) {
    int halfSize = 4; //

    Point p1(p.getX() - halfSize, p.getY() - halfSize);
    Point p2(p.getX() + halfSize, p.getY() - halfSize);
    Point p3(p.getX() + halfSize, p.getY() + halfSize);
    Point p4(p.getX() - halfSize, p.getY() + halfSize);

    painter.drawRectangle(p1, p2, p3, p4, Color(255, 0, 255));
}
void Shape::setAngle(double angle)
{
    this->angle = angle;
}
void Shape::setScale(double sx, double sy)
{
    this->scaleX = sx;
    this->scaleY = sy;
}
void Shape::setPoint(int x, int y)
{
    this->position= Point(x,y);
}
void Shape::updateTransform(Point pivot) {

    transform.setRotation(this->angle);
    transform.setScale(this->scaleX, this->scaleY);

    transform.computeFinalMatrix(pivot, this->position);
}
double Shape::getScaleX()
{
    return this->scaleX;
}
double Shape::getScaleY()
{
    return this->scaleY;
}
double Shape::getAngle()
{
    return this->angle;
}
