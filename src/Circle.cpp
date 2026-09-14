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
    this->updateTransform(this->center);
    int raioTransformado = (int)(this->radius * this->getScaleX());
    p.drawCircle(
        this->transform.apply(this->center),
        raioTransformado,
        color);

    if (this->isFilled()) {
        Point c = this->transform.apply(this->center);
        p.fillCircle(c, raioTransformado, fillColor.deSaturateColor(50));
    }

    if (isSelected()) {
        p.drawCircle(this->transform.apply(this->center), raioTransformado+ 3, Color(255, 0, 255));
        drawSelectionMarker(p,this->transform.apply(this->center));
    }
}

bool Circle::contains(Point p, int tolerance, bool onlyBorderSelect) {
    double dx = p.getX() - this->transform.apply(this->center).getX();
    double dy = p.getY() - this->transform.apply(this->center).getY();
    double dist = sqrt(dx * dx + dy * dy);


    if (onlyBorderSelect && !isFilled()){

        return dist <= ((int)(this->radius * this->getScaleX())) + tolerance &&
                dist >= ((int)(this->radius * this->getScaleX())) - tolerance;

    } else {
        return dist <= ((int)(this->radius * this->getScaleX())) + tolerance;
    }

}

void Circle::translate(int dx, int dy) {
    int currentX = this->position.getX() + dx;
    int currentY = this->position.getY() + dy;
    this->setPoint(currentX, currentY);
}
