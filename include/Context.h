#ifndef CONTEXT_H
#define CONTEXT_H
#include<SDL2/SDL.h>
#include <string>
#include<Color.h>


class Context
{
    public:
        static Context * getInstance();
        void setWindowSurface(SDL_Surface * window_surface);
        void setRenderer(SDL_Renderer * pRenderer);
        SDL_Surface * getWindowSurface();
        SDL_Renderer * getRenderer();

        bool loadBitmapFont(const std::string& bmpPath, int charWidth, int charHeight, int charsPerRow = 16);
        void drawChar(char c, int x, int y, int scale = 1, Color color = Color(255, 255, 255));
        void drawString(const std::string& text, int x, int y, int scale = 1, Color color = Color(255, 255, 255));

    protected:
        Context();
    private:
        virtual ~Context();
        static Context * _instance;
        SDL_Surface * _window_surface;
        SDL_Renderer * _pRenderer;

        SDL_Surface* _pFontSurface;
        int _charWidth;
        int _charHeight;
        int _charsPerRow;
        char _startingChar; 
};

#endif // CONTEXT_H
