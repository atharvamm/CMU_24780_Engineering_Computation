#include <iostream>
#include <string>
#include "mmlplayer.h"
#include "fssimplewindow.h"
#include "yssimplesound.h"



int main(void)
{
	MMLSegmentPlayer mmlplayer;


	mmlplayer.AddSegment(
		"@26T200O4CDEFGAB",
		"",
		"",
		"",
		"",
		""
	);


	FsOpenWindow(0,0,800,600,0);

	YsSoundPlayer player;
	player.Start();

	YsSoundPlayer::Stream stream;
	player.StartStreaming(stream);

	YsSoundPlayer::SoundData nextWave;
	auto rawWave=mmlplayer.GenerateWave(100);  // Create for next 100ms
	nextWave.CreateFromSigned16bitStereo(YM2612::WAVE_SAMPLING_RATE,rawWave);

	if(0!=mmlplayer.GetLastErrorCode())
	{
		for(auto msg : mmlplayer.GetLastError().Format())
		{
			std::cout << msg << std::endl;
		}
	}

	for(;;)
	{
		FsPollDevice();

		player.KeepPlaying();  // <- This line is only needed for Linux ALSA.

		auto key=FsInkey();
		if(FSKEY_ESC==key)
		{
			break;
		}

		if(YSTRUE==player.StreamPlayerReadyToAcceptNextSegment(stream,nextWave))
		{
			player.AddNextStreamingSegment(stream,nextWave);
			auto rawWave=mmlplayer.GenerateWave(100);  /// Create for next 100ms
			nextWave.CreateFromSigned16bitStereo(YM2612::WAVE_SAMPLING_RATE,rawWave);

			if(0!=mmlplayer.GetLastErrorCode())
			{
				for(auto msg : mmlplayer.GetLastError().Format())
				{
					std::cout << msg << std::endl;
				}
			}
		}
	}

	player.End();

	return 0;
}
