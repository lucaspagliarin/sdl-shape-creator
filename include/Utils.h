#ifndef UTILS_H
#define UTILS_H
#include "Utils.h"
#include "Context.h"
#include "Point.h"
#include <SDL2/SDL.h>
#include <stdio.h>
#include <unistd.h>

#include <cmath>
#include <string>
#include <chrono>
#include <ctime>
#include <sstream>
#include <iomanip>

using namespace std;

class Utils
{
    public:
        static Utils * getInstance();
        static Point midPoint(Point p1, Point p2);
        static double distance(Point a, Point b);
        static string generateTimestamp();
        static void saveFile();
    protected:
        Utils();
    private:
        virtual ~Utils();
        static Utils * _instance;
};

#endif // UTILS_H