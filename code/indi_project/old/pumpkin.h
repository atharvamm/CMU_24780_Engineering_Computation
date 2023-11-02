#ifndef PUMPKIN_H
#define PUMPKIN_H

#include "primitives.h"
#include <vector>

// void drawTriangle(float centerX,float centerY, float triangleSize, float rotationAngle);

class Pumpkin{
    public:
        float cx,cy;
        std::vector<std::vector<Point>> stemTris,mouthTris;
        std::vector<Point> nosePts;
        Point eye1,eye2;
        int eye1Size,eye2Size,eye1Angle,eye2Angle;

        Pumpkin(float centerX, float centerY);
        void drawStem();
        void drawBody();
        void drawEyesandNose();
        void drawMouth();
        void drawPumpkin();
};

#endif  // PUMPKIN_H
