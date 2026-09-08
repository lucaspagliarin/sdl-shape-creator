#include "Shape.h"
#include "Context.h"
#include "Painter.h"
#include "math.h"
#include <iostream>

Shape::Shape()
{
    //ctor
}

Shape::~Shape()
{
    //dtor
}

void Shape::draw(){
    // override
    std::cout << "Drawing..." << std::endl;
}
