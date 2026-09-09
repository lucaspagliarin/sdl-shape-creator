#ifndef POINT_H
#define POINT_H


class Point
{
    public:
        Point();
        Point(int x, int y);
        int getX()const;
        int getY()const;
        void setX(int x);
        void setY(int y);
        virtual ~Point();

    protected:

    private:
        int x, y;
};

#endif // POINT_H
