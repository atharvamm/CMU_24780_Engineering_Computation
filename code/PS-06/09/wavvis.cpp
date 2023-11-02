#include <string>
#include <iostream>
#include "fssimplewindow.h"
#include "yssimplesound.h"
#include <stdio.h>
#include <stdlib.h>

int main(void)
{
    YsSoundPlayer::SoundData wav;

	std::string fileName;
	std::cout << "Enter Wav File Name:";
	std::getline(std::cin,fileName);

    if(YSOK!=wav.LoadWav(fileName.c_str()))
    {
        std::cout << "Failed to read %s\n";
        return 1;
    }


    FsOpenWindow(0,0,800,600,1);

    int ptr0=0;
    int range=wav.GetNumSamplePerChannel();



    for(;;)
    {
        FsPollDevice();
        auto key=FsInkey();
        if(FSKEY_ESC==key)
        {
            break;
        }
        if(FSKEY_Z==key)
        {
            range/=2;
            if(range<500)
            {
                range=500;
            }
        }
        else if(FSKEY_M==key)
        {
            range*=2;
            if(wav.GetNumSamplePerChannel()<range)
            {
                range=wav.GetNumSamplePerChannel();
            }
        }
        else if(FSKEY_LEFT==key)
        {
            ptr0+=range/4;
        }
        else if(FSKEY_RIGHT==key)
        {
            ptr0-=range/4;
        }


        glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

        int wid,hei;
        FsGetWindowSize(wid,hei);
        for(int channel=0; channel<wav.GetNumChannel(); ++channel)
        {
            switch(channel)
            {
            case 0:
                glColor3ub(0,0,0);
                break;
            case 1:
                glColor3ub(255,0,0);
                break;
            }
            glBegin(GL_LINE_STRIP);
            for(long long int x=0; x<wid; ++x)
            {
                long long ptr=ptr0+x*range/wid;

                int y=wav.GetSignedValue16(channel,ptr);
                y=hei/2-y*(hei/5)/32767;

                glVertex2i(x,y);
            }
            glEnd();
        }

        FsSwapBuffers();
        FsSleep(10);
    }
    FsCloseWindow();
    return 0;
}
