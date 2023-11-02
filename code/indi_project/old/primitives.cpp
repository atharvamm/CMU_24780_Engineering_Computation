#include "cmath"
#include "fssimplewindow.h"
#include "primitives.h"

Point::Point(double initX, double initY){
            x = initX;
            y = initY;
        }

Point::Point(){
        x = 0;
        y = 0;
    }

void Point::translatePoint(double dist, char dir, bool positive){
        int multiplier = (positive)?1:-1;
        if ('X' == dir){
            x += multiplier*dist; 
        }
        else{
            y += multiplier*dist;
        }
    }

void Point::rotatePoint(double cx, double cy, double degrees, bool clockwise) {
    double radians = (M_PI / 180.0) * degrees;

    double s = std::sin(radians);
    double c = std::cos(radians);

    x -= cx;
    y -= cy;

    if (clockwise) {
        double new_x = x * c - y * s;
        double new_y = x * s + y * c;
        x = new_x;
        y = new_y;
    } else {
        double new_x = x * c + y * s;
        double new_y = -x * s + y * c;
        x = new_x;
        y = new_y;
    }

    x += cx;
    y += cy;
    }

void Point::drawPoint(){
    glPointSize(8);
    glColor3f(0,1,0);
    glBegin(GL_POINTS);
    glVertex2d(x,y);
    glEnd();    
}

void drawTriangle(float centerX,float centerY, float triangleSize, float rotationAngle, float r, float g, float b) {
    // Calculate the vertices of the equilateral triangle based on the center and size
    float x1 = centerX - triangleSize / 2;
    float y1 = centerY - (triangleSize * 0.866);
    float x2 = centerX + triangleSize / 2;
    float y2 = y1;
    float x3 = centerX;
    float y3 = centerY + (triangleSize * 0.866);

    // Convert the rotation angle from degrees to radians
    float angleRad = rotationAngle * M_PI / 180.0f;

    // Apply the rotation transformation
    float cosA = cos(angleRad);
    float sinA = sin(angleRad);

    float x1Rotated = centerX + (x1 - centerX) * cosA - (y1 - centerY) * sinA;
    float y1Rotated = centerY + (x1 - centerX) * sinA + (y1 - centerY) * cosA;

    float x2Rotated = centerX + (x2 - centerX) * cosA - (y2 - centerY) * sinA;
    float y2Rotated = centerY + (x2 - centerX) * sinA + (y2 - centerY) * cosA;

    float x3Rotated = centerX + (x3 - centerX) * cosA - (y3 - centerY) * sinA;
    float y3Rotated = centerY + (x3 - centerX) * sinA + (y3 - centerY) * cosA;

    // Set the drawing color (e.g., red)
    glColor3f(r, g, b);

    // Draw the rotated triangle
    glBegin(GL_TRIANGLES);
    glVertex2f(x1Rotated, y1Rotated);
    glVertex2f(x2Rotated, y2Rotated);
    glVertex2f(x3Rotated, y3Rotated);
    glEnd();
}