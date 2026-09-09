#include "Shape.h"
#include "Context.h"
#include "Painter.h"
#include "math.h"
#include <iostream>

Shape::Shape()
{
    selected = false;
    filled = false;
}

Shape::~Shape()
{
    //dtor
}

void Shape::draw(){
    // override
    std::cout << "Desenhando..." << std::endl;
}

bool Shape::contains(Point p, int tolerance) {
    // subclasses devem sobrescrever com o hit-test geometrico correto
    return false;
}

void Shape::translate(int dx, int dy) {
    // subclasses devem sobrescrever movendo seus pontos internos
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

void Shape::drawSelectionMarker(Point p) {
    Painter painter = Painter();
    painter.drawRectangle(p.getX() - 4, p.getY() - 4, p.getX() + 4, p.getY() + 4, Color(255, 0, 255));
}