#include <string>
#include <iostream>

#include "yssimplesound.h"
#include "fssimplewindow.h"


int main(void)
{
	YsSoundPlayer player;
	YsSoundPlayer::SoundData wav;

	std::string fileName;
	std::cout << "Enter Wav File Name:";
	std::getline(std::cin,fileName);

	if(YSOK!=wav.LoadWav(fileName.c_str()))
	{
		std::cout << "Cannot load wave file." << std::endl;
		return 1;
	}

	FsOpenWindow(0,0,800,600,1);
	player.Start();

	player.PlayOneShot(wav);

	for(;;)
	{
		FsPollDevice();
		player.KeepPlaying();


		auto key=FsInkey();
		if(FSKEY_ESC==key)
		{
			break;
		}

		if(FSKEY_SPACE==key)
		{
			player.PlayOneShot(wav);
		}
	}

	player.End();

	return 0;
}
