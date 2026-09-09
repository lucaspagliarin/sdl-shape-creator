#include "Utils.h"
#include "Context.h"
#include<SDL2/SDL.h>
#include <stdio.h>
#include <unistd.h>

#include <cmath>
#include <string>
#include <chrono>
#include <ctime>
#include <sstream>
#include <iomanip>

using namespace std;

Utils * Utils::_instance = nullptr;

Utils::Utils()
{
    //ctor
}

Utils::~Utils()
{
    //dtor
}

Utils * Utils::getInstance()
{
    if(_instance == 0) {
        _instance = new Utils();
    }
    return _instance;
}

Point Utils::midPoint(Point p1, Point p2) {
    int newX = (p1.getX() + p2.getX()) / 2;
    int newY = (p1.getY() + p2.getY()) / 2;
    return Point(newX, newY);
}

double Utils::distance(Point a, Point b) {
    double dx = a.getX() - b.getX();
    double dy = a.getY() - b.getY();
    return sqrt(dx * dx + dy * dy);
}

string Utils::generateTimestamp() {
    auto now = std::chrono::system_clock::now();
    std::time_t currentTime = std::chrono::system_clock::to_time_t(now);
    std::tm localTime = *std::localtime(&currentTime);

    std::ostringstream oss;
    oss << std::put_time(&localTime, "%Y%m%d_%H%M%S");
    return oss.str();
}

void Utils::saveFile() {
    SDL_Surface* windowSurface = Context::getInstance()->getWindowSurface();
    if (!windowSurface) {
        printf("Nao foi possivel salvar: surface da janela indisponivel.\n");
        return;
    }

    string name = "desenho_" + generateTimestamp() + ".bmp";

    if (SDL_SaveBMP(windowSurface, name.c_str()) == 0) {
        printf("Arquivo salvo com sucesso: %s\n", name.c_str());
    } else {
        printf("Erro ao salvar: %s\n", SDL_GetError());
    }
}