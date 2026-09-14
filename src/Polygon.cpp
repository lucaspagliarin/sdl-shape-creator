#include "Shape.h"
#include "Polygon.h"
#include "Line.h"
#include "Context.h"
#include "Painter.h"
#include "Utils.h"
#include "math.h"
#include <list>
#include <iostream>
#include <vector>

using namespace std;

Polygon::Polygon()
{
    //ctor
}

Polygon::~Polygon()
{
    //dtor
}

Polygon::Polygon(list<Point> points, Color color) {
    this->points = points;
    this->color = color;
    this->initialCentroid = this->calculateCentroid();
}

void Polygon::draw(Painter& p) {

    this->updateTransform(this->initialCentroid);

    list<Point> pontosDraw;
  for (const Point& pt : this->points) {

        Point ptTransformado = this->transform.apply(pt);
        pontosDraw.push_back(ptTransformado);
    }
    Point centroidatual = this->transform.apply(this->initialCentroid);
    p.drawPolygon(
        pontosDraw,
        this->color);

    if (this->isFilled()) {
        Color paintColor = fillColor.deSaturateColor(50);

        Color oldColor = p.getColorAt(centroidatual.getX(), centroidatual.getY());

        p.floodFill(centroidatual.getX(), centroidatual.getY(), paintColor, oldColor);

    }



    if (isSelected()) {
        for (Point& pt : pontosDraw) {
            drawSelectionMarker(p,pt);
        }
        p.drawCircle(centroidatual, 5, Color(255,0,255));
}}

bool Polygon::contains(Point p, int tolerance, bool onlyBorderSelect) {
    if (this->points.size() < 2) return false;

    this->updateTransform(this->initialCentroid);

    vector<Point> pontosDraw;
    for (const Point& pt : this->points) {
        pontosDraw.push_back(this->transform.apply(pt));
    }

    if (onlyBorderSelect && !isFilled()) {
        double minDist = -1;
        Point previous = pontosDraw.back();

        for (const Point& current : pontosDraw) {
            double d = distancePointToSegment(p, previous, current);
            if (minDist < 0 || d < minDist) minDist = d;
            previous = current;
        }

        return minDist >= 0 && minDist <= tolerance;
    } else {

        bool inside = false;
        size_t n = pontosDraw.size();

        for (size_t i = 0, j = n - 1; i < n; j = i++) {
            double xi = pontosDraw[i].getX(), yi = pontosDraw[i].getY();
            double xj = pontosDraw[j].getX(), yj = pontosDraw[j].getY();

            bool intersect = ((yi > p.getY()) != (yj > p.getY())) &&
                              (p.getX() < (xj - xi) * (p.getY() - yi) / (yj - yi) + xi);

            if (intersect) inside = !inside;
        }

        return inside;
    }
}


void Polygon::translate(int dx, int dy) {
    int currentX = this->position.getX() + dx;
    int currentY = this->position.getY() + dy;
    this->setPoint(currentX, currentY);
}

Point Polygon::calculateCentroid() {
    size_t n = this->points.size();

    if (n < 3) {
        return Point{0, 0};
    }

    int cx = 0;
    int cy = 0;
    double signedArea = 0.0;

    auto current = this->points.begin();

    for (size_t i = 0; i < n; ++i) {

        auto next = current;
        std::advance(next, 1);
        if (next == this->points.end()) {
            next = this->points.begin();
        }

        double x0 = current->getX();
        double y0 = current->getY();
        double x1 = next->getX();
        double y1 = next->getY();

        double a = (x0 * y1) - (x1 * y0);
        signedArea += a;

        cx += (x0 + x1) * a;
        cy += (y0 + y1) * a;

        ++current;
    }

    signedArea *= 0.5;

    if (std::abs(signedArea) < 1e-9) {
        return Point{0, 0};
    }

    cx /= (6.0 * signedArea);
    cy /= (6.0 * signedArea);

    return Point{cx, cy};
}
