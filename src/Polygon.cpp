#include "Shape.h"
#include "Polygon.h"
#include "Line.h"
#include "Context.h"
#include "Painter.h"
#include "Utils.h"
#include "math.h"
#include <list>
#include <iostream>

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
        // Aplica a matriz de transforma��o no ponto atual
        Point ptTransformado = this->transform.apply(pt);

        // Coloca o novo ponto transformado na nova lista
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

    p.drawCircle(centroidatual, 5, Color(255,0,0));

    if (isSelected()) {
        for (Point& pt : pontosDraw) {
            drawSelectionMarker(p,pt);
        }
}}

bool Polygon::contains(Point p, int tolerance) {
    if (this->points.size() < 2) return false;

   this->updateTransform(this->initialCentroid);

    list<Point> pontosDraw;
    for (const Point& pt : this->points) {
        Point ptTransformado = this->transform.apply(pt);
        pontosDraw.push_back(ptTransformado);
    }

    double minDist = -1;
    Point previous = pontosDraw.back();

    for (const Point& current : pontosDraw) {
        double d = distancePointToSegment(p, previous, current);
        if (minDist < 0 || d < minDist) minDist = d;
        previous = current;
    }

    return minDist >= 0 && minDist <= tolerance;
}


void Polygon::translate(int dx, int dy) {
    int currentX = this->position.getX() + dx;
    int currentY = this->position.getY() + dy;
    this->setPoint(currentX, currentY);
}

Point Polygon::calculateCentroid() {
    size_t n = this->points.size();

    // A valid polygon needs at least 3 vertices
    if (n < 3) {
        return Point{0, 0}; // Or handle according to your Point constructor
    }

    int cx = 0;
    int cy = 0;
    double signedArea = 0.0;

    // Use iterators to traverse the std::list
    auto current = this->points.begin();

    for (size_t i = 0; i < n; ++i) {
        // Get the next iterator, wrap around to the beginning if at the end
        auto next = current;
        std::advance(next, 1);
        if (next == this->points.end()) {
            next = this->points.begin();
        }

        double x0 = current->getX(); // Assuming Point has public fields x and y
        double y0 = current->getY();
        double x1 = next->getX();
        double y1 = next->getY();


        // Shoelace formula component
        double a = (x0 * y1) - (x1 * y0);
        signedArea += a;

        cx += (x0 + x1) * a;
        cy += (y0 + y1) * a;

        // Move to the next point in the list
        ++current;
    }

    signedArea *= 0.5;


    // Check to avoid division by zero if the polygon is a straight line
    if (std::abs(signedArea) < 1e-9) {
        return Point{0, 0};
    }

    cx /= (6.0 * signedArea);
    cy /= (6.0 * signedArea);

    // Return the calculated center as a new Point object
    return Point{cx, cy};
}
