#include "Shape.h"
#include "Circle.h"
#include "Context.h"
#include "Painter.h"
#include "math.h"

using namespace std;

Circle::Circle()
{
    //ctor
}

Circle::~Circle()
{
    //dtor
}

Circle::Circle(Point center, int radius, Color color) {
    this->center = center;
    this->radius = radius;
    this->color = color;
}

void Circle::draw(Painter& p) {

    p.drawCircle(
        this->center,
        this->radius,
        color);

    if (this->isFilled()){
        Color paintColor = fillColor.deSaturateColor(50);
        Color oldColor = p.getColorAt(this->center.getX(), this->center.getY());

        p.floodFill(
            this->center.getX(),
            this->center.getY(),
            paintColor,
            oldColor
        );
    }

    if (isSelected()) {
        p.drawCircle(this->center, this->radius + 3, Color(255, 0, 255));
        drawSelectionMarker(p,this->center);
    }
}

bool Circle::contains(Point p, int tolerance) {
    double dx = p.getX() - this->center.getX();
    double dy = p.getY() - this->center.getY();
    double dist = sqrt(dx * dx + dy * dy);

    // o circulo e preenchido (flood-fill), entao qualquer pixel
    // dentro do raio (+ tolerancia) faz parte do objeto
    return dist <= this->radius + tolerance;
}

void Circle::translate(int dx, int dy) {
    this->center.setX(this->center.getX() + dx);
    this->center.setY(this->center.getY() + dy);
}
