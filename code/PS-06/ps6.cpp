#include <string>
#include <iostream>
#include "fssimplewindow.h"
#include "yssimplesound.h"


int pointer(std::string fileName){
	// You need to have an instance of YsSoundPlayer.
	YsSoundPlayer player;
    YsSoundPlayer::SoundData notice;

	player.MakeCurrent();
	player.Start();

	FsChangeToProgramDir();

	if(YSOK!=notice.LoadWav(fileName.c_str()))
	{
		std::cout<<"Error!  Cannot load "<<fileName<<"!\n"<<std::endl;
        return -1;
	}

    int ptr0=0;
    int range=notice.GetNumSamplePerChannel();


	for(;;)
	{
        glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
		FsPollDevice();
        int wid,hei,draw_center;
        FsGetWindowSize(wid,hei);

		auto key=FsInkey();
		if(FSKEY_ESC==key)
		{
			break;
		}

        player.Start();
        player.PlayOneShot(notice);
        while(YSTRUE==player.IsPlaying(notice))
        {   
            glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
            auto currentTime=player.GetCurrentPosition(notice);
            auto currentPos=notice.SecToNumSample(currentTime);
            long long ptr= 0;

            if (notice.GetNumChannel() > 1)
            {
                ptr=(currentPos/(double)(range))*wid;

                int y1 = notice.GetSignedValue16(0,currentPos);
                y1 = 150 - y1*(hei/2)/32767;
                glPointSize(8);
                glColor3ub(255,0,0);
                glBegin(GL_POINTS);
                glVertex2i(ptr,y1);
                glEnd();

                int y2 = notice.GetSignedValue16(0,currentPos);
                y2 = 450 - y2*(hei/2)/32767;
                glPointSize(8);
                glColor3ub(0,0,255);
                glBegin(GL_POINTS);
                glVertex2i(ptr,y2);
                glEnd();
            }

            else {
                ptr=(currentPos/(double)(2*range))*wid;

                int y = notice.GetSignedValue16(0,currentPos);
                y = 150 - y*(hei/2)/32767;
                glPointSize(8);
                glColor3ub(255,0,0);
                glBegin(GL_POINTS);
                glVertex2f(ptr,y);
                glEnd();
            }

            glColor3ub(0,0,0);
            glBegin(GL_LINES);
            glVertex2i(0,150);
            glVertex2i(800,150);

            glVertex2i(0,450);
            glVertex2i(800,450);
            glEnd();


            glColor3ub(0,255,0);
            glBegin(GL_LINES);
            glVertex2f(ptr,0);
            glVertex2f(ptr,hei);
            glEnd();

            player.KeepPlaying();

            for(int channel=0; channel<notice.GetNumChannel(); ++channel)
            {
            switch(channel)
            {
            case 0:
                glColor3ub(255,0,0);
                draw_center = 150;
                break;
            case 1:
                glColor3ub(0,0,255);
                draw_center = 450;
                break;
            }
            glBegin(GL_LINE_STRIP);
            for(long long int x=0; x<wid; ++x)
            {
                long long ptr=ptr0+x*range/wid;
                int y=notice.GetSignedValue16(channel,ptr);
                y = draw_center-y*(hei/2)/32767;

                glVertex2i(x,y);
            }
            glEnd();
            }
        FsSwapBuffers();
        FsSleep(10);
        }
    break;
    }
    return 0;
}


int main(void){
    std::string fileName;
    int state = -1;
    bool new_window = true;

    while (true){

    std::cout << "Paste .wav file in the same directory as the code." <<std::endl;
    std::cout << "Enter name of file when prompted." <<std::endl;
    std::cout << "Enter name of .wav file:";
    std::getline(std::cin,fileName);

    if (new_window){FsOpenWindow(50,50,800,600,1);}
    new_window = false;
    if (fileName.empty()){
        break;}
    else{
        state = pointer(fileName);
        if (-1 == state){
            std::cout<<"Please enter a different filename!!!"<<std::endl;
        }
    }
}
FsCloseWindow();
return 0;
}








