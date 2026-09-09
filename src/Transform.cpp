#include "Transform.h"
#include "Point.h"
#include <cmath>
#define M_PI 3.14159265358979323846

Transform::~Transform()
{
    //dtor
}

void Transform::setIdentity(float matrix[3][3]) {
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            matrix[i][j] = (i == j) ? 1.0f : 0.0f;
        }
    }
}

Transform::Transform()
{
    setIdentity(mTranslation);
    setIdentity(mRotation);
    setIdentity(mScale);
    setIdentity(mFinal);
}

void Transform::setTranslation(Point& position)
{
    setIdentity(mTranslation);
    mTranslation[0][2] = position.getX();
    mTranslation[1][2] = position.getY();
}
void Transform::setRotation(float angleDegrees) {
    setIdentity(mRotation);
    float rad = angleDegrees * (M_PI / 180.0f);
    float cosA = std::cos(rad);
    float sinA = std::sin(rad);

    mRotation[0][0] = cosA;  mRotation[0][1] = -sinA;
    mRotation[1][0] = sinA;  mRotation[1][1] =  cosA;
}
void Transform::setScale(float sx, float sy) {
    setIdentity(mScale);
    mScale[0][0] = sx;
    mScale[1][1] = sy;
}
static void multiply3x3(const float A[3][3], const float B[3][3], float out[3][3]) {
    float temp[3][3];
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            temp[i][j] = 0.0f;
            for (int k = 0; k < 3; ++k) {
                temp[i][j] += A[i][k] * B[k][j];
            }
        }
    }
    for (int i = 0; i < 3; ++i)
        for (int j = 0; j < 3; ++j)
            out[i][j] = temp[i][j];
}

void Transform::computeFinalMatrix(Point pivot, Point position) {

    float toOrigin[3][3];
    setIdentity(toOrigin);
    toOrigin[0][2] = -pivot.getX();
    toOrigin[1][2] = -pivot.getY();

    float backFromOrigin[3][3];
    setIdentity(backFromOrigin);
    backFromOrigin[0][2] = pivot.getX() + position.getX();
    backFromOrigin[1][2] = pivot.getY() + position.getY();


    float temp1[3][3];
    float temp2[3][3];

    multiply3x3(mScale, toOrigin, temp1);
    multiply3x3(mRotation, temp1, temp2);
    multiply3x3(backFromOrigin, temp2, mFinal);
}
Point Transform::apply(const Point& p) {
    float x = p.getX();
    float y = p.getY();

    float newX = mFinal[0][0] * x + mFinal[0][1] * y + mFinal[0][2];
    float newY = mFinal[1][0] * x + mFinal[1][1] * y + mFinal[1][2];

    return Point(std::round(newX), std::round(newY));
}
