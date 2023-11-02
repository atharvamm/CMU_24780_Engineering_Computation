#include <stdio.h>
#include <time.h>
#include <math.h>
#include "fssimplewindow.h"
#include <iostream>

const double YsPi=3.1415927;

void DrawCircle(double cx,double cy,double rad,unsigned char r,unsigned char g,unsigned char b)
{
	glColor3ub(r,g,b);
	glBegin(GL_LINE_LOOP);
	for(int i=0; i<64; ++i)
	{
		double ang=YsPi*(double)i/32.0;
		double s=sin(ang);
		double c=cos(ang);
		double x=cx+rad*c;
		double y=cy+rad*s;
		glVertex2d(x,y);
	}
	glEnd();
}

int main(void){
    FsOpenWindow(50,50,600,600,1);
    int lb,mb,rb,mx,my;
    for (;;){
        glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
        FsPollDevice();
        auto key = FsInkey();
        if(FSKEY_NULL== key)
        {
        FsGetMouseEvent(lb,mb,rb,mx,my);
        printf("%d %d %d %d %d\n",lb,mb,rb,mx,my);
        }
        else if (FSKEY_SPACE == key){
        DrawCircle(mx,my,50,0,0,0);
        }
        else{
            break;
        }
        FsSwapBuffers();
        FsSleep(20);
    }

    

    return 0;
}




