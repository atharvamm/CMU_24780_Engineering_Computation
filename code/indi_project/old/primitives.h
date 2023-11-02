#ifndef PRIMITIVES_H
#define PRIMITIVES_H

#include "cmath"
#include "fssimplewindow.h"

class Point{
    
    public:
        double x,y;
        Point(double initX, double initY);

        Point();

        void translatePoint(double dist, char dir, bool positive);
        void rotatePoint(double cx, double cy, double degrees, bool clockwise);
        void drawPoint();
};


void drawTriangle(float centerX,float centerY, float triangleSize, float rotationAngle,float r,float g, float b);

#endif // PRIMITIVES_H