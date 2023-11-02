#include "fssimplewindow.h"
#include <iostream>
#include <cmath>

const int WINDOW_WIDTH = 800,WINDOW_HEIGHT = 600, WINDOW_X = 50,WINDOW_Y = 50;


void drawStipple(float centerX,float centerY){
    glEnable(GL_LINE_STIPPLE);
	glLineStipple(1,0b1111111100000000);
	glColor3ui(25,100,50);

    glBegin(GL_LINES);
	glVertex2i(WINDOW_WIDTH/2,WINDOW_HEIGHT/2);
	glVertex2i(centerX,centerY);
	glEnd();
}


void drawPoint(float x, float y, float size = 4.0) {
    glColor3f(1.0, 0.0, 0.0);
    glPointSize(size);
    glBegin(GL_POINTS);
    glVertex2f(x, y);
    glEnd();
}

void drawRotatingTriangleFan(float centerX, float centerY, float angle, float radius, int numVertices) {
    glColor3f(0.0, 0.0, 1.0);
    glBegin(GL_TRIANGLE_FAN);
    
    // Center point
    glVertex2f(centerX, centerY);

    // Calculate the angle between vertices
    float angleIncrement = 360.0f / numVertices;

    for (int i = 0; i <= numVertices; ++i) {
        if (1 == i % 2){continue;} 
        float angle = i * angleIncrement;
        float x = centerX + radius * cos(angle * 3.14159265 / 180.0);
        float y = centerY + radius * sin(angle * 3.14159265 / 180.0);
        glVertex2f(x, y);

    }

    glEnd();
}


int main(void){
    float centerX,centerY;
    int angle = 0;
    std::cout<<"Please Enter Center for Fan (X Y):";
    std::cin>>centerX>>centerY;

    FsOpenWindow(WINDOW_X,WINDOW_Y,WINDOW_WIDTH,WINDOW_HEIGHT,1);
    while(true){
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    FsPollDevice();
    auto key = FsInkey();
    if (key == FSKEY_ESC){
        break;
    }
    switch(key)
		{
		case FSKEY_UP:
            centerY = centerY - 5;
            if (centerY < 10){centerY = 10;}
            break;
		case FSKEY_DOWN:
            centerY = centerY + 5;
            if (centerY > WINDOW_HEIGHT - 10){centerY = WINDOW_HEIGHT - 10;}
			break;
		case FSKEY_LEFT:
            centerX = centerX - 5;
            if (centerX < 10){centerX = 10;}
            break;
        case FSKEY_RIGHT:
            centerX = centerX + 5;
            if (centerX > WINDOW_WIDTH - 10){centerX = WINDOW_WIDTH - 10;}
            break;
        }

    drawRotatingTriangleFan(centerX, centerY, angle, 50.0, 6);
    drawPoint(centerX, centerY);
    drawStipple(centerX,centerY);
    angle = angle + 10;

    FsSwapBuffers();
    FsSleep(20);
    }
    return 0;
}