#include <iostream>
#include "fssimplewindow.h"

int main(void)
{
    FsOpenWindow(100,100,256,256,1);
    // Character for the cell (x,y) will be pattern[y*16+x]
    char pattern[]=
    {
        // By the way, if you want to define a long string, you can write like this.
        "...1........1..."
        "...11......11..."
        "...1.1....1.1..."
        "...1..1..1..1..."
        "...1..1111..1..."
        "..1..........1.."
        "..1..11..11..1.."
        "..1..........1.."
        ".1..111..111..1."
        ".1.1.1.11.1.1.1."
        "1..1.1.11.1.1..1"
        "1...111..111...1"
        ".1............1."
        ".1.....11.....1."
        "..111......111.."
        ".....111111....."
    };

    for (;;){
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    glPointSize(1);
    glBegin(GL_POINTS);
    for (int x = 0; x< 256; x ++){
        for (int y = 0; y < 256; y ++){
            if(true == ('1' == pattern[(y/16)*16+(x/16)])){
                glColor3f(0.0,0.0,0.0);}
            else{
                glColor3f(1.0,1.0,1.0);
            }
            glVertex2i(x,y);
        }
    }
    glEnd();

    FsPollDevice();
    auto key = FsInkey();
    if (FSKEY_ESC == key){
        break;
    }
    FsSwapBuffers();
    }
    
    return 0;
} 