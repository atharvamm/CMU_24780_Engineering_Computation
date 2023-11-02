#include <string>
#include <iostream>

#include "yssimplesound.h"


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

	player.Start();

	player.PlayOneShot(wav);

	while(YSTRUE==player.IsPlaying(wav))
	{
		player.KeepPlaying();
	}

	player.End();

	return 0;
}
