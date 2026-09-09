#include "Color.h"
#include "Context.h"

Color::Color()
{
    //ctor
}


Color::Color(Uint8 r, Uint8 g, Uint8 b, Uint8 a)
{
    this->r = r;
    this->g = g;
    this->b = b;
    this->a = a;
}

Color::Color(Uint8 r, Uint8 g, Uint8 b)
{
    this->r = r;
    this->g = g;
    this->b = b;
    this->a = 255;
}

Color::Color(Uint32 rgb)
{
    this->r = getColorComponent(rgb, r);
    this->g = getColorComponent(rgb, g);
    this->b = getColorComponent(rgb, b);
    this->a = 255;
}

Color::~Color()
{
    //dtor
}

Uint32 Color::getColor(int r, int g, int b) {
    return RGB(r, g, b);
}

Uint32 Color::getColor() {
    return RGB(this->r, this->g, this->b);
}

Uint8 Color::getR()
{
    return this->r;
}
Uint8 Color::getG()
{
    return this->g;
}
Uint8 Color::getB()
{
    return this->b;
}

Uint32 Color::RGB(int r, int g, int b) {

    SDL_Surface * surface = Context::getInstance()->getWindowSurface();

    return SDL_MapRGBA(surface->format, r, g, b, 255);
}

Uint8 Color::getColorComponent( Uint32 pixel, char component ) {

    Uint32 mask;

    switch(component) {
        case 'b' :
        case 'B' :
            mask = RGB(0,0,255);
            pixel = pixel & mask;
            break;
        case 'r' :
        case 'R' :
            mask = RGB(255,0,0);
            pixel = pixel & mask;
            pixel = pixel >> 16;
            break;
        case 'g' :
        case 'G' :
            mask = RGB(0,255,0);
            pixel = pixel & mask;
            pixel = pixel >> 8;
            break;
    }
    return (Uint8) pixel;
}

Color Color::deSaturateColor(int amount) {
    // 1. Clamp the input amount between 0 and 255 to prevent unexpected behavior
    if (amount < 0) amount = 0;
    if (amount > 255) amount = 255;

    // 2. Calculate the grayscale intensity (perceived luminance)
    int gray = (int)(0.2126f * this->r + 0.7152f * this->g + 0.0722f * this->b);

    // 3. Linearly interpolate using integer math, then shift right by 8 bits (divide by 256)
    // Formula: original + (amount * (gray - original)) / 255
    Uint8 newR = (Uint8)(this->r + (amount * (gray - (int)this->r)) / 255);
    Uint8 newG = (Uint8)(this->g + (amount * (gray - (int)this->g)) / 255);
    Uint8 newB = (Uint8)(this->b + (amount * (gray - (int)this->b)) / 255);

    return Color(newR, newG, newB);
}
