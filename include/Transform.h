#ifndef TRANSFORM_H
#define TRANSFORM_H
#include "Point.h"

class Transform
{
    public:
        Transform();
        virtual ~Transform();
        void setTranslation(Point& position);
        void setRotation(float angleDegrees);
        void setScale(float sx, float sy);
        void computeFinalMatrix(Point pivot, Point position);
        Point apply(const Point& p);
    protected:

    private:
    float mTranslation[3][3];
    float mRotation[3][3];
    float mScale[3][3];

    float mFinal[3][3];

    void setIdentity(float matrix[3][3]);
};

#endif // TRANSFORM_H
