#ifndef UTILS_H
#define UTILS_H
#include <Point.h>

class Utils
{
    public:
        static Utils * getInstance();
        static Point midPoint(Point p1, Point p2);
        static double distance(Point a, Point b);
    protected:
        Utils();
    private:
        virtual ~Utils();
        static Utils * _instance;
};

#endif // UTILS_H