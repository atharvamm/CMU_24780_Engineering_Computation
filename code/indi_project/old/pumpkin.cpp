#include "fssimplewindow.h"
#include <cmath>
#include <vector>
#include "primitives.h"
#include "pumpkin.h"
#include "trinagulate.h"



// void drawTriangle(float centerX,float centerY, float triangleSize, float rotationAngle) {
//     // Calculate the vertices of the equilateral triangle based on the center and size
//     float x1 = centerX - triangleSize / 2;
//     float y1 = centerY - (triangleSize * 0.866);
//     float x2 = centerX + triangleSize / 2;
//     float y2 = y1;
//     float x3 = centerX;
//     float y3 = centerY + (triangleSize * 0.866);

//     // Convert the rotation angle from degrees to radians
//     float angleRad = rotationAngle * M_PI / 180.0f;

//     // Apply the rotation transformation
//     float cosA = cos(angleRad);
//     float sinA = sin(angleRad);

//     float x1Rotated = centerX + (x1 - centerX) * cosA - (y1 - centerY) * sinA;
//     float y1Rotated = centerY + (x1 - centerX) * sinA + (y1 - centerY) * cosA;

//     float x2Rotated = centerX + (x2 - centerX) * cosA - (y2 - centerY) * sinA;
//     float y2Rotated = centerY + (x2 - centerX) * sinA + (y2 - centerY) * cosA;

//     float x3Rotated = centerX + (x3 - centerX) * cosA - (y3 - centerY) * sinA;
//     float y3Rotated = centerY + (x3 - centerX) * sinA + (y3 - centerY) * cosA;

//     glColor3f(0.0f, 0.0f, 0.0f);

//     // Draw the rotated triangle
//     glBegin(GL_TRIANGLES);
//     glVertex2f(x1Rotated, y1Rotated);
//     glVertex2f(x2Rotated, y2Rotated);
//     glVertex2f(x3Rotated, y3Rotated);
//     glEnd();
// }
    
Pumpkin::Pumpkin(float centerX,float centerY){
    cx = centerX;
    cy = centerY;

    // Stem
    std::vector<std::vector<int>> stemPointsHelper = {
        {-40,0},{-50,-10},{-25,-15},{-25,-25},
        {0,-35},{5,-32},{10,-26},{8,-20},
        {4,-27},{0,-30},{-15,-25},{-15,-15},
        {10,-10},{0,0},{-15,-5},{-25,-5}};
    std::vector<Point> stemPoints;
    for (auto point : stemPointsHelper){
        stemPoints.push_back(Point(cx+point[0],cy+point[1]));
    }
    stemTris = triangulateConcavePolygon(stemPoints);

    // Mouth
    std::vector<std::vector<int>> mouthPointsHelper = {{-25,50},{-45,30},{-20,35},{5,30},{-15,50},{-20,47}};
    std::vector<Point> mouthPoints;
    for (auto point : mouthPointsHelper){
        mouthPoints.push_back(Point(cx+point[0],cy+point[1]));
    }
    mouthTris = triangulateConcavePolygon(mouthPoints);

    // Nose
    nosePts = {Point(cx - 30,cy + 25),Point(cx - 20,cy + 5),Point(cx - 20,cy + 20),Point(cx - 10,cy + 25)};

    // Eye
    eye1.x = cx - 40;eye1.y = cy+5;
    eye2.x = cx;eye2.y = cy+5;
    eye1Size = 12;eye2Size = 12;
    eye1Angle = 135;eye2Angle = 225;


}

void Pumpkin::drawStem(){
        glColor3f(0.0,0.5,0.0);
        glBegin(GL_TRIANGLES);
        for (const auto& triangle : stemTris) {
            for (const Point& point : triangle) {
                glVertex2i(point.x,point.y);
            }
        }
        glEnd();
        }



void Pumpkin::drawBody() {
    // Set the drawing color for the pumpkin body
    glColor3f(1.0, 0.5, 0.0);  // Orange color

    glBegin(GL_POLYGON);
    for (int i = 0; i < 360; i++) {
        float angle = i * 3.14159265359 / 180.0;
        float x = cx - 20 +  55 * cos(angle);  // Adjust the radius as needed
        float y = cy + 28 + 40 * sin(angle);  // Adjust the radius as needed
        glVertex2f(x, y);
    }
    glEnd();
}

        
void Pumpkin::drawEyesandNose(){
            drawTriangle(eye1.x,eye1.y,eye1Size,eye1Angle,0,0,0);
            drawTriangle(eye2.x,eye2.y,eye2Size,eye2Angle,0,0,0);

            glColor3f(0.0,0.0,0.0);
            glBegin(GL_TRIANGLE_STRIP);
            for (Point point : nosePts){
                glVertex2f(point.x,point.y);
            }
            glEnd();

        }

void Pumpkin::drawMouth(){
    glColor3f(0.0,0.0,0.0);
    glBegin(GL_TRIANGLES);
    for (const auto& triangle : mouthTris) {
        for (const Point& point : triangle) {
            glVertex2i(point.x,point.y);
        }
    }
    glEnd();
    }

void Pumpkin::drawPumpkin(){
    this->drawStem();
    this->drawBody();
    this->drawMouth();
    this->drawEyesandNose();
}








/* 
1. Rotating Pumpkin
2. Human Skeleton Moving in Air
3. Blank Screen
4. Final Ghost
*/











