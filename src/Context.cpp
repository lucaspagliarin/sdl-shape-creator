#include "Context.h"

Context * Context::_instance = nullptr;

Context::Context() : 
    _window_surface(nullptr), 
    _pRenderer(nullptr), 
    _pFontSurface(nullptr),
    _charWidth(0),
    _charHeight(0),
    _charsPerRow(16),
    _startingChar(' ') // ASCII 32 is the space character
{
}

Context::~Context()
{
    if (_pFontSurface != nullptr) {
        SDL_FreeSurface(_pFontSurface);
        _pFontSurface = nullptr;
    }
}

Context * Context::getInstance()
{
    if(_instance == 0) {
        _instance = new Context();
    }
    return _instance;
}


void Context::setWindowSurface(SDL_Surface * window_surface)
{
    _window_surface = window_surface;
}

void Context::setRenderer(SDL_Renderer * pRenderer)
{
    _pRenderer = pRenderer;
}

SDL_Surface * Context::getWindowSurface()
{
    return _window_surface;
}

SDL_Renderer* Context::getRenderer()
{
    return _pRenderer;
}

bool Context::loadBitmapFont(const std::string& bmpPath, int charWidth, int charHeight, int charsPerRow)
{
    //if (_pRenderer == nullptr) return false;
    
    if (_pFontSurface != nullptr) {
        SDL_FreeSurface(_pFontSurface);
        _pFontSurface = nullptr;
    }

    // Load BMP using purely native SDL2 (No SDL_image required)
    _pFontSurface = SDL_LoadBMP(bmpPath.c_str());
    if (_pFontSurface == nullptr) {
        return false;
    }

    SDL_SetColorKey(_pFontSurface, SDL_TRUE, SDL_MapRGB(_pFontSurface->format, 255, 0, 255));

    _charWidth = charWidth;
    _charHeight = charHeight;
    _charsPerRow = charsPerRow;
    return true;
}

void Context::drawChar(char c, int x, int y, int scale, Color color)
{
    // Verifica se a superfície da janela e da fonte estão disponíveis
    if (_window_surface == nullptr || _pFontSurface == nullptr) return;

    int asciiIndex = c - _startingChar;
    if (asciiIndex < 0) return;

    int cellX = (asciiIndex % _charsPerRow) * _charWidth;
    int cellY = (asciiIndex / _charsPerRow) * _charHeight;

    SDL_Rect srcRect = { cellX, cellY, _charWidth, _charHeight };
    SDL_Rect dstRect = { x, y, _charWidth * scale, _charHeight * scale };

    SDL_SetSurfaceColorMod(_pFontSurface, color.getR(), color.getG(), color.getB());

    // Copia e redimensiona (se scale > 1) a área do caractere para a _window_surface
    SDL_BlitScaled(_pFontSurface, &srcRect, _window_surface, &dstRect);
}

void Context::drawString(const std::string& text, int x, int y, int scale, Color color)
{
    int currentX = x;
    for (char c : text) {
        if (c == '\n') {
            y += _charHeight * scale;
            currentX = x;            
        } else {
            drawChar(c, currentX, y, scale, color);
            currentX += _charWidth * scale;
        }
    }
}