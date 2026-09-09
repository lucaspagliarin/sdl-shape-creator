#include "Shape.h"
#include "Bezier.h"
#include "Line.h"
#include "Context.h"
#include "Painter.h"
#include "math.h"

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

    p.drawBezier(
        this->start,
        this->end,
        this->anchor1,
        this->anchor2,
        color);


    p.drawCircle(this->start, 5, color);
    p.drawCircle(this->end, 5, color);
    p.drawCircle(this->anchor1, 5, color);
    p.drawCircle(this->anchor2, 5, color);

    p.drawLine(this->start, this->anchor1, color, 1);
    p.drawLine(this->end, this->anchor2, color, 1);
}

bool Bezier::contains(Point p, int tolerance) {
    double minDist = -1;

    // amostra a curva com a mesma equacao parametrica usada para desenha-la
    for (float u = 0.0f; u <= 1.0f; u += 0.01f) {
        double xu = pow((1 - u), 3) * start.getX()
                  + 3 * u * pow(1 - u, 2) * anchor1.getX()
                  + 3 * pow(u, 2) * (1 - u) * anchor2.getX()
                  + pow(u, 3) * end.getX();

        double yu = pow((1 - u), 3) * start.getY()
                  + 3 * u * pow(1 - u, 2) * anchor1.getY()
                  + 3 * pow(u, 2) * (1 - u) * anchor2.getY()
                  + pow(u, 3) * end.getY();

        double dx = p.getX() - xu;
        double dy = p.getY() - yu;
        double d = sqrt(dx * dx + dy * dy);

        if (minDist < 0 || d < minDist) minDist = d;
    }

    return minDist >= 0 && minDist <= tolerance;
}

void Bezier::translate(int dx, int dy) {
    this->start.setX(this->start.getX() + dx);
    this->start.setY(this->start.getY() + dy);
    this->end.setX(this->end.getX() + dx);
    this->end.setY(this->end.getY() + dy);
    this->anchor1.setX(this->anchor1.getX() + dx);
    this->anchor1.setY(this->anchor1.getY() + dy);
    this->anchor2.setX(this->anchor2.getX() + dx);
    this->anchor2.setY(this->anchor2.getY() + dy);
}
