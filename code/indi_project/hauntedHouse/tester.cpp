#include "fssimplewindow.h"




int main(void) {
    FsOpenWindow(100,100,600,600,1);
    


    for (;;){
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);



    FsSwapBuffers();
    FsPollDevice();
    auto key = FsInkey();
    if (FSKEY_ESC == key){
        break;
    }
    FsSleep(100);
    }
    return 0;
}



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

        player.KeepPlaying();
        FsSwapBuffers();
        FsSleep(10);
        }
    break;
    }
    return 0;
}


int main(void){
    std::string fileName;
    bool new_window = true;
return 0;
}










// player.Start();
// player.PlayBackground(wav);
// player.PlayOneShot(wav);
// while(YSTRUE==player.IsPlaying(wav))
// {
// player.KeepPlaying();
// }
// player.End();




/*

int pointer(std::string fileName){
	if(YSOK!=notice.LoadWav(fileName.c_str()))
	{
		std::cout<<"Error!  Cannot load "<<fileName<<"!\n"<<std::endl;
        return -1;
	}
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

        player.KeepPlaying();
        FsSwapBuffers();
        FsSleep(10);
        }
    break;
    }
    return 0;
}


int main(void){
    std::string fileName;
    bool new_window = true;
return 0;
}
*/