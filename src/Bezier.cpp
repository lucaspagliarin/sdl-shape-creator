#include "Shape.h"
#include "Bezier.h"
#include "Line.h"
#include "Context.h"
#include "Painter.h"
#include "math.h"
#include "Utils.h"

using namespace std;

Bezier::Bezier()
{
    //ctor
}

Bezier::~Bezier()
{
    //dtor
}

Bezier::Bezier(Point start, Point end, Point anchor1, Point anchor2, Color color) {
    this->start = start;
    this->end = end;
    this->anchor1 = anchor1;
    this->anchor2 = anchor2;
    this->points = {start, end, anchor1, anchor2};
    this->color = color;
}

Bezier::Bezier(list<Point> points, Color color) {
    this->points = points;
    this->start = points.front();
    points.pop_front();
    this->end = points.front();
    points.pop_front();
    this->anchor1 = points.front();
    points.pop_front();
    this->anchor2 = points.front();
    points.pop_front();
    this->color = color;
}

void Bezier::draw(Painter& p) {
    //cria pivo para rotação e escala
   Point pivo = Utils::midPoint(this->start,this->end);
   //atualiza matriz
   this->updateTransform(pivo);
    Point tempStart=this->transform.apply(this->start);
    Point tempEnd=this->transform.apply(this->end);
    Point tempArchor1=this->transform.apply(this->anchor1);
    Point tempArchor2=this->transform.apply(this->anchor2);

    p.drawBezier(
        tempStart,
        tempEnd,
        tempArchor1,
        tempArchor2,
        color);


    if (isSelected()) {

        p.drawCircle(tempStart, 5, color.deSaturateColor(-100));
        p.drawCircle(tempEnd, 5, color.deSaturateColor(-100));
        p.drawCircle(tempArchor1, 5, color.deSaturateColor(-100));
        p.drawCircle(tempArchor2, 5, color.deSaturateColor(-100));

        p.drawLine(tempStart, tempArchor1, color.deSaturateColor(-100), 1);
        p.drawLine(tempEnd, tempArchor2, color.deSaturateColor(-100), 1);

    }
}

bool Bezier::contains(Point p, int tolerance, bool onlyBorderSelect) {
    double minDist = -1;


    for (float u = 0.0f; u <= 1.0f; u += 0.01f) {
        double xu = pow((1 - u), 3) * this->transform.apply(start).getX()
                  + 3 * u * pow(1 - u, 2) * this->transform.apply(anchor1).getX()
                  + 3 * pow(u, 2) * (1 - u) * this->transform.apply(anchor2).getX()
                  + pow(u, 3) * this->transform.apply(end).getX();

        double yu = pow((1 - u), 3) * this->transform.apply(start).getY()
                  + 3 * u * pow(1 - u, 2) * this->transform.apply(anchor1).getY()
                  + 3 * pow(u, 2) * (1 - u) * this->transform.apply(anchor2).getY()
                  + pow(u, 3) * this->transform.apply(end).getY();

        double dx = p.getX() - xu;
        double dy = p.getY() - yu;
        double d = sqrt(dx * dx + dy * dy);

        if (minDist < 0 || d < minDist) minDist = d;
    }

    return minDist >= 0 && minDist <= tolerance;
}

void Bezier::translate(int dx, int dy) {
    //atualiza o deslocamento x e y
    int currentX = this->position.getX() + dx;
    int currentY = this->position.getY() + dy;
    this->setPoint(currentX, currentY);
}
