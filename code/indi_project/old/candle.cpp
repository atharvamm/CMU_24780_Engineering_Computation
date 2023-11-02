#include "fssimplewindow.h"
#include "candle.h"



Candle::Candle(float startX, float startY)
{
    sx = startX;
    sy = startY;
    float mainBodyHelper[6][2] = {{6, 25}, {-6, 25}, {6, -25}, {-6, -25}, {6, -35}, {6, -45}},
    wickHelper[6][2] = {{1, - 37},{-2,-37 + 14/2},{1,-41},{-2,-41},{1,-45},{-2,-43}}, 
    fireHelper[12][2] = {{3,-40},{-3,-40},{5,-50},{-5,-51},{6,-52},{-3,-53},{5,-54},{-1,-55},{3,-56},{-1,-57},{0,-58},{-5,-61}};


    int i = 0;
    for (const auto& point : mainBodyHelper) {
    mainBody[i][0] = sx + point[0];
    mainBody[i][1] = sy + point[1];
    i++;
    }

    i = 0;
    for (const auto& point : wickHelper) {
    wick[i][0] = sx + point[0];
    wick[i][1] = sy + point[1];
    i++;
    }

    i = 0;
    for (const auto& point : fireHelper) {
    fireBody[i][0] = sx + point[0];
    fireBody[i][1] = sy + point[1];
    i++;
}
}

bool Candle::getVisibility(){
            return objVisible;
        }

void Candle::switchVisibility(){
            objVisible = !objVisible;
        }

void Candle::fireVisbility(){
            fireVisible = !fireVisible; 
        }


//         // # To make the fire dynamic just move some of the points in random direction
//         void fireAnimation(){}

void Candle::fireStatic(){
            glColor3f(1,0.5,0.0);
            glBegin(GL_QUAD_STRIP);
            for (int i = 0; i < 12; i++){
                    glVertex2f(fireBody[i][0],fireBody[i][1]);
                }
            glEnd();
        }

void Candle::drawCandle(){
            // Wick
            glColor3f(0.2,0.2,0.2);
            glBegin(GL_QUAD_STRIP);
            for (int i = 0; i < 6; i++){
                glVertex2f(wick[i][0],wick[i][1]);
            }
            glEnd();

            this->fireStatic();

            // Main Body
            // glColor3f(0,0,0);
            glColor3f(0.9f, 0.9f, 0.9f);
            glBegin(GL_QUAD_STRIP);
            for (int i = 0; i < 6; i++){
                    glVertex2f(mainBody[i][0],mainBody[i][1]);
                }
            glEnd();
        }


